#pragma once

#ifndef SURF_H
#define SURF_H


#include "Mesh.h"
#include "SurfFace.h"
#include "Utils.h"

#include <iostream>
#include <map>
using namespace std;

class Surf{
private:
	vector<std::shared_ptr<Point>> vecPoints;
	vector<SurfFace> vecFaces;
	vector<Point> inputPoints; // for smooth
	vector<bool> mask; //for smooth
	vector<float> quan; // for smooth

	inline vector<Point> getInputPoints() { return this->inputPoints; }
	inline vector<bool> getMask() { return this->mask; }
	inline vector<float> getQuan() { return this->quan; }
	inline void setInputPoint(vector<Point> inputPoints) { this->inputPoints = inputPoints; }
	inline void setMask(vector<bool> mask) { this->mask = mask; }
	inline void setQuan(vector<float> quan) { this->quan = quan; }
 
	void runVorn();
	void cleanFaces(vector<bool> mask); // clean the unneaded faces(by mask)
	void cleanPoints(); // removes all the unused points

	//smooth functions:
	pair<vector<size_t>, vector<size_t>> _setPinPout();
	void _updateInputPoints(vector<size_t> Pout, vector<size_t> Pin);
	void _makeMask(size_t PoutS, size_t PinS);
	pair<vector<size_t>, vector<size_t>> _S2addPoints(vector<size_t> Pout, vector<size_t> Pin);

	Surf();
public:	
	static Surf createSurf(vector<Point> InputPoints, vector<bool> mask, vector<float> quan);
	void smoothSurf();
	const Mesh to_mesh(string label, float alpha);
	void exportToObj(string output, string label, float alpha);

};

#endif
	
