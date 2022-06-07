/*
	This program has an example contains query with two overlapping cubes.
	The goal of this program is to show that the contains query under the fcpw
	library can be used to prove if two 3D shapes are overlapping in space.
*/

#include <iostream>
#include <fcpw/fcpw.h>

using namespace std;
using namespace fcpw;

int main()
{
	// creating a 3D scene with 2 overlapping cubes
	Scene<3> scene;

	scene.setObjectTypes({{PrimitiveType::Triangle}});

	const int nVertices = 16, nTriangles = 24;
	scene.setObjectVertexCount(nVertices, 0);
	scene.setObjectTriangleCount(nTriangles, 0);

	Eigen::Matrix<float, nVertices / 2, 3> pos1, pos2;

	// cube 1 (width of 10, has a corner at the origin)
	pos1 <<	0, 0, 0,
	     	10, 0, 0,
		0, 10, 0,
		10, 10, 0,
		0, 0, 10,
		10, 0, 10,
		0, 10, 10,
		10, 10, 10;

	// cube 2 (width of 10, has a corner at the center of cube 1)
	pos2 << 5, 5, 5,
	     	15, 5, 5, 
		5, 15, 5,
		15, 15, 5,
		5, 5, 15,
		15, 5, 15, 
		5, 15, 15,
		15, 15, 15;

	// respective indices of vertices for triangles in both cubes
	int indices[] = {	0, 1, 2,
				0, 1, 3, 
				0, 1, 4,
				0, 1, 5,
				0, 2, 4, 
				0, 2, 6,
				1, 3, 7,
				1, 5, 7,
				2, 3, 7,
				2, 6, 7,
				4, 5, 6,
				5, 6, 7	};

	// add vertices and indices to the scene
	for (int i = 0; i < nVertices / 2; i++)
	{
		scene.setObjectVertex(pos1.row(i), i, 0);
		scene.setObjectVertex(pos2.row(i), i + nVertices / 2, 0);
	}

	for (int i = 0; i < nTriangles / 2; i++)
	{
		scene.setObjectTriangle(&indices[3 * i], i, 0);
		scene.setObjectTriangle(&indices[3 * i], i + nTriangles / 2, 0);
	}

	scene.build(AggregateType::Bvh_OverlapVolume, true);

	
	/* ==== DETERMINE IF ONE CUBE OVERLAPS THE OTHER ==== */


	// iterrate over all vertices in cube 2
	for (int i = 0; i < nVertices / 2; i++)
	{
		// get the ith vertex
		Vector<3> vertex = pos2.row(i);

		// run contains query on this vertex
		cout << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): " << (scene.contains(vertex) ? "true" : "false") << endl;
	}

	// note that if any of the vertices in cube 2 are contained within the scene, then cube 2 overlaps with cube 1!

	return 0;
}
