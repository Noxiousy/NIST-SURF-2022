// a custom implementation of Ritter's bounding sphere algorithm

#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>

#include "libzeno/Vector3.h"

using namespace std;
using namespace zeno;

/* ==== HELPER FUNCTIONS ==== */


// return the distance between two points a & b
float distance(Vector3<double> a, Vector3<double> b)
{
	return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
}

// return whether or not a vertex is contained within a sphere
bool contains(tuple<Vector3<double>, float> sphere, Vector3<double> vertex)
{
	return distance(get<0>(sphere), vertex) < get<1>(sphere);
}

// return a random 3D point within a sphere (utilized by fcpw.cpp)
Vector3<double> getRandomPoint(tuple<Vector3<double>, float> sphere)
{	
	// randomly generate two variables between [0, 1]
	double u = (double) rand() / RAND_MAX, v = (double) rand() / RAND_MAX;
	
	// compute the latitude and longitude in radians
        double lambda = acos(2 * u - 1) - (M_PI / 2), phi = 2 * M_PI * v;
        
	// using lambda and phi to generate the random rectangular coordinates
	double x = cos(lambda) * cos(phi) * get<1>(sphere) + get<0>(sphere)[0];
	double y = cos(lambda) * sin(phi) * get<1>(sphere) + get<0>(sphere)[1];
	double z = sin(lambda) * get<1>(sphere) + get<0>(sphere)[2];

	return {x, y, z};
}


/* ==== BOUDNING SPHERE FUCNTION ==== */


// return a tuple containing the center of the sphere and the radius
void getBoundingSphere(vector<Vector3<double>> vertices, tuple<Vector3<double>, float> &sphere)
{
	// choose a point x
	Vector3<double> x(vertices[0]);

	// find a point y that's furthest from x
	int d = 0;
	Vector3<double> y;
	for (Vector3<double> vertex : vertices)
	{
		if (distance(x, vertex) > d)
		{
			d = distance(x, vertex);
			y = vertex;
		}
	}

	// find a point z that's furthest from y
	d = 0;
	Vector3<double> z;
	for (Vector3<double> vertex : vertices)
	{
		if (distance(y, vertex) > d)
		{
			d = distance(y, vertex);
			z = vertex;
		}
	}

	// the initial sphere is centered at the middle point of y and z
	Vector3<double> center = {(y[0] + z[0]) / 2, (y[1] + z[1]) / 2, (y[2] + z[2]) / 2};

	// the radius is half of the distance between y and z
	sphere = make_tuple(center, d / 2);

	// check if all vertices are within initial sphere
	for (Vector3<double> vertex : vertices)
	{
		// expand radius to include vertex if not already contained within the sphere
		if (!contains(sphere, vertex))
			get<1>(sphere) = distance(get<0>(sphere), vertex);
	}

	// by now, sphere is a tuple containing the center vertex and radius of the bounding sphere
	// containing all the vertices listed in the parameter
}
