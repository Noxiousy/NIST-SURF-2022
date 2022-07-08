#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <math.h>
#include <time.h>
#include <fstream>

#include "fcpw/fcpw.h"
#include "read_obj.h"
#include "bounding_sphere.h"

using namespace std;
using namespace fcpw;

// ./fcpw.exe [obj_file] [queryType] true [# of queries] [seed] [results?]
// ./fcpw.exe [obj_file] [queryType] false [x] [y] [z] [results?]
int main(int argc, char** argv)
{
	// list of integers to store positions
	vector<Vector3> vertices;
	vector<int> indices;

	// parse .obj file for vertices and triangle indices
	parse(vertices, indices, argv[1]);
	
	// begin preprocessing timer
	clock_t t = clock();

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

	scene.build(AggregateType::Bvh_Volume, true);
	float preprocessingTime = (float) (clock() - t) / CLOCKS_PER_SEC;
	
	// create a bounding sphere around the geometry
	tuple<Vector3, float> sphere;
	getBoundingSphere(vertices, sphere);


	/* ==== QUERIES ==== */

	
	// variables
	bool randomization = (string(argv[3]) == "true" ? true : false), saveResults = (string(argv[argc - 1]) == "true" ? true : false);
	float benchmarkTime = 0.0;
	string query = argv[2];
	
	ofstream outfile("results.txt");

	// create results file if necessary
	if (saveResults)
	{
		for (int i = 0; i < argc; i++)
		{
			if (i != argc - 1)
				outfile << argv[i] << " ";
			else
				outfile << argv[i] << endl;
		}	

	}

	// determine if randomization is used
	if (randomization)
	{
		// random queries
		int seed = stoi(argv[5]), n = stoi(argv[4]);

		srand(seed);

		// check query type
		if (query == "contains")
		{
			// run n number of trails
			for (int i = 0; i < n; i++)
			{
				// timer
				t = clock();

				// contains query on random point
				Vector3 vertex = getRandomPoint(sphere);
				bool result = scene.contains(vertex);

				// sum time
				benchmarkTime += (float) (clock() - t) / CLOCKS_PER_SEC;

				// save result if specified
				if (saveResults)
					outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): " << (result ? "true" : "false") << endl;
			}
		}
		else if (query == "closest_point")
		{
			Interaction<3> interaction;

			// run n number of trials
			for (int i = 0; i < n; i++)
			{
				// timer
				t = clock();

				// closest point query on random point
				Vector3 vertex = getRandomPoint(sphere);
				scene.findClosestPoint(vertex, interaction);

				// sum time 
				benchmarkTime += (float) (clock() - t) / CLOCKS_PER_SEC;

				// save result if specified
				if (saveResults)
					outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): Distance (" << interaction.d
						<< "), Closest Point (" << interaction.p[0] << ", " << interaction.p[1] << ", " << interaction.p[2] << ")" << endl;
			}
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// console benchmark output
		cout << n << " FCPW " << query << " queries: Preprocessing Time (" << preprocessingTime 
			<< "s), Benchmark (" << benchmarkTime << "s), Vertices (" << vertices.size() << ")" << endl;
	}
	else 
	{
		// non-random query
		Vector3 vertex(stoi(argv[4]), stof(argv[5]), stof(argv[6]));

		// check query type
		if (query == "contains")
		{
			// begin timer
			t = clock();

			// query
			bool result = scene.contains(vertex);

			// stop timer
			benchmarkTime = (float) (clock() - t) / CLOCKS_PER_SEC;

			if (saveResults)
				outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): " << (result ? "true" : "false") << endl;
		}
		else if (query == "closest_point")
		{
			Interaction<3> interaction;

			// begin timer
			t = clock();

			// query
			scene.findClosestPoint(vertex, interaction);

			// stop timer
			benchmarkTime = (float) (clock() - t) / CLOCKS_PER_SEC;


			if (saveResults)
				outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): Distance (" << interaction.d	
					<< "), Closest Point ("	<< interaction.p[0] << ", " << interaction.p[1] << ", " << interaction.p[2] << ")" << endl;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// print benchmark
		cout << "FCPW " << query << " query: Preprocessing Time (" << preprocessingTime
			<< "s), Benchmark (" << benchmarkTime << "s)" << endl;
	}	

	if (saveResults)
		cout << "Results saved to results.txt" << endl;

	outfile.close();

	return 0;
}
