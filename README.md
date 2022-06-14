
**Please note that the information in this README.md is not correct. I will work to combine the `fcpw.cpp`, 
`benchmark.cpp`, and `overlap.cpp` files into one file that can be used to run the `FCPW` library queries. 
In doing so, this README.md will be updated with more accurate information...**

---

# 2022 SURF Zeno

2022 SURF project on Zeno spatial data structures.

This repository contains example programs utilizing the FCPW C++ library. Specifically,
it showcases how to create a 3D spatial index (called a Scene) with geometric shapes 
and how to query those shapes/vertices. `FastestClosestPoint/fcpw.cpp` contains "Closest 
Point" and "Contains" queries. 

All of the source code in this repository utilizes CMake, which also depends on the FCPW directory.

# FCPW

[The library](https://github.com/rohan-sawhney/fcpw) efficiently runs queries of 2D and 3D shapes.
The two queries that will be utilized by the ZENO program are the `contains` query and the `findClosestPoint`
query.

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
# Runtime

See `../benchmark/README.md` for more information about the runtime and efficiencies of both
queries.
