/*
 * obj_to_bod.cpp is a C++ file that reads an .obj file path and converts the contents of
 * it to a .bod file. 
 *
 * Please note that this code assumes the object provided in the .obj file is represented
 * with triangular faces. Thus, the resulting .bod file will only contain TRIANGLE entries.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "read_obj.h"

using namespace std;

int main(int argc, char** argv)
{
	string path = argv[1];

	// get the contents of the .obj file
	vector<vector<float>> vertices;
	vector<int> indices;

	parse(vertices, indices, path);

	// write to a new .bod file
	int pos = 0;
	while ((pos = path.find("/")) != string::npos)
		path.erase(0, pos + 1);

	path.replace(0, 0, "BOD files/");
	path.replace(path.length() - 4, 4, ".bod");

	ofstream outfile(path);
	if (outfile.is_open())
	{
		for (int i = 0; i < indices.size(); i++)
		{
			if (i % 3 == 0)
				outfile << "TRIANGLE";

			vector<float> vertex(vertices[indices[i]]);

			outfile << " " << vertex[0] << " " << vertex[1] << " " << vertex[2];

			if (i % 3 == 2)
				outfile << endl;
		}

		outfile.close();
	}
	else
		cout << "Unable to open \"" << path << "\"..." << endl;

	return 0;
}
