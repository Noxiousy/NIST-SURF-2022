/*
 * fcpw.cpp now supports .obj file reading to construct geometric shapes
 */

#include <iostream>
#include <vector>
#include <string>
#include <fcpw/fcpw.h>

// my own header file
#include "read_obj.h"

using namespace std;
using namespace fcpw;

int main(int argc, char *argv[])
{
	// ==== READING THE .obj FILE ====
	

	// list of integers to store positions
	vector<vector<float>> vertices;
	vector<int> indices;

	// parse .obj file for vertices and triangle indices
	parse(vertices, indices, argv[1]);
	
	// the indices listed in the .obj file are indexed starting at 1. decrement all values
	for (int i = 0; i < indices.size(); i++)
		indices[i] -= 1;

	
	// ==== CREATING THE SCENE ====


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

	// create some QUERY POINTS (q1 is outside the tetrahedron, q2 is inside)
	Vector<3> q1(0, 0, 0), q2(2, 2, 2);

	// Interactions are passed to closest point query functions to store information about said query
	Interaction<DIM> interaction;

	// perform a closest point query within the scene
	scene.findClosestPoint(q1, interaction);

	// output of closest point query
	cout << "--- CLOSEST POINT QUERY ---" << endl;
	cout << "Query Point: (" << q1[0] << ", " << q1[1] << ", " << q1[2] << ")" << endl;
	cout << "Closest Point: (" << interaction.p[0] << ", " << interaction.p[1] << ", " << interaction.p[2] << ")" << endl;
	cout << "Distance from Query Point to Closest Point: " << interaction.d << endl;
	cout << "Primitive Index of Closest Point: " << interaction.primitiveIndex << endl;
	cout << "Object Index of Closest Point: " << interaction.objectIndex << endl;

	// output of contains query
	cout << endl << "--- CONTAINS QUERY --- " << endl;
	cout << "Q1: " << scene.contains(q1) << endl;
	cout << "Q2: " << scene.contains(q2) << endl;
	cout << "(0 -> false, 1 -> true)" << endl;

	return 0;
}
