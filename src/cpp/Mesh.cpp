#include "Mesh.h"

using namespace boost::algorithm;
Mesh::~Mesh() {}

Mesh::Mesh() {}

Mesh::Mesh(vector<Point> points, vector<IndexedFace> faces, string label, float alpha) : points(points), faces(faces), label(label), alpha(alpha) {}

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

void Mesh::operator<<(string output) { //TODO get the color sorted(a way to convert quan to color)
	if (ends_with(output, ".obj")) { //check if hte output file ends with .obj, and delete it if it does
		output.erase(output.length() - 4, 4);
	}
	ofstream o; // the obj file
	o.open(output + ".obj");
	ofstream m; //the mtl file
	m.open(output + ".mtl");
	string mtl = output;
	while (mtl.find('/') != string::npos) {
		mtl = mtl.substr(mtl.find('/'), string::npos);
	}
	//write obj file starter
	o << "# This 3D code was produced by Vivid /n/n/n";
	o << "mtllib" + mtl + "/n";
	o << "o" + label + "/n";
	//write points to obj file
	for (vector<Point>::iterator it = this->points.begin(); it != this->points.end(); it++) {
		o << "v " + int2str(it->getX()) + " " + int2str(it->getY()) + " " + int2str(it->getZ()) + "/n";
	}
	//sort vecFaces by color
	sort(this->faces.begin(), this->faces.end(), compFace); // NlogN
	//write faces to obj file + write colors to mtl file
	size_t counter = 0;
	vector<float> color = quan2color(this->faces[0].getColor());
	m << "newmtl surf_" + int2str(counter) + "\n" + \
		"Ns 96.078\n" + \
		"Ka 1.000 1.000 1.000 \n" + \
		"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
		"Ks 0.000 0.000 0.000\n" + \
		"Ni 1.000000\n" + \
		"d " + to_string(this->alpha) + "\n" + \
		"illum 0\n" + \
		"em 0.000000\n\n\n";
	o << "usemtl surf_" + int2str(counter) + "/n";
	for (vector<IndexedFace>::iterator it = this->faces.begin(); it != this->faces.end(); it++) {
		if (compareColor(color, quan2color(it->getColor()))) { //if true write the face to the obj file
			o << "f ";
			for (vector<size_t>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
				o << int2str(*p) + " ";
			}
			o << "/n";
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = quan2color(it->getColor());
			counter++;
			m << "newmtl surf_" + int2str(counter) + "\n" + \
				"Ns 96.078\n" + \
				"Ka 1.000 1.000 1.000 \n" + \
				"Kd " + to_string(color[0]) + " " + to_string(color[1]) + " " + to_string(color[2]) + "\n" + \
				"Ks 0.000 0.000 0.000\n" + \
				"Ni 1.000000\n" + \
				"d " + to_string(alpha) + "\n" + \
				"illum 0\n" + \
				"em 0.000000\n\n\n";
			o << "usemtl surf_" + int2str(counter) + "/n";
			// now we can write the face in the obj file
			o << "f ";
			for (vector<size_t>::iterator p = it->getPoints().begin(); p != it->getPoints().end(); p++) {
				o << int2str(*p) + " ";
			}
			o << "/n";
		}
	}
	o.close();
	m.close();
}

void Mesh::simplify(float verticlePercent, float error) {
	//TODO sort out the reduse

}

//geters seters

string Mesh::getLabel() { return this->label; }
float Mesh::getAlpha() { return this->alpha; }
vector<Point> Mesh::getPoints() { return this->points; }
vector<IndexedFace> Mesh::getFaces() { return this->faces; }
void Mesh::setLabel(string label) { this->label = label; }
void Mesh::setAlpha(float alpha) { this->alpha = alpha; }

