#include <iostream>
#include <ctime>
#include <vector>
#include <tuple>
#include <cfloat>

#include "bounding_sphere.h"
#include "read_obj.h"
#include "libzeno/AABBTree.h"
#include "libzeno/Vector3.h"
#include "libzeno/Triangle.h"

using namespace std;
using namespace zeno;

int main(int argc, char** argv)
{
	// read vertices and faces from .obj file
	vector<Vector3<double>> vertices;
	vector<int> indices;
	
	parse(vertices, indices, argv[1]);

	// begin preprocessing timer
	clock_t t = clock();

	// store Triangles
	vector<Triangle<double>> triangles;

	for (int i = 0; i < indices.size() / 3; i += 3)
	{
		Triangle<double> t(vertices[indices[i]],
				   vertices[indices[i + 1]],
				   vertices[indices[i + 2]]);

		triangles.push_back(t);
	}

	// initialize scene
	AABBTree<Triangle<double>> scene;

	scene.preprocess(&triangles);
	cout << "Preprocessing time: " << (float) (clock() - t) / CLOCKS_PER_SEC << " seconds." << endl;

	// create bounding sphere
	tuple<Vector3<double>, float> sphere;
	getBoundingSphere(vertices, sphere);

	// query variables
	bool randomization = (string(argv[3]) == "true" ? true : false);
	int seed = stoi(argv[4]);
	string query = argv[2];

	const Triangle<double> *nearestObject;
	double sqDistance = DBL_MAX;

	if (randomization)
	{
		// run n random queries
		int n = stoi(argv[5]);

		if (query == "contains")
		{
			t = clock();

			for (int i = 0; i < n; i++)
				scene.objectsContain(getRandomPoint(sphere, seed));

			t = clock() - t;
		}
		else if (query == "closest_point")
		{
			t = clock();

			for (int i = 0; i < n; i++)
				scene.findNearestObject(getRandomPoint(sphere, seed), &nearestObject, &sqDistance);

			t = clock() - t;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// benchmark
		cout << "Benchmark: " << n << " \"" << query << "\" queries in " << (float) t / CLOCKS_PER_SEC << " seconds." << endl;

		cout << "Total vertices: " << vertices.size() << endl;
	}
	else
	{
		Vector3<double> queryPoint(stod(argv[5]), stod(argv[6]), stod(argv[7]));

		if (query == "contains")
		{
			t = clock();

			bool result = scene.objectsContain(queryPoint);

			t = clock() - t;

			cout << "Contains query result:\n" << result << endl;
		}	
		else if (query == "closest_point")
		{
			t = clock();

			scene.findNearestObject(queryPoint, &nearestObject, &sqDistance);

			t = clock() - t;

			cout << "Closest Point results:\nQuery Point: " << queryPoint << "\nNearest Object: " << *nearestObject << "\nSquared Distance: " << sqDistance << endl;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// benchmark
		cout << "Benchmark: 1 \"" << query << " query in " << (float) t / CLOCKS_PER_SEC << " seconds." << endl;
	}

	return 0;
}
