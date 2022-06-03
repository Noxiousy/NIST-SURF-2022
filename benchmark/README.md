# Benchmarking FCPW Queries

To help boost the efficiency of the ZENO program, we will benchmark the runtimes of `contains` and `findClosestPoint` queries from the `fcpw` library. The file `benchmark_fcpw.cpp` creates a scene with a cube centered about the origin. The cube has a length/width/height of 40 units. For more information about how the `fcpw` library works (including information about `Scene` and specific queries), see `../FastestClosestPoint/fcpw.cpp`.

## TL;DR - Results

The `contains` query averaged a runtime of *1.35 seconds per one million queries*. The `findClosestPoint` query averaged a runtime of *1.95 seconds per one million*. Randomization (or lack thereof) on each query point had no effect on either runtime.

# API

`CMakeLists.txt` is provided to build the source code. After the executable has been built, command-line arguments are used to create benchmark tests. Use the following format for each executable call:

`./fcpw.exe [# of queries] [query] [random?]`

`# of queries` is the number of queries that run and timed, `query` is the query type. This must either be **"contains"** or **"closest_point"** respectively. `random?` determines whether or not random query points will be tested for each trial. **"true"** will use random generation, while **"false"** (or anything other than "true") will used a fixed query point.

As an example, a benchmark test of one million contains queries with randomization can be called using the following command:

`./fcpw.exe 1000000 contains true`
