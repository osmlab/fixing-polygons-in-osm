cd /data/osm2pgsql
# this statefile isn't for minutely diffs, but copy it anyways to have a record
cp /data/robot/planet/osmosis-status/state.txt /srv/replication/state-orig.txt
# takes ~12h
osm2pgsql_oldpoly --ignore-oldstyle-polygons -d osm2pgsql_noold \
  -S /srv/openstreetmap-carto/openstreetmap-carto.style --slim \
  --flat-nodes /data/osm2pgsql/noold.nodes --cache 34000 /data/robot/planet/current-planet.osm.pbf
