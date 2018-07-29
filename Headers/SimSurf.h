#pragma once

#include "Surf.h"
using namespace std;

class SimSurf: public Surf {
private:

	float * OG_points; // the original points that we got as an input


	SimSurf(float ** inputPoints, vector<Face> vecFaces, vector<Point> vecPoints, string label);
	void runVorn(float ** inputPoints);
	void cleanFaces(bool mask[]);
	void removePoints(); //clears all the points who arent used from the vecPoints
public:
	SimSurf * create(float ** inputPoints, bool mask[], float quan[], string label);
	void smoothSurf(); //smooth the surf
};
	
