
# Frequently Asked Questions (FAQ)

## What exactly is the problem?

There are really two problems here. First there are lots multipolygon relations
tagged *old style* with the tags on the outer ways. And then there is the
problem of broken data leading to invalid geometries, polygons with
self-intersections for instance.

Read the [background documentation](background.md) for all the details.


## Are we talking about *polygons* here or *multipolygons*

We are always talking about both. A polygon is just a special form of a
multipolygon with just one outer ring.

Read the [background documentation](background.md) for all the details.


## Are we talking about areas created from closed ways, too, or just multipolgyon relations?

Both. Only multipolygon relations have the problem with the *old style* tagging,
but the problems with invalid geometries also affect closed ways.


## What are *old style* multipolygon relations?

[Multipolygon
relations](http://wiki.openstreetmap.org/wiki/Relation:multipolygon) can be
tagged in two ways. The preferred and most common way is to put the tags for
the (multi)polygon on the relation. The *old style* puts tags on the outer
way(s) instead.


## What about boundary relations?

Relations tagged `type=boundary` typically used for administrative boundaries
behave in many aspects like multipolygon relations. (Because, after all, they
define the area of a city or country or so, which is a (multi)polygon.) So we
need to have a look at these, too.


## What are we doing about it?

This is a large effort and the contributions of many people are neede to do
all this. Here are some things we are doing or planing to do:

* Write documentation that explain how to best tag (multi)polygons.
* Write new tools and enhance existing tools to detect problematic
  (multi)polygons and bring this information to the mapper.
* Create statistics to track progress.
* Actually fix the (multi)polygons manually.
* Maybe write software to fix (some of) the polygons automatically.


## Who is doing all this?

You are. Well, we hope you will be helping. This is a larger effort that needs
everybodys help. We need mappers to actually fix the data. But before they can
do that, we need to create new tools or extend existing ones that help find the
problematic cases and help fixing them. We can also use support from editor
developers. If you think this is a worthwile project, get involved.


## How long is this all going to take?

We don't know that yet. Currently we are at the beginning of this effort and
there is a lot to do. We are certainly talking about months here.


## Will we ever finish this effort?

Of course there will never be a time, when OSM data including all polygons is
perfect. Our goal is to get rid of the *old style* tagging completely and
reduce the number of geometrically invalid (multi)polygons so far, that the
map will look okay, even if geometrically invalid (multi)polygons are ignored.
This goal is, of course, somewhat imprecise. Over the course of the effort
we'll probably get to a better understanding of the details here.


## I have some software that handles multipolygons. What do I have to do?

The short answer is: nothing. If your software works today, it will continue to
work.

The slightly longer answer is: You can check your software and see whether it
can be simplified. Chances are that you can make your software simpler and
faster. Of course you might want to wait a bit, until more (multi)polygons are
fixed.


## I have some questions, where can I go?

See the [Contact section in the README](../README.md#contact).

