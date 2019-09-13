#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Surf.h"
using namespace std;

class CModel
{
public:
	CModel();
	CModel(vector<CMesh> aMeshes);
	CModel(vector<CSurf> aSurfs, string aLabel, cord_t aAlpha);
	~CModel();
	void ExportToObj(string aOutput);
	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurf aSurf, string aLabel, cord_t aAlpha);
private:
	vector<CMesh> mMeshes;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * aMesh, size_t * mtl_counter, size_t aPointsCounter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, Color_t color, cord_t aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);
};

#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(Model, m) {
    py::class_<CModel>(m, "CModel")
            .def(py::init<> (), "default constructor for CModel")
            .def(py::init<vector<CMesh> > (), "constructor for CModel, from meshes", py::arg("Meshes"))
            .def(py::init<vector<CSurf>, string, cord_t> (), "constructor for CModel, from surfs", py::arg("Surfs"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("AddMesh", &CModel::AddMesh, "add another mesh to Model", py::arg("Mesh"))
            .def("AddSurf", &CModel::AddSurf, "add another mesh to model, using surf", py::arg("surf"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("ExportToObj", &CModel::ExportToObj, "writes the surfaces to an OBJ file", py::arg("aOutputFile"));
}
#endif