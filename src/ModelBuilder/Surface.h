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
    CVoronoi mVoronoi;

    std::vector<std::shared_ptr<CPoint> > mVecPoints = {};
    std::vector<CSurfaceFace> mVecFaces = {};

    std::vector<CPoint> mInputPoints = {}; // for smooth
    std::vector<bool> mMask = {}; //for smooth
    std::vector<coord_t> mQuan = {}; // for smooth
    CPoint mCenVector = {}; // holds the center of the data (used to center the data by 000 and back to original upon export)

    // TODO: Organize with same order as in .cpp
    // Centralization Sub-Methods
    CPoint GetGeometricCenter(const std::vector<std::vector<double>> &arInputPoints); // find the center of the model (used to transform the data to be centered around 000)
    double FindContainingRadius(const vector<CPoint>& arPoints); // TODO: Remnant, should be deleted.
    CPoint FindContainingBox(const vector<CPoint>& arPoints); // Find Box dimensions for Voronoi.

    // Handle Input Sub-Methods
    void PreProcessPoints(); // Centering, scaling, adding noise.
    std::vector<coord_t>& NormQuan(std::vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax
    void ScaleParticles(); // Rescales particles to scale __ - __
    void AddParticleNoise();

    // TODO: Add scale_down mesh.
    // TODO: AddNoise function for points. How do we decide if we need to or not? How much noise?
    // TODO: Voronoi shouldn't be in constructor, and the functions run always after runvorn should be placed into runvorn.
    // Response: Voronoi doesn't always run clean funcs directly after (as in smooth) so I create a seperate CreateSurface() method to handle it in the public.
    //vorn function:
    void Voronoi();

    // Cleaning Sub-Methods
    void CleanFaces(std::vector<bool>& aMask); // clean the unneeded faces(by mask)
    void CleanPoints(); // removes all the unused points (TODO (Tomer asks): clean points that are not on rlevant faces?)
    void CleanEdges(); // cleans faces that are out of the box radius (happens as a result of too little points as input)
    void CleanDoublePoints(); // remove all the double face points from the model

    // TODO: What the hell is this??
    // Smoothing Sub-Methods
    void SetPinPout(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void UpdatePoutPin(std::vector<size_t>& aPOut, std::vector<size_t>& aPIn);
    void UpdateInputPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void MakeMask(size_t aPOutSize, size_t aPInSize);
    void Stage2AddPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void AddPoints(std::vector<size_t> * apPVec, std::vector<CPoint> * apNewPoints, std::vector<coord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2);

    void CleanDoublePointsVorn(std::vector<CPoint>& arNewPoints, std::vector<coord_t>& arNewQuan, std::vector<size_t>& arNewIn, std::vector<size_t>& arNewOut);
    std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);

public:
    CSurface(const CSurface &surf); //copy constructor
    // operator =
    CSurface(std::vector<std::vector<double >> aInputPoints, std::vector<bool> aMask, std::vector<coord_t> aQuan, coord_t aVMin, coord_t aVMax); //TODO should be const and by ref, why vector<vector<double >> instead of CPOINTS?

    void CreateSurface(); // Runs Voronoi plus the other cleaning sub-methods
    void Smooth();           // Runs Voronoi plus the other smoothing and cleaning sub-methods
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
