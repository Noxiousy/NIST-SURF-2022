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

	// create bounding sphere
	tuple<Vector3<double>, float> sphere;
	getBoundingSphere(vertices, sphere);

	// query variables
	bool randomization = (string(argv[3]) == "true" ? true : false);
	string query = argv[2];
	clock_t timer;

	const Triangle<double> *nearestObject;
	double sqDistance = DBL_MAX;

	if (randomization)
	{
		// run n random queries
		int n = stoi(argv[4]);

		if (query == "contains")
		{
			timer = clock();

			for (int i = 0; i < n; i++)
				scene.objectsContain(getRandomPoint(sphere));

			timer = clock() - timer;
		}
		else if (query == "closest_point")
		{
			timer = clock();

			for (int i = 0; i < n; i++)
				scene.findNearestObject(getRandomPoint(sphere), &nearestObject, &sqDistance);

			timer = clock() - timer;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// benchmark
		cout << "Benchmark:\n" << n << " \"" << query << "\" queries in " << (float) timer / CLOCKS_PER_SEC << " seconds." << endl;

		cout << "Total vertices: " << vertices.size() << endl;
	}
	else
	{
		Vector3<double> queryPoint(stod(argv[4]), stod(argv[5]), stod(argv[6]));

		if (query == "contains")
		{
			timer = clock();

			bool result = scene.objectsContain(queryPoint);

			timer = clock() - timer;

			cout << "Contains query result:\n" << result << endl;
		}	
		else if (query == "closest_point")
		{
			timer = clock();

			scene.findNearestObject(queryPoint, &nearestObject, &sqDistance);

			timer = clock() - timer;

			cout << "Closest Point results:\nQuery Point: " << queryPoint << "\nNearest Object: " << *nearestObject << "\nSquared Distance: " << sqDistance << endl;
		}
		else
		{
			cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
			return 0;
		}

		// benchmark
		cout << "Benchmark:\n1 \"" << query << " query in " << (float) timer / CLOCKS_PER_SEC << " seconds." << endl;
	}

	return 0;
}
