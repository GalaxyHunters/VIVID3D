#include "Surf.h" // imports Vornoi3D.hpp


#define BOX_SIZE 2
#define HEIGHT 1

int main(){
	std::cout << "starting program" << endl;
	vector<Point> points;
	vector<bool> mask;
	vector<float> quan;

//////////////////////////////////////////////  make pyramid  //////////////////////////////

	int a(0);

	for (double i = -BOX_SIZE; i <= BOX_SIZE; i += 2) {
		for (double j = -BOX_SIZE; j <= BOX_SIZE; j += 2) {
			for (double z = -BOX_SIZE; z <= BOX_SIZE; z += 2) {
				points.push_back(Point(i, j, z));
				if (z >= 0 && HEIGHT >= z) {
					a = HEIGHT - z;
				}

				else {
					a = 0;
				}

				if ((z >= 0 && HEIGHT >= z) && (-a <= i && i <= a) && (-a <= j && j <= a)) {
					mask.push_back(true);
					quan.push_back(pow(10, a) + 0.0001);
				}

				else {
					mask.push_back(false);
					quan.push_back(0.00001);
				}
			}
		}
	}

///////////////////////////////////////////////////  make cube  ////////////////////////////////
//	for (int i = 2; i > -4; i -= 2) { // make the vornoi input points, a 3d grid for all combination optionts for 2, 0, -2
//		for (int j = 2; j > -4; j -= 2) {
//			for (int z = 2; z > -4; z -= 2) {
//				points.push_back(Point(i, j, z));
//				quan.push_back(0);
//				mask.push_back(false);
//				if (i == j && j == z && z == 0) { 
//					mask.back() = true; 
//				}
//			}
//		}
//	}

	std::cout << quan.size() << endl;
	std::cout << "starting input" << endl;
	Surf surf = surf.createSurf(points, mask, quan);
	std::cout << "starting smooth" << endl;
//	surf.smoothSurf();
	std::cout << "starting output" << endl;
        surf.exportToObj("../../test_models/makefile_pyramid_test_run", "vivid_3d_obj", 1.0);
}
