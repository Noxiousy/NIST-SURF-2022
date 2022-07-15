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

	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle<double> triangle(vertices[indices[i]],
				   	  vertices[indices[i + 1]],
				   	  vertices[indices[i + 2]]);

		triangles.push_back(triangle);
	}

	// initialize scene
	AABBTree<Triangle<double>> scene;

	scene.preprocess(&triangles);
	float preprocessingTime = (float) (clock() - t) / CLOCKS_PER_SEC;

	// create bounding sphere
	tuple<Vector3<double>, float> sphere;
	getBoundingSphere(vertices, sphere);

	// query variables
	bool saveResults = (string(argv[argc - 1]) == "true" ? true : false);
	float benchmarkTime = 0.0;
	string query = argv[2];
	
	ofstream outfile("results.txt");
	if (saveResults)
	{
		for (int i = 0; i < argc; i++)
		{
			if (i == argc - 1)
				outfile << argv[i] << endl;
			else
				outfile << argv[i] << " ";
		}	
	}

	// run n random queries
	int n = stoi(argv[3]);
	srand(stoi(argv[4]));

	// only working with closest point queries now
	if (false)
	{
		for (int i = 0; i < n; i++)
		{
			t = clock();

			Vector3<double> vertex = getRandomPoint(sphere);
			bool result = scene.objectsContain(vertex);

			benchmarkTime += (float) (clock() - t) / CLOCKS_PER_SEC;

			if (saveResults)
				outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): " << (result ? "(true)" : "(false)") << endl;
		}
	}
	else if (query == "closest_point")
	{
		for (int i = 0; i < n; i++)
		{
			double sqDistance = DBL_MAX;
			const Triangle<double> *nearestObject;

			t = clock();
			
			Vector3<double> vertex = getRandomPoint(sphere);
			scene.findNearestObject(vertex, &nearestObject, &sqDistance);
			
			benchmarkTime += (float) (clock() - t) / CLOCKS_PER_SEC;
			
			if (saveResults)
				outfile << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): Distance (" << sqrt(sqDistance) << ")" << endl;
		}	
	}
	else
	{
		cout << "Please enter \"contains\" or \"closest_point\" for the query argument..." << endl;
		return 0;
	}
	
	// benchmark
	cout << fixed << n << " ZENO " << query << " queries: Preprocessing Time (" << preprocessingTime 
		<< "s), Benchmark (" << benchmarkTime << "s), Vertices (" << vertices.size() << ")" << endl;

	if (saveResults)
		cout << "Results saved to results.txt" << endl;

	outfile.close();

	return 0;
}
