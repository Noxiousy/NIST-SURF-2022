#include <iostream>
#include <ctime>
#include <vector>

#include "libzeno/AABBTree.h"
#include "libzeno/Vector3.h"
#include "libzeno/Triangle.h"
#include "libzeno/Cuboid.h"

using namespace std;
using namespace zeno;

int main(int argc, char** argv)
{
	Vector3<double> a(0.0, 0.0, 0.0);
	Vector3<double> b(0.0, 5.0, 0.0);
       	Vector3<double> c(5.0, 5.0, 0.0);

	Triangle<double> t(a, b, c);	

	vector<Triangle<double>> triangles;
	triangles.push_back(t);
	
	t.preprocess();

	cout << t.contains({1.0, 1.0, 0.0}) << endl;

	AABBTree<Triangle<double>> scene;

	scene.preprocess(&triangles);

	// closest point query
	const Triangle<double> **nearestObject;
	const Vector3<double> queryPoint(6.0, 0.0, 0.0);
	double sqDistance;

	scene.findNearestObject(queryPoint, nearestObject, &sqDistance);

	// returns a close-to-zero number?
	cout << sqDistance << endl;

	return 0;
}
