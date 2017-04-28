/*****************************************************************************

    area_stats_and_report

*****************************************************************************/

#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include <gdalcpp.hpp>

#include <osmium/area/assembler.hpp>
#include <osmium/area/multipolygon_collector.hpp>
#include <osmium/area/problem_reporter_ogr.hpp>
#include <osmium/handler/dump.hpp>
#include <osmium/index/map/dense_mmap_array.hpp>
#include <osmium/index/map/sparse_mmap_array.hpp>
#include <osmium/index/node_locations_map.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/util/memory.hpp>
#include <osmium/util/verbose_output.hpp>
#include <osmium/visitor.hpp>

#include "sqlite.hpp"

using index_type = osmium::index::map::Map<osmium::unsigned_object_id_type, osmium::Location>;
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

class LastTimestampHandler : public osmium::handler::Handler {

    osmium::Timestamp m_timestamp;

public:

    LastTimestampHandler() :
        m_timestamp(osmium::start_of_time()) {
    }

    void osm_object(const osmium::OSMObject& object) noexcept {
        if (object.timestamp() > m_timestamp) {
            m_timestamp = object.timestamp();
        }
    }

    osmium::Timestamp get_timestamp() const noexcept {
        return m_timestamp;
    }

}; // class LastTimestampHandler

void print_help() {
    std::cout << "area_stats_and_report [OPTIONS] OSMFILE\n\n"
              << "Report errors and stats from assembling areas.\n"
              << "\nOptions:\n"
              << "  -d, --debug[=LEVEL]    Set area assembler debug level\n"
              << "  -e, --errors=DBNAME    Database name for errors\n"
              << "  -h, --help             This help message\n"
              << "  -i, --index=INDEX_TYPE Set index type for location index (default: sparse_mmap_array)\n"
              << "  -I, --show-index-types Show available index types for location index\n"
              << "  -s, --stats=DBNAME     Database name for statistics\n"
              ;
}

using collector_type = osmium::area::MultipolygonCollector<osmium::area::Assembler>;

void read_relations(collector_type& collector, const osmium::io::File& file) {
    osmium::io::Reader reader(file, osmium::osm_entity_bits::relation);
    collector.read_relations(reader);
    reader.close();
}

void insert(Sqlite::Statement& statement, const std::string& time, const char* key, uint64_t value) {
    statement.bind_text(time)
             .bind_text(key)
             .bind_int64(value)
             .execute();
}

void write_stats(const std::string& database_name, const std::string& last_time, const osmium::area::area_stats& stats) {
    Sqlite::Database db{database_name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE};

    db.exec("CREATE TABLE IF NOT EXISTS stats (date TEXT, key TEXT, value INT64 DEFAULT 0);");

    Sqlite::Statement stmt{db, "INSERT INTO stats (date, key, value) VALUES (?, ?, ?);"};

    insert(stmt, last_time, "area_really_complex_case", stats.area_really_complex_case);
    insert(stmt, last_time, "area_simple_case", stats.area_simple_case);
    insert(stmt, last_time, "area_touching_rings_case", stats.area_touching_rings_case);
    insert(stmt, last_time, "duplicate_nodes", stats.duplicate_nodes);
    insert(stmt, last_time, "duplicate_segments", stats.duplicate_segments);
    insert(stmt, last_time, "from_relations", stats.from_relations);
    insert(stmt, last_time, "from_ways", stats.from_ways);
    insert(stmt, last_time, "inner_rings", stats.inner_rings);
    insert(stmt, last_time, "inner_with_same_tags", stats.inner_with_same_tags);
    insert(stmt, last_time, "intersections", stats.intersections);
    insert(stmt, last_time, "member_ways", stats.member_ways);
    insert(stmt, last_time, "no_tags_on_relation", stats.no_tags_on_relation);
    insert(stmt, last_time, "no_way_in_mp_relation", stats.no_way_in_mp_relation);
    insert(stmt, last_time, "nodes", stats.nodes);
    insert(stmt, last_time, "open_rings", stats.open_rings);
    insert(stmt, last_time, "outer_rings", stats.outer_rings);
    insert(stmt, last_time, "short_ways", stats.short_ways);
    insert(stmt, last_time, "single_way_in_mp_relation", stats.single_way_in_mp_relation);
    insert(stmt, last_time, "touching_rings", stats.touching_rings);
    insert(stmt, last_time, "ways_in_multiple_rings", stats.ways_in_multiple_rings);
    insert(stmt, last_time, "wrong_role", stats.wrong_role);
}

