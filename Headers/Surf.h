
#include "Face.h"
#include <vector>

using namespace std;

class Surf {
private:
	vector<Face> vecFaces;
	vector<Point> vecPoints;
	string label;
	float Vmin;
	float Vmax;

public:
	Surf(vector<Face> vecFaces, vector<Point> vecPoints, string label);
	void exportObj(string output);
	Surf reduce();
};
