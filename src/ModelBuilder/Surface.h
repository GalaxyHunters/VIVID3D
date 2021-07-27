#ifndef VIVID_SURFACE_H
#define VIVID_SURFACE_H

#include "Voronoi.h"
#include "Mesh.h"
#include "LogFile.h"
#include <memory>

namespace vivid
{
//TODO Color field needs to be changed everywhere
//TODO Typedef has to be by const ref
//TODO Should it containd Cpoint?
class CSurfacePoint
{ // used to sort and clean the mVoronoi input points
public:
    CPoint mPoint = {};
    coord_t mQuan = 0;
    bool mIsIn = false;

    CSurfacePoint() {};
    CSurfacePoint(CPoint aPoint, coord_t aQuan, bool aIsIn): mPoint(aPoint), mQuan(aQuan), mIsIn(aIsIn) {}
};

//TODO Should it containd CFace?
class CSurfaceFace
{
public:
    std::vector<std::shared_ptr<CPoint> > mPoints = {};
    std::pair<size_t, size_t> mPairPoints = {};
    coord_t mColor = 0;

    CSurfaceFace(const std::vector<std::shared_ptr<CPoint>> &arPoints, coord_t aColor, const std::pair<size_t, size_t> &arPairPoints) :
            mPoints(arPoints), mColor(aColor), mPairPoints(arPairPoints){};
    CSurfaceFace() {};
    ~CSurfaceFace() {};
};

//public CMesh
class CSurface : public CMesh
{
private:
    static void CallBack(const vivid::CLogFile::ELogType a)
    {
        CLogFile::GetInstance()->WriteToLog(a);
    }
    CLogFile::LogCallBackFunction mLogFile = CallBack;

    CVoronoi mVoronoi;                       // CVoronoi contains the ComputeVoronoi and other functions for interfacing with Elad

    std::vector<std::shared_ptr<CPoint> > mVecPoints = {};
    std::vector<CSurfaceFace> mVecFaces = {};

    std::vector<std::vector<size_t>> mPointsInFaces = {}; // For mapping which faces contain which points

    std::vector<CPoint> mInputPoints = {};   // for smooth
    std::vector<bool> mMask = {};            // for smooth
    std::vector<coord_t> mQuan = {};         // for smooth
    CPoint mCenVector = {};                  // holds the center of the data (used to center the data by 000 and back to original upon export)
    std::pair<CPoint, CPoint> mBoxPair = {}; // Holds min and max boxPoints for ComputeVoronoi
    coord_t mScale = 0;                      // holds value for scaling to original scale upon export to mesh

    // Centralization Sub-Methods
    coord_t FindContainingRadius();          // Used for Scaling
    std::vector<CPoint> FindContainingBox(); // Find Box dimensions for RunVorn.

    // Handle Input Sub-Methods
    // TODO: Fix CleanDoubleInputPoints
    void CleanDoubleInputPoints();           // remove all the double input points
    void PreProcessPoints();                 // Centering, scaling, adding noise.
    std::vector<coord_t>& NormQuan(std::vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax

    //vorn function:
    void RunVorn();

    // Cleaning Sub-Methods
    void CleanFaces();        // clean the unneeded faces(by mMask)
    void CleanPoints();       // removes all the unused points
    void CleanEdges();        // cleans faces that are out of the box radius (happens as a result of too little points as input)
    void CleanDoublePoints(); // remove all the double face points from the model

    // Smoothing Sub-Methods
    // Part 1
    void SetPinPout(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    void UpdateInputPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn);
    // Part 2
    void UpdatePoutPin(std::vector<size_t>& aPOut, std::vector<size_t>& aPIn);
    void Stage2ModifyPoints(vector<size_t>& arPOut, vector<size_t>& arPIn);
    void FindPairPoints(size_t aCPoint1, size_t aCPoint2, std::vector<size_t> &arPIn, std::vector<size_t> &arPOut, size_t aPOutSize, size_t aPInSize,
                        CSurfaceFace &arFace, std::vector<CPoint> &arNewPoints, std::vector<coord_t> &arNewQuan, size_t &arIndex);
    void AddPointsAlt(std::vector<size_t> &arPVec, std::vector<CPoint> &arNewPoints, std::vector<coord_t> &arNewQuan,
                      size_t &arNewIndex, size_t aCPoint1, size_t aCPoint2, size_t aCPoint3); // Adds points between every pair by aSmoothFactor
    void Stage2AddPoints(std::vector<size_t>& arPOut, std::vector<size_t>& arPIn, int aSmoothFactor);
    void AddPoints(std::vector<size_t> * apPVec, std::vector<CPoint> * apNewPoints, std::vector<coord_t> * apNewQuan,
                   size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2, int aSmoothFactor); // Adds points between every pair by aSmoothFactor
    void CleanDoublePointsVorn(std::vector<CPoint>& arNewPoints, std::vector<coord_t>& arNewQuan,
                               std::vector<size_t>& arNewIn, std::vector<size_t>& arNewOut); // Handles the reallocation after RemoveDoublesVornInput
    std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);   // Called by CleanDoublePointsVorn
    // Part 3
    void MakeMask(size_t aPOutSize, size_t aPInSize);

public:
    /**
     * CSurface Constructor
     * @param[in] arInputPoints the input point data in x,y,z form.
     * @param[in] arMask a boolean mask of true and false points
     * @param[in] arQuan a vector containing the quantity of each point
     * @param[in] aVMin the minimum value in arQuan, anything below will be set to aVMin
     * @param[in] aVMin the maximum value in arQuan, anything below will be set to aVMax
     */
    CSurface(const std::vector<std::vector<double >> &arInputPoints, const std::vector<bool> &arMask,
             std::vector<coord_t> &arQuan, coord_t aVMin, coord_t aVMax);
    /**
     * CSurface Copy-Constructor
     */
    CSurface(const CSurface &surf);
    // operator =

    /**
     * Create the surfaces using the input data
     */
    void CreateSurface();

    /**
     * Smooth method
     * @param[in] aSuperSmooth boolean value for activating SuperSmooth Algorithm
     * @param[in] aSmoothFactor An integer value between 1 and 8
     */
    void Smooth(bool aSuperSmooth = false, int aSmoothFactor = 2);

    /**
     * Convert the CSurface object to CMesh object
     * @param[in] aLabel the label to assign to the new mesh
     * @param[in] aAlpha the alpha to assign to the new mesh
     * @returns CMesh converted mesh
     */
    const CMesh ToMesh(string aLabel, coord_t aAlpha); // TODO: When inheritance from mesh, this wont be needed because it will always become mesh

    void VecCSurf();

    // Getters, Setters
    // TODO: which gets do we really need and why?
    inline const std::vector<CPoint>& GetInputPoints() { return mInputPoints; }
    inline const std::vector<bool>& GetMask() { return mMask; }
    inline const std::vector<coord_t>& GetQuan() { return mQuan; }
    inline const std::vector<std::shared_ptr<CPoint> >& GetVecPoints() { return mVecPoints; }
    inline const std::vector<CSurfaceFace>& GetVecfaces() { return mVecFaces; }

    inline void SetInputPoints(const std::vector<CPoint> &arInputPoints) { mInputPoints = arInputPoints; }
    inline void SetMask(const std::vector<bool> &arMask) { mMask = arMask; }
    inline void SetQuan(std::vector<coord_t> &arQuan) { mQuan = arQuan; }
};

} // namespace vivid
#endif //VIVID_SURFACE_H
