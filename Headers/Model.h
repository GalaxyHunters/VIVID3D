#pragma once
#include "Mesh.h"

#ifndef MODEL_H
#define MODEL_H

using namespace std;

class Model
{
public:
	Model(vector<Mesh> meshes);
	~Model();
	void operator<<(string output);
	void save(string outputFile);
	static Mesh load(string inputFile);
	void addMesh(Mesh mesh);
private:
	vector<Mesh> meshes;
};

#endif