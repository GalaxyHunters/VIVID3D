#include <vector>
#include <iostream>
#include "Point.h"
#include "IndexedFace.h"

using namespace std;

class Mesh {

private:
	string label;
	vector<Point> points;
	vector<IndexedFace> faces;

public:
	Mesh(vector<Point> points, vector<IndexedFace> faces, string label);
	~Mesh();
	void simplify(size_t triangles, size_t vertices, float error);
	void operator<<(string output);
//protected:
//	vector<Face> vecFaces;
//	vector<Point> vecPoints;
//	string label;
//	float alpha;
//public:
//	Mesh();
//	Mesh(vector<Face> vecFaces, vector<Point> vecPoints, string label, float alpha);
//	void exportObj(string output);
//	Mesh reduce();
//	//Mesh merge(Mesh mesh); for now this function is cancelld
//	void removePoints(); //clears all the points who arent used from the vecPoints

};
