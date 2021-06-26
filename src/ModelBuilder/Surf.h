

#ifndef SURF_H
#define SURF_H

class CSurf;

#include "Mesh.h"
#include "SurfFace.h"
#include "../lib/External.h"

#include <iostream>
#include <map>

using namespace std;

class CPointData_t { // used to sort and clean the voronoi input points
public:
	CPoint mPoint;
	coord_t mQuan;
	bool mIsIn;
	inline CPointData_t() {};
	inline CPointData_t(CPoint aPoint, coord_t aQuan, bool aIsIn): mPoint(aPoint), mQuan(aQuan), mIsIn(aIsIn) {}
};





class CSurf{
private:
	vector<shared_ptr<CPoint> > mVecPoints;
	vector<CSurfFace> mVecFaces;
	vector<CPoint> mInputPoints; // for smooth
	vector<bool> mMask; //for smooth
	vector<coord_t> mQuan; // for smooth
	CPoint mCenVector; // holds the center of the data (used to center the data by 000 and back to original upon export)
 
	vector<coord_t>& NormQuan(vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax
    CPoint FindCenPoint(const vector<vector<double>> &aInputPoints); // find the center of the model (used to transform the data to be centered around 000)

	//vorn function:
	void RunVorn();
	void CleanFaces(vector<bool>& aMask); // clean the unneeded faces(by mask)
	void CleanPoints(); // removes all the unused points
	void CleanEdges(); // cleans faces that are out of the box radius (happens as a result of too little points as input)
	void RemoveDoublePoints(); // remove all the double face points from the model

	//smooth functions:
	void SetPinPout(vector<size_t>& arPOut, vector<size_t>& arPIn);
	void UpdateInputPoints(vector<size_t>& arPOut, vector<size_t>& arPIn);
	void MakeMask(size_t aPOutSize, size_t aPInSize);
	void Stage2AddPoints(vector<size_t>& arPOut, vector<size_t>& arPIn);
	void AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<coord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2);
	void CleanDoublePointsVorn(vector<CPoint>& arNewPoints, vector<coord_t>& arNewQuan, vector<size_t>& arNewIn, vector<size_t>& arNewOut);
	vector<CPointData_t> RemoveDoublesVornInput(vector<CPointData_t>& arData);

	CSurf();
	
public:	
	CSurf(const CSurf &surf); //copy constructor
	CSurf(vector<vector<double >> aInputPoints, vector<bool> aMask, vector<coord_t> aQuan, coord_t aVMin, coord_t aVMax); //TODO should be const and by ref, why vector<vector<double >> instead of CPOINTS?
	void SmoothSurf();
	const CMesh ToMesh(string aLabel, coord_t aAlpha); // TODO: why const?
	void ExportToObj(string aOutputFile, string aLabel, coord_t aAlpha);

	inline vector<CPoint>& GetInputPoints() { return mInputPoints; }
	inline vector<bool>& GetMask() { return mMask; }
	inline vector<coord_t>& GetQuan() { return mQuan; }
	inline void SetInputPoints(vector<CPoint>& aInputPoints) { mInputPoints = aInputPoints; }
	inline void SetMask(vector<bool>& aMask) { mMask = aMask; }
	inline void SetQuan(vector<coord_t>& aQuan) { mQuan = aQuan; }
	inline vector<shared_ptr<CPoint> >& GetVecPoints() { return mVecPoints; }
	inline vector<CSurfFace>& GetVecfaces() { return mVecFaces; }

};

#endif
	
