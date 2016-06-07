
#include <iostream>
#include <getopt.h>

#include <gdalcpp.hpp>

#include <osmium/index/map/all.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/visitor.hpp>

#include <osmium/geom/mercator_projection.hpp>
#include <osmium/geom/ogr.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>

typedef osmium::index::map::Map<osmium::unsigned_object_id_type, osmium::Location> index_pos_type;
typedef osmium::handler::NodeLocationsForWays<index_pos_type> location_handler_type;

class MyOGRHandler : public osmium::handler::Handler {

    osmium::geom::OGRFactory<osmium::geom::MercatorProjection>& m_factory;
    gdalcpp::Layer m_layer_linestring;

public:

    MyOGRHandler(gdalcpp::Dataset& dataset, osmium::geom::OGRFactory<osmium::geom::MercatorProjection>& factory) :
        m_factory(factory),
        m_layer_linestring(dataset, "lines", wkbLineString) {
        m_layer_linestring.add_field("way_id", OFTInteger, 10);
    }

    void way(const osmium::Way& way) {
        try {
            gdalcpp::Feature feature(m_layer_linestring, m_factory.create_linestring(way));
            feature.set_field("way_id", int(way.id()));
            feature.add_to_layer();
        } catch (...) { //osmium::geometry_error&) {
            std::cerr << "Ignoring illegal geometry for way " << way.id() << ".\n";
        }
    }

};

/* ================================================== */

void print_help() {
    std::cout << "area_ways [OPTIONS] [INFILE [OUTFILE]]\n\n" \
              << "If INFILE is not given stdin is assumed.\n" \
              << "If OUTFILE is not given 'area_ways.shp' is used.\n" \
              << "\nOptions:\n" \
              << "  -h, --help                 This help message\n" \
              << "  -l, --location_store=TYPE  Set location store\n" \
              << "  -f, --format=FORMAT        Output OGR format (Default: 'ESRI Shapefile')\n" \
              << "  -L                         See available location stores\n";
}

int main(int argc, char* argv[]) {
    const auto& map_factory = osmium::index::MapFactory<osmium::unsigned_object_id_type, osmium::Location>::instance();

    static struct option long_options[] = {
        {"help",                 no_argument, 0, 'h'},
        {"format",               required_argument, 0, 'f'},
        {"location_store",       required_argument, 0, 'l'},
        {"list_location_stores", no_argument, 0, 'L'},
        {0, 0, 0, 0}
    };

    std::string output_format { "ESRI Shapefile" };
    std::string location_store { "sparse_mmap_array" };

    while (true) {
        int c = getopt_long(argc, argv, "hf:l:L", long_options, 0);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                print_help();
                exit(0);
            case 'f':
                output_format = optarg;
                break;
            case 'l':
                location_store = optarg;
                break;
            case 'L':
                std::cout << "Available map types:\n";
                for (const auto& map_type : map_factory.map_types()) {
                    std::cout << "  " << map_type << "\n";
                }
                exit(0);
            default:
                exit(1);
        }
    }

    std::string input_filename;
    std::string output_filename("area_ways.shp");
    int remaining_args = argc - optind;
    if (remaining_args > 2) {
        std::cerr << "Usage: " << argv[0] << " [OPTIONS] [INFILE [OUTFILE]]" << std::endl;
        exit(1);
    } else if (remaining_args == 2) {
        input_filename =  argv[optind];
        output_filename = argv[optind+1];
    } else if (remaining_args == 1) {
        input_filename =  argv[optind];
    } else {
        input_filename = "-";
    }

    osmium::io::Reader reader(input_filename);

    std::unique_ptr<index_pos_type> index_pos = map_factory.create_map(location_store);
    location_handler_type location_handler(*index_pos);
    location_handler.ignore_errors();

    osmium::geom::OGRFactory<osmium::geom::MercatorProjection> factory;

    gdalcpp::Dataset dataset{output_format, output_filename, gdalcpp::SRS{factory.proj_string()}};
    MyOGRHandler ogr_handler(dataset, factory);

    osmium::apply(reader, location_handler, ogr_handler);
    reader.close();
}

