# NIST SURF 2022: Benchmarking Queries with Zeno and FCPW

Author: Jaxon Ko

SURF Mentor: Derek Juba

## TL;DR

On behalf of the NIST Summer Undergraduate Research Fellowship (SURF), I've worked to compare two C++ Bounding Volume
Hierarchy libraries. The more optimized library will be used in the next version of Zeno, a NIST program that estimates
material properties based on the particles of a material. Currently, Zeno computes these properties using an 
internal library. The library will be compared to an open-source one known as The Fastest Closest Points in the 
West (FCPW).

Our research has shown that FCPW is the more efficient library. However, these results were obtained with limitations to
both libraries in order to bear similarities in the process (see `benchmark/README.md` section for more details).

---

# API

The C++ programs, `FastestClosestPoints/fcpw.cpp` and `ZenoClosestPoints/query.cpp`, were used to test both the FCPW and internally-developed Zeno libraries, respectively. CMake files are included in each directory to create the executables for each program.

## Command Line Arguments

To run each executable, use the following arguments:

```
./fcpw.exe [.obj file] [queryType] [# of trials] [seed] [results?]
```

- `[.obj file]` is file location to an object file rendering the geometric model of the particle.
- `[queryType]` is the type of query (either "contains" or "closest_point" exactly).
- `[# of trials]` is the number of random queries to be run.
- `[seed]` is the seed used for pseudorandomness.
- `[results]` is a boolean used to save the results of each random query (either "true" or "false" exactly)

**Note that the `./fcpw.exe` command can be switched for `./zeno.exe` to test the Zeno program.**

The purpose of these programs to the time how long it takes for each program to construct its geometric model (preprocessing time) as well as
compute its queries (query time). These times will be printed to the console, and will be analyzed by other scripts.

---
