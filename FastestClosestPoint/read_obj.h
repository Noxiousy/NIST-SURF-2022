/*
 * This header file contains a modified parser for reading .obj files
 * The parser is limited to vertices and indices specified by "v" and "f" 
 * lines of text.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<string> split(string s, string delim)
{
	// resulting list of strings
	vector<string> result;

	// variables to help split the string
	int pos;
	string substr;
	
	// iterrate until the delimiter is no longer within the text
	while ((pos = s.find(delim)) != string::npos)
	{
		// get the substring between the start of s and the next delim
		substr = s.substr(0, pos);

		// store the substring within the resulting list
		result.push_back(substr);

		// remove the substring and the corresponding delimiter from s
		s.erase(0, pos + delim.length());
	}

	// by now, there should only be 3 substrings in result
	// we must store the remaining text in the list
	result.push_back(s);

	// by now, result contains substrings of the original text split by the delimiter
	return result;
}

void parse(vector<vector<float>> &vertices, vector<int> &indices, string file)
{
	// open the file
	ifstream objFile(file);

	if (objFile.is_open())
	{
		// variables to help parse the .obj file
		string line;

		// iterrate through each line in the .obj file
		while (getline(objFile, line))
		{
			// check if this line begins with "v " or "f"
			if (line.find("v ") == 0 || line.find("f ") == 0)
			{
				// split this line by spaces
				vector<string> splitLine = split(line, " ");

				// check type of line ("v" or "f")
				if (splitLine[0] == "v")
				{
					// store float entries in a list of floats
					vector<float> vertex;

					vertex.push_back(stof(splitLine[1]));
					vertex.push_back(stof(splitLine[2]));
					vertex.push_back(stof(splitLine[3]));

					// push vertex list into vertices list
					vertices.push_back(vertex);
				}
				else
				{
					// check if "f" entries have "/" delim
					if (splitLine[1].find("/"))
					{
						// store integer before slashes in splitLine
						splitLine[1] = splitLine[1].substr(0, splitLine[1].find("/"));
						splitLine[2] = splitLine[2].substr(0, splitLine[2].find("/"));
						splitLine[3] = splitLine[3].substr(0, splitLine[3].find("/"));
					}

					// store indices
					indices.push_back(stoi(splitLine[1]));
					indices.push_back(stoi(splitLine[2]));
					indices.push_back(stoi(splitLine[3]));
				}
			}
		}

		// close the file
		objFile.close();
	}
	else
		cout << "Unable to open the .obj file..." << endl;
}
