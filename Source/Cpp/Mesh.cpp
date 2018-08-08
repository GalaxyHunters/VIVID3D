#include "fstream"
#include "Mesh.h"
#include "string"
#include "boost/algorithm/string/predicate.hpp"
#include "../../misc/int2str.hpp"

using namespace boost::algorithm;
Mesh::~Mesh() {}

Mesh::Mesh(vector<Point> points, vector<IndexedFace> faces, string label) : points(points), faces(faces), label(label) {}

//void Mesh::removePoints() {
//	vector<Point> newPoints;
//	for (vector<Point>::iterator it = points.begin(); it != points.end(); it++){
//		if (it->isPointDel() != true) {
//			newPoints.push_back(*it);
//		}
//	}
//	this->points = newPoints;
//}

//vector<float> static quan2color(float quan) {
//	vector<float> output;
//	output.push_back(0);
//	output.push_back(0);
//	output.push_back(0);
//	return output;
//}

//bool static compareColor(vector<float> color, vector<float> color2) {
//	return (color[0] == color2[0] && color[1] == color2[1] && color[2] == color2[2]);
//}
//
//bool compareQuan(Face face1, Face face2) {
//	return (face1.getQuan() > face2.getQuan());
//}
//bool(*compFace)(Face, Face) = compareQuan;

void Mesh::operator<<(string output) {
}

//void Mesh::operator<<(string output) { //TODO get the color sorted(a way to convert quan to color)
//	if (ends_with(output, ".obj")) { //check if hte output file ends with .obj, and delete it if it does
//		output.erase(output.length() - 4, 4);
//	}
//	ofstream o; // the obj file
//	o.open(output + ".obj");
//	ofstream m; //the mtl file
//	m.open(output + ".mtl");
//	string mtl = output;
//	while (mtl.find('/') != string::npos) {
//		mtl = mtl.substr(mtl.find('/'), string::npos);
//	}
//	//write obj file starter
//	o << "# This 3D code was produced by Vivid /n/n/n";
//	o << "mtllib" + mtl + "/n";
//	o << "o" + label + "/n";
//	//write points to obj file
//	int counter = 1; //obj file index starts at 1, not 0
//	for (vector<Point>::iterator it = points.begin(); it != points.end(); it++) {
//		o << "v " + int2str(it->getX()) + " " + int2str(it->getY) + " " + int2str(it->getZ()) + "/n";
//		//it->setIndex(counter);
//		//counter++;
//	}
//	//sort vecFaces by color
//	sort(faces.begin(), faces.end(), compFace); // NlogN
//	//write faces to obj file + write colors to mtl file
//	counter = 0;
//	vector<float> color = quan2color(faces[0].getQuan());
//	m << "newmtl surf_" + int2str(counter) + "\n" + \
//		"Ns 96.078\n" + \
//		"Ka 1.000 1.000 1.000 \n" + \
//		"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
//		"Ks 0.000 0.000 0.000\n" + \
//		"Ni 1.000000\n" + \
//		"d " + to_string(alpha) + "\n" + \
//		"illum 0\n" + \
//		"em 0.000000\n\n\n";
//	o << "usemtl surf_" + int2str(counter) + "/n";
//	for (vector<Face>::iterator it = vecFaces.begin(); it != vecFaces.end(); it++) {
//		if (compareColor(color, quan2color(it->getQuan()))) { //if true write the face to the obj file
//			o << "f ";
//			for (vector<Point>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
//				o << int2str(p->getIndex()) + " ";
//			}
//			o << "/n";
//		}
//		else // we reached a new color, and we need to write it in mtl before using it
//		{
//			color = quan2color(it->getQuan());
//			counter++;
//			m << "newmtl surf_" + int2str(counter) + "\n" + \
//				"Ns 96.078\n" + \
//				"Ka 1.000 1.000 1.000 \n" + \
//				"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
//				"Ks 0.000 0.000 0.000\n" + \
//				"Ni 1.000000\n" + \
//				"d " + to_string(alpha) + "\n" + \
//				"illum 0\n" + \
//				"em 0.000000\n\n\n";
//			o << "usemtl surf_" + int2str(counter) + "/n";
//			// now we can write the face in the obj file
//			o << "f ";
//			for (vector<Point>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
//				o << p->getIndex() + " ";
//			}
//			o << "/n";
//		}
//	}
//	o.close();
//	m.close();
//}

void Mesh::simplify(size_t triangles, size_t vertices, float error) {
	//TODO sort out the reduse

}