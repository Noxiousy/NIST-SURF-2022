/*
 * fcpw.cpp now supports .obj file reading to construct geometric shapes
 */

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <fcpw/fcpw.h>

// my own header file
#include "read_obj.h"

using namespace std;
using namespace fcpw;

/*
 * command line is as follows
 *
 * Randomized queries:
 * ./fcpw.exe [.obj file path] [query type (contains or closest_point)] true [# of queries]
 *
 * Single query
 * ./fcpw.exe [.obj file path] [query type] false [x] [y] [z]
 *
 */
int main(int argc, char *argv[])
{
	// ==== READING THE .obj FILE ====
	

	// list of integers to store positions
	vector<vector<float>> vertices;
	vector<int> indices;

	// parse .obj file for vertices and triangle indices
	parse(vertices, indices, argv[1]);


	// a scene is a spatial index for the shapes and query.
	const int DIM = 3;
	Scene<DIM> scene;

	const int nVertices = vertices.size(), nTriangles = indices.size() / 3;

	// create a scene with one object containing triangles
	scene.setObjectTypes({{PrimitiveType::Triangle}});

	// set the vertex and the triangle count of the 0th object (only one object type defined above)
	scene.setObjectVertexCount(nVertices, 0);
	scene.setObjectTriangleCount(nTriangles, 0);

	// set the positions of each vertex within the scene by passing each row of the matrix
	for (int i = 0; i < nVertices; i++)
	{
		// create a Vector3 with the i'th row's values in vertices
		Vector3 vertex(vertices[i][0], vertices[i][1], vertices[i][2]);

		// set the object vertex in the scene data
		scene.setObjectVertex(vertex, i, 0);
	}

	// set the indices of each vertices for each traingle
	for (int i = 0; i < nTriangles; i++)
		scene.setObjectTriangle(&indices[3 * i], i, 0);

	// build the scene (true allows vectorization)
	scene.build(AggregateType::Bvh_SurfaceArea, true);


	/* ==== QUERIES ==== */

	
	// variables
	bool randomization = (string(argv[3]) == "true" ? true : false);
	string query = argv[2];
	clock_t t;

	// determine if randomization is used
	if (randomization)
	{
		// random queries
		int n = stoi(argv[4]);

		// check query type
		if (query == "contains")
		{
			// begin timer
			t = clock();

			// run n number of trails
			for (int i = 0; i < n; i++)
				scene.contains({(float) rand(), (float) rand(), (float) rand()});

			// stop timer
			t = clock() - t;
		}
		else if (query == "closest_point")
		{
			Interaction<3> interaction;

			// begin timer
			t = clock();

			// run n number of trials
			for (int i = 0; i < n; i++)
				scene.findClosestPoint({(float) rand(), (float) rand(), (float) rand()}, interaction);

			// stop timer
			t = clock() - t;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// print benchmark
		cout << "Benchmark:\n" << n << " \"" << query << "\" queries in " << (float) t / CLOCKS_PER_SEC << " seconds." << endl;
	}
	else 
	{
		// non-random query
		Vector3 queryPoint(stoi(argv[4]), stof(argv[5]), stof(argv[6]));

		// check query type
		if (query == "contains")
		{
			// begin timer
			t = clock();

			// query
			scene.contains(queryPoint);

			// stop timer
			t = clock() - t;

			// results
			cout << "Query result:\n" << (bool) scene.contains(queryPoint) << endl;
		}
		else if (query == "closest_point")
		{
			Interaction<3> interaction;

			// begin timer
			t = clock();

			// query
			scene.findClosestPoint(queryPoint, interaction);

			// stop timer
			t = clock() - t;

			// results
			cout << "Query results:\nClosest Point: (" << interaction.p[0] << ", " << interaction.p[1] << ", " 
				<< interaction.p[2] << ")\nDistance Between Query Point & Closest Point: " << interaction.d << endl;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// print benchmark
		cout << endl << "Benchmark:\n1 \"" << query << "\" query in " << (float) t / CLOCKS_PER_SEC << " seconds." << endl;
	}

	return 0;
}
