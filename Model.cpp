#include "Model.h"

using namespace boost::algorithm;

Model::Model() {}

Model::Model(vector<Mesh> meshes) {
	this->meshes = meshes;
}

vector<float> static quan2color(float quan) {
	vector<float> output;
	output.push_back(0);
	output.push_back(0);
	output.push_back(0);
	return output;
}

bool static compareColor(vector<float> color, vector<float> color2) {
	return (color[0] == color2[0] && color[1] == color2[1] && color[2] == color2[2]);
}

bool static compareQuan(IndexedFace face1, IndexedFace face2) {
	return (face1.getColor() > face2.getColor());
}
static bool(*compFace)(IndexedFace, IndexedFace) = compareQuan;

void Model::operator<<(string output) {
	if (ends_with(output, ".obj")) { //check if hte output file ends with .obj, and delete it if it does
		output.erase(output.length() - 4, 4);
	}
	ofstream o; // the obj file
	o.open(output + ".obj");
	ofstream m; //the mtl file
	m.open(output + ".mtl");
	string mtl = output;
	while (mtl.find('\\') != string::npos) {
		mtl = mtl.substr(mtl.find('\\'), string::npos);
	}
	//write obj file starter
	o << "# This 3D code was produced by Vivid /n/n/n";
	o << "mtllib" + mtl + "/n";
	
	size_t mtlCounter = 0; // will be used to count the newmtl
	size_t pointsCounter = 0; // will be used to count how many points the former obj wrote to the file
	for (vector<Mesh>::iterator mesh = this->meshes.begin(); mesh != this->meshes.end(); mesh++) {
		o << "o" + mesh->getLabel() + "/n";
		//write points to obj file
		for (vector<Point>::iterator it = mesh->getPoints().begin(); it != mesh->getPoints().end(); it++) {
			o << "v " + int2str(it->getX()) + " " + int2str(it->getY()) + " " + int2str(it->getZ()) + "/n";
		}
		//sort vecFaces by color
		sort(mesh->getFaces().begin(), mesh->getFaces().end(), compFace); // NlogN
		//write faces to obj file + write colors to mtl file
		vector<float> color = quan2color(mesh->getFaces()[0].getColor());
		m << "newmtl surf_" + int2str(mtlCounter) + "\n" + \
			"Ns 96.078\n" + \
			"Ka 1.000 1.000 1.000 \n" + \
			"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
			"Ks 0.000 0.000 0.000\n" + \
			"Ni 1.000000\n" + \
			"d " + to_string(mesh->getAlpha()) + "\n" + \
			"illum 0\n" + \
			"em 0.000000\n\n\n";
		o << "usemtl surf_" + int2str(mtlCounter) + "/n";
		for (vector<IndexedFace>::iterator it = mesh->getFaces().begin(); it != mesh->getFaces().end(); it++) {
			if (compareColor(color, quan2color(it->getColor()))) { //if true write the face to the obj file
				o << "f ";
				for (vector<size_t>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
					o << int2str(*p + pointsCounter) + " ";
				}
				o << "/n";
			}
			else // we reached a new color, and we need to write it in mtl before using it
			{
				color = quan2color(it->getColor());
				mtlCounter++;
				m << "newmtl surf_" + int2str(mtlCounter) + "\n" + \
					"Ns 96.078\n" + \
					"Ka 1.000 1.000 1.000 \n" + \
					"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
					"Ks 0.000 0.000 0.000\n" + \
					"Ni 1.000000\n" + \
					"d " + to_string(mesh->getAlpha()) + "\n" + \
					"illum 0\n" + \
					"em 0.000000\n\n\n";
				o << "usemtl surf_" + int2str(mtlCounter) + "/n";
				// now we can write the face in the obj file
				o << "f ";
				for (vector<size_t>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
					o << int2str(*p + pointsCounter) + " ";
				}
				o << "/n";
			}
		}
		pointsCounter += mesh->getPoints().size();
	}
}

void Model::addMesh(Mesh mesh) {
	this->meshes.push_back(mesh);
}


Model::~Model(){}