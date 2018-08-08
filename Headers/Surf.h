#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "Mesh.h"
#include "Surfface.h"
using namespace std;

class Surf{
private:
	vector<std::shared_ptr<Point>> vecPoints;
	vector<SurfFace> vecFaces;
	vector<double[3]> inputPoints; // for smooth
	vector<bool> mask; //for smooth
	vector<float> quan; // for smooth

	void cleanFaces(vector<bool> mask); // clean the unneaded faces(by mask)
	void cleanPoints(); // removes all the unused points

	//smooth functions:
	void _setPinPout(vector<int> Pout, vector<int> Pin);
	void _updateInputPoints(vector<int> Pout, vector<int> Pin);
	void _makeMask(int PoutS, int PinS);
	void _S2addPoints(vector<int> Pout, vector<int> Pin);

	Surf();
public:	
	Surf createSurf(vector<double[3]> C_points, vector<bool> mask, vector<float> quan);
	void smoothSurf();
	const Mesh to_mesh(string label);
	void exportToObj(string output, string label, float alpha);


//private:
//	
//	vector<double[3]> inputPoints; // the original points that we got as an input
//	vector<bool> mask;
//	vector<float> quan;
//
//	Surf(vector<double[3]> inputPoints, vector<PolyFace> vecFaces, vector<Point> vecPoints, string label, float alpha);
//	void runVorn();
//	void cleanFaces(vector<bool> mask);
//	void _setPinPout(vector<int> Pout, vector<int> Pin);
//	void _updateInputPoints(vector<int> Pout, vector<int> Pin); // update Cpoints in order to run vornoi again(smooth)
//	void _makeMask(int PoutS, int PinS);
//	void _S2addPoints(vector<int> Pout, vector<int> Pin);
//public:
//	Surf();
//	Surf createSurf(vector<double[3]> inputPoints, vector<bool> mask, vector<float> quan, string label, float alpha);
//	void smoothSurf(); //smooth the surf
//	string getLabel();
//	void setLabel(string label);
//	float getAlpha();
//	void setAlpha(float alpha);
};
	
