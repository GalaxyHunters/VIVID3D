

#ifndef SURF_H
#define SURF_H


#include "Mesh.h"
#include "SurfFace.h"
#include "External.h"

#include <iostream>
#include <map>

using namespace std;

class CPointData_t { // used to sort and clean the voronoi input points
public:
	CPoint mPoint;
	cord_t mQuan;
	bool mIsIn;
	inline CPointData_t() {};
	inline CPointData_t(CPoint aPoint, cord_t aQuan, bool aIsIn) : mPoint(aPoint), mQuan(aQuan), mIsIn(aIsIn) {}
};

class CSurf{
private:
	vector<std::shared_ptr<CPoint> > mVecPoints;
	vector<CSurfFace> mVecFaces;
	vector<CPoint> mInputPoints; // for smooth
	vector<bool> mMask; //for smooth
	vector<cord_t> mQuan; // for smooth
 
	vector<cord_t> NormQuan(vector<cord_t> aQuan, cord_t aVMin, cord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax

	//vorn function:
	void RunVorn();
	void CleanFaces(vector<bool> aMask); // clean the unneeded faces(by mask)
	void CleanPoints(); // removes all the unused points
	void CleanEdges(); // cleans faces that are out of the box radius (happens as a result of too little points as input)
	void RemoveDoublePoints(); // remove all the double face points from the model

	//smooth functions:
	pair<vector<size_t>, vector<size_t> > SetPinPout();
	void UpdateInputPoints(vector<size_t> aPOut, vector<size_t> aPIn);
	void MakeMask(size_t aPOutSize, size_t aPInSize);
	pair<vector<size_t>, vector<size_t> > Stage2AddPoints(vector<size_t> aPOut, vector<size_t> aPIn);
	void AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<cord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2);
	pair<vector<size_t>, vector<size_t> > CleanDoublePointsVorn(vector<CPoint> aNewPoints, vector<cord_t> aNewQuan, vector<size_t> aNewIn, vector<size_t> aNewOut);
	vector<CPointData_t> RemoveDoublesVornInput(vector<CPointData_t> aData);

	CSurf();
	
public:	
	CSurf(const CSurf &surf); //copy constructor
	CSurf(vector<vector<double >> aInputPoints, vector<bool> aMask, vector<cord_t> aQuan, cord_t aVMin, cord_t aVMax);
	static vector<CSurf> CreateSurf(vector<CPoint> aInputPoints, vector<vector<bool> > aMask, vector<cord_t> aQuan, cord_t aVMin, cord_t aVMax);
	void SmoothSurf();
	const CMesh ToMesh(string aLabel, cord_t aAlpha);
	void ExportToObj(string aOutputFile, string aLabel, cord_t aAlpha);

	inline vector<CPoint> GetInputPoints() { return this->mInputPoints; }
	inline vector<bool> GetMask() { return this->mMask; }
	inline vector<cord_t> GetQuan() { return this->mQuan; }
	inline void SetInputPoints(vector<CPoint> aInputPoints) { this->mInputPoints = aInputPoints; }
	inline void SetMask(vector<bool> aMask) { this->mMask = aMask; }
	inline void SetQuan(vector<cord_t> aQuan) { this->mQuan = aQuan; }
	inline vector<shared_ptr<CPoint> > GetVecPoints() { return this->mVecPoints; }
	inline vector<CSurfFace> GetVecfaces() { return this->mVecFaces; }

};

#endif
	
