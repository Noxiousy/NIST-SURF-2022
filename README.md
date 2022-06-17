# NIST SURF 2022: Updating ZENO Queries

Author: Jaxon Ko

SURF Mentor: Derek Juba

---

For this 2022 SURF project, the queries used throughout the ZENO program will be updated
with the FCPW C++ library. ZENO is a program that creates spatial data structures mimicing
molecular structures. This data structure can be used to compute material, solution, and 
suspension properties of specific shapes and molecules. For more information about how ZENO
works (along with source code), see [this website](https://zeno.nist.gov/). The "Fastest Closest Point In The West"
(FCPW) library creates its own spatial index, namely `Scene`, with geometric shapes and provides
efficient queries. The ZENO program will utilize the `findClosestPoint` and `contains` queries.

# Command Line Arguments

This repository contains source code (`../FastestClosestPoint/fcpw.cpp`) that will read an 
`.obj` file and create a `Scene` with the shapes listed in the file. After the spatial data 
structure has been formed, users can specify query details using command line arguments.

## Random Queries

To run a random number of queries, specify `true` for the fourth command line argument as follows:

> `./fcpw.exe [.obj file] [query] true [number of trials]`

`[.obj file]` is the file path of the .obj file from this directory.
`[query]` is the query type (either "contains" or "closest_point").
`[number of trials]` is the number of random queries that will be benchmarked.

## Non-random Query

To use a specific query point, specify `false` for the fourth command line argument as follows:

> `./fcpw.exe [.obj file] [query] false [x] [y] [z]`

`[.obj file]` is the file path of the .obj file from this directory.
`[query]` is the query type (either "contains" or "closest_point").
The remaining arguments specify the coordinates of the query point. For 3 dimensions, this would be
`[x]`, `[y]`, and `[z]`. **Please note that this code assumes 3D spaces, so at least 3 `float` arguments must be specified.**

---

# FCPW

[The FCPW library](https://github.com/rohan-sawhney/fcpw) efficiently runs queries of 3D shapes.
The two queries that will be utilized by the ZENO program are the `contains` query and the 
`findClosestPoint` query.

##  Contains

After constructing a scene, the following function call can be used to run a contains query:

```C++
scene.contains(queryPoint);
```

Note that `queryPoint` is of type `fcpw::Vector<3>`. These vectors represent coordinates in 3D space. For
scenes with multiple geometric shapes, the scene data can be accessed and dereferenced to run contains queries:

```C++
SceneData<3> *data = scene.getSceneData();
data -> aggregateInstancePtrs[i] -> contains(queryPoint);
```

Note that `i` is the index of the geometric shape that will be queried. The former function call will make `i` queries
for each of the pointers, while the latter will return the result of a single pointer.

##  Closest Point

Using the same process as the `contains` query, we can make `findClosestPoint` function calls:

```C++
Interaction<3> interaction;
scene.findClosestPoint(queryPoint, interaction);
```

Since the result of a closest point query is binary (not just true or false), we pass the function an 
`Interaction`. After the function call, `interaction` will store information about the query, such as
the closest point from `queryPoint`, the distance between said point and `queryPoint`, as well as the
object and primitive indices of the closest point. 

```C++
interaction.d;   //  distance (integer)
interaction.p;   //  closest point (fcpw::Vector<3>)
interaction.primitiveIndex;
interaction.objectIndex;
```

# Benchmarking FCPW Queries

To help boost the efficiency of the ZENO program, the runtime of `contains` and `findClosestPoint` queries
will be benchmarked. 

## Results

*I am working to create a spherical bounding box to mimic the ZENO closest point query as best as possible.
When this has been completed, the benchmarked results will be listed here*