int main(int argc, char* argv[]) {
    osmium::util::VerboseOutput vout{true};

    static const struct option long_options[] = {
        {"debug",      optional_argument, 0, 'd'},
        {"errors",     required_argument, 0, 'e'},
        {"help",       no_argument,       0, 'h'},
        {"index",      required_argument, 0, 'i'},
        {"show-index", no_argument,       0, 'I'},
        {"stats",      required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    std::string errors_database_name{"areas.db"};
    std::string stats_database_name{"stats.db"};
    std::string location_index_type{"sparse_mmap_array"};

    const auto& map_factory = osmium::index::MapFactory<osmium::unsigned_object_id_type, osmium::Location>::instance();

    osmium::area::Assembler::config_type assembler_config;
    assembler_config.create_empty_areas = false;
    assembler_config.check_roles = true;

    while (true) {
        int c = getopt_long(argc, argv, "d::e:hi:Is:", long_options, 0);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'd':
                assembler_config.debug_level = optarg ? std::atoi(optarg) : 1;
                break;
            case 'e':
                errors_database_name = optarg;
                break;
            case 'h':
                print_help();
                exit(0);
            case 'i':
                location_index_type = optarg;
                break;
            case 'I':
                std::cout << "Available index types:\n";
                for (const auto& map_type : map_factory.map_types()) {
                    std::cout << "  " << map_type;
                    if (map_type == location_index_type) {
                        std::cout << " (default)";
                    }
                    std::cout << '\n';
                }
                exit(0);
            case 's':
                stats_database_name = optarg;
                break;
            default:
                exit(2);
        }
    }

    int remaining_args = argc - optind;
    if (remaining_args != 1) {
        std::cerr << "Usage: " << argv[0] << " [OPTIONS] OSMFILE\n";
        exit(2);
    }

    LastTimestampHandler last_timestamp;

    auto location_index = map_factory.create_map(location_index_type);
    location_handler_type location_handler(*location_index);
    location_handler.ignore_errors(); // XXX

    const osmium::io::File input_file(argv[optind]);

    CPLSetConfigOption("OGR_SQLITE_SYNCHRONOUS", "OFF");
    osmium::geom::OGRFactory<> factory;

    gdalcpp::Dataset dataset{"SQLite", errors_database_name, gdalcpp::SRS{factory.proj_string()}, { "SPATIALITE=TRUE", "INIT_WITH_EPSG=NO" }};
    dataset.enable_auto_transactions();
    dataset.exec("PRAGMA journal_mode = OFF;");

    gdalcpp::Layer dummy_area_layer{dataset, "areas", wkbMultiPolygon};
    dummy_area_layer.add_field("id", OFTInteger, 10);
    dummy_area_layer.add_field("valid", OFTInteger, 1);
    dummy_area_layer.add_field("source", OFTString, 1);
    dummy_area_layer.add_field("orig_id", OFTInteger, 10);

    osmium::area::ProblemReporterOGR problem_reporter{dataset};
    assembler_config.problem_reporter = &problem_reporter;

    collector_type collector(assembler_config);

    vout << "Starting first pass (reading relations)...\n";
    read_relations(collector, input_file);
    vout << "First pass done.\n";

    vout << "Memory:\n";
    collector.used_memory();

    vout << "Starting second pass (reading nodes and ways and assembling areas)...\n";
    osmium::io::Reader reader2(input_file);

    osmium::apply(reader2, location_handler, last_timestamp, collector.handler([](osmium::memory::Buffer&&) {
    }));

    reader2.close();
    vout << "Second pass done\n";

    dataset.commit_transaction();
    collector.used_memory();

    const std::string last_time = last_timestamp.get_timestamp().to_iso();
    write_stats(stats_database_name, last_time, collector.stats());

    vout << "Estimated memory usage:\n";
    vout << "  location index: " << (location_index->used_memory() / 1024) << "kB\n";

    osmium::MemoryUsage mcheck;
    vout << "Actual memory usage:\n"
         << "  current: " << mcheck.current() << "MB\n"
         << "  peak:    " << mcheck.peak() << "MB\n";

    vout << "Done.\n";

    return 0;
}

