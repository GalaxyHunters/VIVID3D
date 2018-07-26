#pragma once

#include "Face.h"
#include <vector>

using namespace std;

class Surf {
	
private:
	vector<Face> vecFaces;
	vector<Point> vecPoints;
	string label;
	float * OG_points; // the original points that we got as an input


	Surf(float ** inputPoints, vector<Face> vecFaces, vector<Point> vecPoints, float quan[], string label);
	void runVorn(float ** inputPoints);
	void clearFaces(bool mask[]);
public:
	Surf* create(float ** inputPoints, bool mask[], float quan[], string label);
	void smoothSurf(); //smooth the surf
	void exportObj(string output);
	void exportRedusedObj(string output, int repeat); //export obj after reduse, repeat is the amount of times reduse will be ran(1 is usally the best)
	void removePoints(); //clears all the points who arent used from the vecPoints
};