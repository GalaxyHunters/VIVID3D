#ifndef VIVID_SURFACE_H
#define VIVID_SURFACE_H

#include "Voronoi.h"
#include "Mesh.h"
#include <memory>

namespace vivid
{
//TODO Color field needs to be changed everywhere
//TODO Typedef has to be by const ref
//TODO Should it containd Cpoint?
class CSurfacePoint { // used to sort and clean the mVoronoi input points
public:
    CPoint mPoint = {0, 0, 0};
    coord_t mQuan = 0;
    bool mIsIn = false;

    CSurfacePoint() {};
    CSurfacePoint(CPoint aPoint, coord_t aQuan, bool aIsIn): mPoint(aPoint), mQuan(aQuan), mIsIn(aIsIn) {}
};

//TODO Should it containd CFace?
class CSurfaceFace{
public:
    std::vector<std::shared_ptr<CPoint> > mPoints = {};
    std::pair<size_t, size_t> mPairPoints = {0,0};
    coord_t mColor = 0;

    CSurfaceFace(const std::vector<std::shared_ptr<CPoint>> &arPoints, coord_t aColor, const std::pair<size_t, size_t> &arPairPoints) :
            mPoints(arPoints), mColor(aColor), mPairPoints(arPairPoints){};
    CSurfaceFace() {};
    ~CSurfaceFace() {};
};

//public CMesh
class CSurface : public CMesh {
private:
    CVoronoi mVoronoi;                       // CVoronoi contains the ComputeVoronoi and other functions for interfacing with Elad

    std::vector<std::shared_ptr<CPoint> > mVecPoints = {};
    std::vector<CSurfaceFace> mVecFaces = {};

    std::vector<CPoint> mInputPoints = {};   // for smooth
    std::vector<bool> mMask = {};            // for smooth
    std::vector<coord_t> mQuan = {};         // for smooth
    CPoint mCenVector = {};                  // holds the center of the data (used to center the data by 000 and back to original upon export)
    std::pair<CPoint, CPoint> mBoxPair = {}; // Holds min and max boxPoints for ComputeVoronoi
    coord_t mScale = 0;                      // holds value for scaling to original scale upon export to mesh


    // TODO: Organize with same order as in .cpp
    // Centralization Sub-Methods
    coord_t FindContainingRadius();          // Used for Scaling
    std::vector<CPoint> FindContainingBox(); // Find Box dimensions for RunVorn.

    // Handle Input Sub-Methods
    void CleanDoubleInputPoints();           // remove all the double input points
    void PreProcessPoints();                 // Centering, scaling, adding noise.
    std::vector<coord_t>& NormQuan(std::vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax

    //vorn function:
    void RunVorn();

    // Cleaning Sub-Methods
    void CleanFaces(); // clean the unneeded faces(by mMask)
    void CleanPoints(); // removes all the unused points
    void CleanEdges(); // cleans faces that are out of the box radius (happens as a result of too little points as input)
    void CleanDoublePoints(); // remove all the double face points from the model

    // Smoothing Sub-Methods
    // Part 1
    void SetPinPout(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void UpdateInputPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    // Part 2
    void UpdatePoutPin(std::vector<size_t>& aPOut, std::vector<size_t>& aPIn);
    void Stage2AddPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void AddPoints(std::vector<size_t> * apPVec, std::vector<CPoint> * apNewPoints, std::vector<coord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2);
    void CleanDoublePointsVorn(std::vector<CPoint>& arNewPoints, std::vector<coord_t>& arNewQuan, std::vector<size_t>& arNewIn, std::vector<size_t>& arNewOut);
    std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);
    // Part 3
    void MakeMask(size_t aPOutSize, size_t aPInSize);

public:
    CSurface(const CSurface &surf); //copy constructor
    // operator =
    CSurface(std::vector<std::vector<double >> aInputPoints, std::vector<bool> aMask, std::vector<coord_t> aQuan, coord_t aVMin, coord_t aVMax); //TODO should be const and by ref, why vector<vector<double >> instead of CPOINTS?

    void CreateSurface(); // Runs RunVorn plus the other cleaning sub-methods
    void Smooth();           // Runs RunVorn plus the other smoothing and cleaning sub-methods
    const CMesh ToMesh(string aLabel, coord_t aAlpha); // TODO: When inheritance from mesh, this wont be needed because it will always become mesh


    // TODO: which gets do we really need and why?
    inline std::vector<CPoint>& GetInputPoints() { return mInputPoints; }
    inline std::vector<bool>& GetMask() { return mMask; }
    inline std::vector<coord_t>& GetQuan() { return mQuan; }
    inline std::vector<std::shared_ptr<CPoint> >& GetVecPoints() { return mVecPoints; }
    inline std::vector<CSurfaceFace>& GetVecfaces() { return mVecFaces; }

    inline void SetInputPoints(std::vector<CPoint>& aInputPoints) { mInputPoints = aInputPoints; }
    inline void SetMask(std::vector<bool>& aMask) { mMask = aMask; }
    inline void SetQuan(std::vector<coord_t>& aQuan) { mQuan = aQuan; }
};

} // namespace vivid
#endif //VIVID_SURFACE_H
