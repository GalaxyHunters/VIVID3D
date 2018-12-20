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
	Surf();
	vector<std::shared_ptr<Point>> vecPoints;
	vector<SurfFace> vecFaces;
	vector<Point> inputPoints; // for smooth
	vector<bool> mask; //for smooth
	vector<float> quan; // for smooth

	inline vector<Point> getInputPoints() { return this->inputPoints; }
	inline vector<bool> getMask() { return this->mask; }
	inline vector<float> getQuan() { return this->quan; }
	inline void setInputPoints(vector<Point> inputPoints) { this->inputPoints = inputPoints; }
	inline void setMask(vector<bool> mask) { this->mask = mask; }
	inline void setQuan(vector<float> quan) { this->quan = quan; }

	void runVorn();
	void cleanFaces(vector<bool> mask); // clean the unneaded faces(by mask)
	void cleanPoints(); // removes all the unused points

	//smooth functions:
	pair<vector<size_t>, vector<size_t>> _setPinPout();
	void _updateInputPoints(vector<size_t> Pout, vector<size_t> Pin);
	void _makeMask(size_t PoutSize, size_t PinSize);
	pair<vector<size_t>, vector<size_t>> _S2addPoints(vector<size_t> Pout, vector<size_t> Pin);

public:
	static Surf createSurf(vector<Point> InputPoints, vector<bool> mask, vector<float> quan);
	void smoothSurf();
	const Mesh to_mesh(string label, float alpha);
	void exportToObj(string output, string label, float alpha);

};


extern "C"
{
	inline Surf Surf_new(vector<Point> InputPoints, vector<bool> mask, vector<float> quan) { return Surf::createSurf(InputPoints, mask, quan); }
	inline void Surf_smooth(Surf surf) { surf.smoothSurf(); }
	inline const Mesh Surf_to_mesh(Surf surf, string label, float alpha) { return surf.to_mesh(label, alpha); }
	inline void Surf_exportToObj(Surf surf, string output, string label, float alpha) { surf.exportToObj(output, label, alpha); }
}

#endif
