#include <iostream>
#include <fcpw/fcpw.h>

using namespace std;
using namespace fcpw;

int main()
{
	// creating a 3D scene with 2 overlapping cubes
	Scene<3> scene;
	scene.setObjectTypes({{PrimitiveType::Triangle}, {PrimitiveType::Triangle}});

	const int nVertices = 8, nTriangles = 12;	// constants for each scene
	scene.setObjectVertexCount(nVertices, 0);
	scene.setObjectVertexCount(nVertices, 1);
	scene.setObjectTriangleCount(nTriangles, 0);
	scene.setObjectTriangleCount(nTriangles, 1);

	Eigen::Matrix<float, nVertices, 3> pos1, pos2;

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
				0, 1, 5, 
				0, 2, 6,
				0, 4, 5,
				0, 4, 6, 
				1, 2, 3, 
				1, 3, 7,
				1, 5, 7,
				2, 3, 7,
				2, 6, 7,
				4, 5, 6, 
				5, 6, 7
       			};

	// add vertices and indices to the scene
	for (int i = 0; i < nVertices; i++)
	{
		scene.setObjectVertex(pos1.row(i), i, 0);
		scene.setObjectVertex(pos2.row(i), i, 1);
	}

	for (int i = 0; i < nTriangles; i++)
	{
		scene.setObjectTriangle(&indices[3 * i], i, 0);
		scene.setObjectTriangle(&indices[3 * i], i, 1);
	}

	scene.build(AggregateType::Bvh_OverlapVolume, true);

	
	/* ==== DETERMINE IF ONE CUBE OVERLAPS THE OTHER ==== */


	// get scene data
	SceneData<3> *data = scene.getSceneData();

	// output
	cout << "Determine which vertices from CUBE 2 are inside CUBE 1: " << endl;

	// iterrate over all vertices in cube 2
	for (int i = 0; i < nVertices; i++)
	{
		// get the ith vertex
		Vector<3> vertex = pos2.row(i);

		// run contains query on this vertex and first cube
		cout << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): " 
			<< (data -> aggregateInstancePtrs[0] -> contains(vertex) ? "true" : "false") << endl;
	}

	// note that if any of the vertices in cube 2 are contained within the scene, then cube 2 overlaps with cube 1

	// output
	cout << endl << "Determine which vertices from CUBE 1 are inside CUBE 1 (should be all true or all false): " << endl;

	for (int i = 0; i < nVertices; i++)
	{
		Vector<3> vertex = pos1.row(i);

		cout << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "): "
			<< (data -> aggregateInstancePtrs[0] -> contains(vertex) ? "true" : "false") << endl;
	}

	// the queries above proves that a contains query on the vertices of any shape will return false
	return 0;
}
