#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model();
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
	inline Model Model_new(vector<Mesh> meshes) { return Model(meshes); }
	inline void Model_export(Model model, string output) { model << output; }
	inline void Model_save(Model model, string outputFile) { model.save(outputFile); }
	inline Mesh Model_load(Model model, string inputFile) { return model.load(inputFile); }
	inline void Model_addMesh(Model model, Mesh mesh) { model.addMesh(mesh); }
}

#endif