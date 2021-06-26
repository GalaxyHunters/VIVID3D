#ifndef SURF_H
#define SURF_H

#include "Mesh.h"
#include <memory>

namespace vivid
{

class CSurfacePoint { // used to sort and clean the voronoi input points
public:
	CPoint mPoint;
	coord_t mQuan;
	bool mIsIn;
	inline CSurfacePoint() {};
	inline CSurfacePoint(CPoint aPoint, coord_t aQuan, bool aIsIn): mPoint(aPoint), mQuan(aQuan), mIsIn(aIsIn) {}
};


class CSurfaceFace{
public:
    std::vector<std::shared_ptr<CPoint> > mPoints;
    std::pair<size_t, size_t> mPairPoints;
    coord_t mColor;

    inline CSurfaceFace(std::vector<std::shared_ptr<CPoint>> pPoints, coord_t pColor, std::pair<size_t, size_t> pPairPoints) :
            mPoints(pPoints), mColor(pColor), mPairPoints(pPairPoints){};
    inline CSurfaceFace() {};
    inline ~CSurfaceFace() {};
};


class CSurface{
private:
    std::vector<std::shared_ptr<CPoint> > mVecPoints;
    std::vector<CSurfaceFace> mVecFaces;
    std::vector<CPoint> mInputPoints; // for smooth
    std::vector<bool> mMask; //for smooth
    std::vector<coord_t> mQuan; // for smooth
	CPoint mCenVector; // holds the center of the data (used to center the data by 000 and back to original upon export)

    std::vector<coord_t>& NormQuan(std::vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax
    CPoint FindCenPoint(const std::vector<std::vector<double>> &aInputPoints); // find the center of the model (used to transform the data to be centered around 000)

	//vorn function:
	void RunVorn();
	void CleanFaces(std::vector<bool>& aMask); // clean the unneeded faces(by mask)
	void CleanPoints(); // removes all the unused points
	void CleanEdges(); // cleans faces that are out of the box radius (happens as a result of too little points as input)
	void RemoveDoublePoints(); // remove all the double face points from the model

	//smooth functions:
	void SetPinPout(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
	void UpdateInputPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
	void MakeMask(size_t aPOutSize, size_t aPInSize);
	void Stage2AddPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
	void AddPoints(std::vector<size_t> * apPVec, std::vector<CPoint> * apNewPoints, std::vector<coord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2);
	void CleanDoublePointsVorn(std::vector<CPoint>& arNewPoints, std::vector<coord_t>& arNewQuan, std::vector<size_t>& arNewIn, std::vector<size_t>& arNewOut);
    std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);

	CSurface();
	
public:	
	CSurface(const CSurface &surf); //copy constructor
	CSurface(std::vector<vector<double >> aInputPoints, std::vector<bool> aMask, std::vector<coord_t> aQuan, coord_t aVMin, coord_t aVMax); //TODO should be const and by ref, why vector<vector<double >> instead of CPOINTS?
	void SmoothSurf();
	const CMesh ToMesh(string aLabel, coord_t aAlpha); // TODO: why const?
	void ExportToObj(string aOutputFile, string aLabel, coord_t aAlpha);

	inline std::vector<CPoint>& GetInputPoints() { return mInputPoints; }
	inline std::vector<bool>& GetMask() { return mMask; }
	inline std::vector<coord_t>& GetQuan() { return mQuan; }
	inline void SetInputPoints(std::vector<CPoint>& aInputPoints) { mInputPoints = aInputPoints; }
	inline void SetMask(std::vector<bool>& aMask) { mMask = aMask; }
	inline void SetQuan(std::vector<coord_t>& aQuan) { mQuan = aQuan; }
	inline std::vector<std::shared_ptr<CPoint> >& GetVecPoints() { return mVecPoints; }
	inline std::vector<CSurfaceFace>& GetVecfaces() { return mVecFaces; }

};

} // namespace vivid
#endif
	
