#include "Mesh.h"
#include "ColorMap.h"
using namespace boost::algorithm;
Mesh::~Mesh() {}

Mesh::Mesh(vector<Point> points, vector<IndexedFace> faces, string label, float alpha) : points(points), faces(faces), label(label), alpha(alpha) {}

Color_t static quan2color(float quan) {
	ColorMap output;
	return output.getColor(quan);
}

bool static compareColor(Color_t color, Color_t color2) {
	return (color.R == color2.R && color.G == color2.G && color.B == color2.B);
}

bool static compareQuan(IndexedFace face1, IndexedFace face2) {
	return (face1.getColor() > face2.getColor());
}
static bool(*compFace)(IndexedFace, IndexedFace) = compareQuan;

void Mesh::operator<<(string output) { //TODO get the color sorted(a way to convert quan to color)
	if (ends_with(output, ".obj")) { //check if the output file ends with .obj, and delete it if it does
		output.erase(output.length() - 4, 4);
	}
	ofstream o; // the obj file
	o.open(output + ".obj");
	ofstream m; //the mtl file
	m.open(output + ".mtl");
	string mtl = output + ".mtl";
	while (mtl.find('\\') != string::npos) {
		mtl = mtl.substr(mtl.find('\\') + 1, string::npos);
	}
	//write obj file starter
	o << "# This 3D code was produced by Vivid \n\n\n";
	o << "mtllib " + mtl + "\n";
	o << "o " + label + "\n";
	//write points to obj file
	for (vector<Point>::iterator it = this->points.begin(); it != this->points.end(); it++) {
		o << "v " + to_string(it->getX()) + " " + to_string(it->getY()) + " " + to_string(it->getZ()) + "\n";
	}
	//sort vecFaces by color
	sort(this->faces.begin(), this->faces.end(), compFace); // NlogN
	//write faces to obj file + write colors to mtl file
	size_t counter = 0;
	Color_t color = quan2color(this->faces[0].getColor());
	m << "newmtl surf_" + to_string(counter) + "\n" + \
		"Ns 96.078\n" + \
		"Ka 1.000 1.000 1.000 \n" + \
		"Kd " + to_string(color.R) + " " + to_string(color.G) + " " + to_string(color.B) + "\n" + \
		"Ks 0.000 0.000 0.000\n" + \
		"Ni 1.000000\n" + \
		"d " + to_string(this->alpha) + "\n" + \
		"illum 0\n" + \
		"em 0.000000\n\n\n";
	o << "usemtl surf_" + int2str(counter) + "\n";
	for (vector<IndexedFace>::iterator it = this->faces.begin(); it != this->faces.end(); it++) {
		if (compareColor(color, quan2color(it->getColor()))) { //if true write the face to the obj file
			o << "f ";
			vector<size_t> facePoints = it->getPoints();
			for (vector<size_t>::iterator point = facePoints.begin(); point != facePoints.end(); point++) {
				o << int2str(*point + 1) + " ";
			}
			o << "\n";
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = quan2color(it->getColor());
			counter++;
			m << "newmtl surf_" + int2str(counter) + "\n" + \
				"Ns 96.078\n" + \
				"Ka 1.000 1.000 1.000 \n" + \
				"Kd " + to_string(color.R) + " " + to_string(color.G) + " " + to_string(color.B) + "\n" + \
				"Ks 0.000 0.000 0.000\n" + \
				"Ni 1.000000\n" + \
				"d " + to_string(alpha) + "\n" + \
				"illum 0\n" + \
				"em 0.000000\n\n\n";
			o << "usemtl surf_" + int2str(counter) + "\n";
			// now we can write the face in the obj file
			o << "f ";
			vector<size_t> facePoints = it->getPoints();
			for (vector<size_t>::iterator point = facePoints.begin(); point != facePoints.end(); point++) {
				o << int2str(*point + 1) + " ";
			}
			o << "\n";
		}
	}
	o.close();
	m.close();
}

void Mesh::decimation(float verticlePercent, float maxError) {
	//triangulation
	this->triangulation();
	//call decimation from utils
	int targetVerticesN = verticlePercent * this->points.size();
	int targetTrianglesN = verticlePercent * this->faces.size();
	pair<vector<Point>, vector<IndexedFace>> temp = decimateMesh(this->points, this->faces, targetVerticesN, targetTrianglesN, maxError);
	this->points = get<0>(temp);
	this->faces = get<1>(temp);
}

void Mesh::triangulation() {
	vector<IndexedFace> triangles;
	for (vector<IndexedFace>::iterator fIt = this->faces.begin(); fIt != this->faces.end(); fIt++) {
		for (size_t i = 1; i < fIt->getPoints().size()-1; i++) { // go over all the vertices from 1 to n-1 and connect them with vertice 0 to create triangles 
			triangles.push_back(IndexedFace((*fIt)[0], (*fIt)[i], (*fIt)[i + 1], fIt->getColor()));
		}
	}
	this->faces = triangles;
}
//geters seters

string Mesh::getLabel() { return this->label; }
float Mesh::getAlpha() { return this->alpha; }
vector<Point> Mesh::getPoints() { return this->points; }
vector<IndexedFace> Mesh::getFaces() { return this->faces; }
void Mesh::setLabel(string label) { this->label = label; }
void Mesh::setAlpha(float alpha) { this->alpha = alpha; }
