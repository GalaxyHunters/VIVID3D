#include "Voronoi3D.hpp"
#include "iostream"
using namespace std;


#define BOX_SIZE 4
#define HEIGHT 2


int main(){
	cout << "starting program" << endl;
	vector<Vector3D> points;
	vector<bool> mask;
	vector<float> quan;

//////////////////////////////////////////////  make pyramid  //////////////////////////////

	int a(0);

	for (double i = -BOX_SIZE; i <= BOX_SIZE; i += 2) {
		for (double j = -BOX_SIZE; j <= BOX_SIZE; j += 2) {
			for (double z = -BOX_SIZE; z <= BOX_SIZE; z += 2) {
				points.push_back(Vector3D(i, j, z));
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

///////////////////////////////////////////////////////////////////////////////////
//point are a 3D grid in the size of BOX_SIZE+1
Voronoi3D temp(Vector3D(-10, -10, -10), Vector3D(10, 10, 10));
cout <<"start vorn"<<endl;
temp.Build(points);
cout << "finished vorn" << endl;
}
