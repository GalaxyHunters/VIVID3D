#include "Surf.h"

int main(){
	std::cout << "starting program" << endl;
	vector<Point> points;
	vector<bool> mask;
	vector<float> quan;
	for (int i = 2; i > -4; i -= 2) {
		for (int j = 2; j > -4; j -= 2) {
			for (int z = 2; z > -4; z -= 2) {
				points.push_back(Point(i, j, z));
				quan.push_back(0);
				mask.push_back(false);
				if (i == j == z == 0) { 
					mask.back() = true; 
				}
			}
		}
	}
	std::cout << "starting input" << endl;
	Surf surf;
	surf = surf.createSurf(points, mask, quan);
	std::cout << "starting output";
	surf.exportToObj("C:\\Users\\Tomer\\vivid", "vivid_3d_obj", 1.0);
}