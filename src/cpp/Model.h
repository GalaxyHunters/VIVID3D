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

extern "C"
{
	Model Model_new(vector<Mesh> meshes) { return Model(meshes); }
	void Model_export(Model model, string output) { model << output; }
	void Model_save(Model model, string outputFile) { model.save(outputFile); }
	Mesh Model_load(Model model, string inputFile) { return model.load(inputFile); }
	void Model_addMesh(Model model, Mesh mesh) { model.addMesh(mesh); }
}


#endif
