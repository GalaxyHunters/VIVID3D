

#ifndef SURF_H
#define SURF_H


#include "Mesh.h"
#include "SurfFace.h"
#include "Utils.h"

#include <iostream>
#include <map>
using namespace std;

class Point_data_t { // used to sort and clean the voronoi input points
public:
	Point point;
	float quan;
	bool isIn;
	inline Point_data_t() {};
	inline Point_data_t(Point point, float quan, bool isIn) : point(point), quan(quan), isIn(isIn) {}
};

class Surf{
private:
	vector<std::shared_ptr<Point>> vecPoints;
	vector<SurfFace> vecFaces;
	vector<Point> inputPoints; // for smooth
	vector<bool> mask; //for smooth
	vector<float> quan; // for smooth
 
	vector<float> normQuan(vector<float> quan, float Vmin, float Vmax); // normalize the values to be between 0 and 1, uses Vmin and Vmax

	//vorn function:
	void runVorn();
	void cleanFaces(vector<bool> mask); // clean the unneaded faces(by mask)
	void cleanPoints(); // removes all the unused points
	void cleanEdges(); // cleans faces that are out of the box radius (happens as a result of two little points as input)
	void RemoveDoublePoints(); // remove all the double face points from the model

	//smooth functions:
	pair<vector<size_t>, vector<size_t>> _setPinPout();
	void _updateInputPoints(vector<size_t> Pout, vector<size_t> Pin);
	void _makeMask(size_t PoutSize, size_t PinSize);
	pair<vector<size_t>, vector<size_t>> _S2addPoints(vector<size_t> Pout, vector<size_t> Pin);
	vector<Point_data_t> RemoveDoublesVornIn(vector<Point_data_t> data);

	Surf();
	
public:	
	Surf(const Surf &surf);
	static Surf createSurf(vector<Point> InputPoints, vector<bool> mask, vector<float> quan, float Vmin, float Vmax);
	static vector<Surf> createSurf(vector<Point> InputPoints, vector<vector<bool>> mask, vector<float> quan, float Vmin, float Vmax);
	void smoothSurf();
	const Mesh to_mesh(string label, float alpha);
	void exportToObj(string output, string label, float alpha);

	inline vector<Point> getInputPoints() { return this->inputPoints; }
	inline vector<bool> getMask() { return this->mask; }
	inline vector<float> getQuan() { return this->quan; }
	inline void setInputPoints(vector<Point> inputPoints) { this->inputPoints = inputPoints; }
	inline void setMask(vector<bool> mask) { this->mask = mask; }
	inline void setQuan(vector<float> quan) { this->quan = quan; }
	inline vector<shared_ptr<Point>> getVecPoints() { return this->vecPoints; }
	inline vector<SurfFace> getVecfaces() { return this->vecFaces; }

};

extern "C"
{
	inline Surf Surf_new(vector<Point> InputPoints, vector<bool> mask, vector<float> quan, float Vmin, float Vmax) { return Surf::createSurf(InputPoints, mask, quan, Vmin, Vmax); }
	inline void Surf_smooth(Surf surf) { surf.smoothSurf(); }
	inline const Mesh Surf_to_mesh(Surf surf, string label, float alpha) { return surf.to_mesh(label, alpha); }
	inline void Surf_exportToObj(Surf surf, string output, string label, float alpha) { surf.exportToObj(output, label, alpha); }
}

#endif
	
