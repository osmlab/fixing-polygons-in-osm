
# Fixing Polygons in OSM

There are many different ways (multi)polygons in OpenStreetMap can be mapped
correctly and there are even more ways in which they can be mapped incorrectly.
Of the over 220 million (multi)polygons in OSM more than 100,000 contain
mapping errors of one kind or another and about 250,000 are tagged *old style*
with tags on the outer ways instead of on the relation making multipolygon
tagging and processing much more complicated and much more expensive than it
needs to be.

We are trying to fix all this and this repository is here to organize this
effort.

Note that this is **not** about inventing some [new way of representing
(multi)polygons in
OSM](https://wiki.openstreetmap.org/wiki/Area/The_Future_of_Areas). This is
about fixing wrong tagging and simplifying the current way of tagging. There
are no changes needed or planned to the core of OSM.

This will make **mapping easier**, because there will be only one canonical way
of tagging multipolygons. Editors can take this into account and help mappers
in better ways. And it will make **using the data easier and faster** because
the programs (such as Osm2pgsql and Osmium) converting OSM data into
multipolygons for rendering or other uses can be simplified. It will also make
it easier to detect mapping errors raising the **quality of the OSM data**.

## Database and extracts

* http://area.jochentopf.com/ - see "Downloads" section
 
## Stats 

* http://area.jochentopf.com/stats/

## More Information

* [Background on (multi)polygons](doc/background.md)
* [Typical problems with OSM multipolygons](doc/problems.md)
* [Frequently Asked Questions (FAQ)](doc/faq.md)
* [Some notes about rendering](doc/rendering.md)


## QA Tools

* [OSM Inspector](http://tools.geofabrik.de/osmi/?view=areas) - updated daily
* [Map](http://area.jochentopf.com/map/index.html#3/30.00/0.00) - updated continuously


## Contact

If you have any questions or want to contribute in any way, here are some ways
to get involved:

* [Open an issue](https://github.com/osmlab/fixing-polygons-in-osm/issues) on
  our github repository.
* Discuss on the
  [OSM developer mailing list](https://lists.openstreetmap.org/listinfo/dev).
* Join us on the `#osm-dev` IRC channel on OFTC ([how to get
  there](http://wiki.openstreetmap.org/wiki/IRC)).


