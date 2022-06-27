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
	Vector3<double> b(7.0, 0.0, 0.0);
       	Vector3<double> c(0.0, 7.0, 0.0);
	Vector3<double> d(0.0, 0.0, 7.0);

	Triangle<double> v1(a, b, c);	
	Triangle<double> v2(a, b, d);
	Triangle<double> v3(a, c, d);
	Triangle<double> v4(b, c, d);

	vector<Triangle<double>> triangles;
	triangles.push_back(v1);
	triangles.push_back(v2);
	triangles.push_back(v3);
	triangles.push_back(v4);

	AABBTree<Triangle<double>> scene;

	scene.preprocess(&triangles);

	// closest point query
	const Triangle<double> *nearestObject;
	const Vector3<double> queryPoint(0.0, 0.0, 10.0);
	double sqDistance;

	scene.findNearestObject(queryPoint, &nearestObject, &sqDistance);

	cout << sqDistance << endl;
	cout << *nearestObject << endl;

	return 0;
}
