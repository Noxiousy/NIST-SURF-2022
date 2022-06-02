#include <iostream>
#include <fcpw/fcpw.h>

using namespace std;
using namespace fcpw;

int main()
{
	// a scene is a spatial index for the shapes and query.
	const int DIM = 3;
	Scene<DIM> scene;

	// create a scene with one object containing triangles
	scene.setObjectTypes({{PrimitiveType::Triangle}});

	// set the vertex and the triangle count of the 0th object (only one object type defined above)
	const int nVertices = 4, nTriangles = 4;
	scene.setObjectVertexCount(nVertices, 0);
	scene.setObjectTriangleCount(nTriangles, 0);

	// create an Eigen::Matrix containing the coordinates of each vertex in the scene
	Eigen::Matrix<float, nVertices, DIM> positions;
	
	// we will use the following four vertices in 3D space to create a tetrahedron
	positions << 	1, 1, 0,
		 	1, 3, 0,
			4, 2, 0,
			2, 2, 2;

	// set the positions of each vertex within the scene by passing each row of the matrix
	for (int i = 0; i < nVertices; i++)
		scene.setObjectVertex(positions.row(i), i, 0);

	// specifiy the indices of each vertex for the four triangles (some combination of three indices)
	int indices[] = {0, 1, 2,
			 0, 1, 3,
			 0, 2, 3,
			 1, 2, 3};
	
	for (int i = 0; i < nTriangles; i++)
		scene.setObjectTriangle(&indices[3 * i], i, 0);

	// build the scene (true allows vectorization)
	scene.build(AggregateType::Bvh_SurfaceArea, true);

	// create some QUERY POINTS (q1 is outside the tetrahedron, q2 is inside)
	Vector<3> q1(0, 0, 0), q2(2, 2, 1);

	// Interactions are passed to closest point query functions to store information about said query
	Interaction<DIM> interaction;

	// perform a closest point query within the scene
	scene.findClosestPoint(q1, interaction);

	// output of closest point query
	cout << "--- CLOSEST POINT QUERY ---" << endl;
	cout << "Query Point: (" << q1[0] << ", " << q1[1] << ", " << q1[2] << ")" << endl;
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
