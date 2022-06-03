#include <iostream>
#include <fcpw/fcpw.h>
#include <time.h>
#include <string>

using namespace fcpw;
using namespace std;

int main(int argc, char* argv[])
{
	// create a 3D scene
	Scene<3> scene;

	// create an object with a bunch of triangles inside it
	scene.setObjectTypes({{PrimitiveType::Triangle}});

	// create a cube out of 12 triangles
	// note that 2 triangles can form a square and 6 squares form a cube
	const int nVertices = 8, nTriangles = 12;
	scene.setObjectVertexCount(nVertices, 0);
	scene.setObjectTriangleCount(nTriangles, 0);

	// vertices of pyramid in the scene
	Eigen::Matrix<float, nVertices, 3> pos;

	/*
	 * the cube will have a length/width/height of 40 centered at the origin.
	 * the vertices of the top and bottom faces are as follows:
	 * 	
	 * 	top face: 	(-20, -20, 20), (-20, 20, 20), (20, 20, 20), (20, -20, 20)
	 * 	bottom face:	(-20, -20, -20), (-20, 20, -20), (20, 20, -20), (20, -20, -20)
	 * 
	 * since the geometry must be formed with trianlges, we create two triangles that share two
	 * opposite corners of each cube face in order to form a square. For example, the square for
	 * the top face can be create with the following traingles:
	 * 	
	 * 	Triangle 1:	(-20, -20, 20), *(-20, 20, 20), (20, 20, 20)
	 * 	Triangle 2:	(-20, -20, 20), *(20, -20, 20), (20, 20, 20)
	 *	*note the difference between the second vertex of each traingle.
	 *
	 *	the resulting square is as follows:
	 *	Square 1: (-20, -20, 20), (-20, 20, 20), (20, 20, 20), (20, -20, 20)
	 */

	pos << 	-20, -20, -20,
	    	 20, -20, -20,
		-20,  20, -20,
		 20,  20, -20,
		-20, -20,  20,
		 20, -20,  20,
		-20,  20,  20,
		 20,  20,  20;

	int indices[] = {0, 1, 2,
			 0, 1, 3,
			 0, 1, 4,
			 0, 1, 5,
			 0, 2, 4,
			 0, 2, 6,
			 7, 6, 2,
			 7, 2, 3,
			 7, 5, 1,
			 7, 3, 1,
			 7, 6, 5,
			 4, 5, 6};

	// add the vertices to the scene
	for (int i = 0; i < nVertices; i++)
		scene.setObjectVertex(pos.row(i), i, 0);

	// add the corresponding indices of the vertices for each triangle to the scene
	for (int i = 0; i < nTriangles; i++)
		scene.setObjectTriangle(&indices[3 * i], i, 0);

	// build scene
	scene.build(AggregateType::Bvh_SurfaceArea, true);


	/* ==== BENCHMARK CODE ==== */
	

	// get benchmark preferences from command line
	// "./fcpw.exe [# of queries] [query] [random]"
	int trials = stoi(argv[1]), i = trials;
	string query = argv[2];
	bool genRandom = (argv[3] == "true" ? true : false);

	// timer
	clock_t t;

	// check query type
	if (query == "contains")
	{
		// check for randomization
		if (genRandom)
		{
			// start timer
			t = clock();

			// loop for i number of trials
			while (i-- > 0)
			{
				// query a random vertex in a 60 x 60 x 60 unit space
				scene.contains({(float)(rand() % 61 - 30),
						(float)(rand() % 61 - 30),
						(float)(rand() % 61 - 30)});
			}

			// stop timer
			t = clock() - t;
		}
		else
		{
			// create a fixed query point at the origin
			Vector<3> queryPoint(0, 0, 0);

			// start timer
			t = clock();

			// loop for i number of trials
			while (i-- > 0) 
				scene.contains(queryPoint);

			// stop timer
			t = clock() - t;
		}
	}
	else if (query == "closest_point")
	{
		// interaction object
		Interaction<3> interaction;

		// check for randomization
		if (genRandom)
		{
			// start timer
			t = clock();

			// loop for i number of trails
			while (i-- > 0)
			{
				// query a random vertex in a 60 x 60 x 60 unit space
				Vector<3> queryPoint(rand() % 61 - 30, rand() % 61 - 30, rand() % 61 - 30);
				scene.findClosestPoint(queryPoint, interaction);
			}

			// stop timer
			t = clock() - t;
		}
		else
		{
			// create a fixed query point at the origin
			Vector<3> queryPoint(0, 0, 0);

			// start timer
			t = clock();

			// loop for i number of trials
			while (i-- > 0)
				scene.findClosestPoint(queryPoint, interaction);

			// stop timer
			t = clock() - t;
		}
	}
	else
	{
		cout << "\"" << query << "\" was not recognized. Please enter \"contains\" or \"closest_point\" for the [query] argument..." << endl;
		return 0;
	}

	// print benchmark
        cout << "Benchmark time for " << trials << " \"" << query << "\" queries: " << (float) t / CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}
