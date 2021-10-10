#ifndef VIVID_SURFACE_H
#define VIVID_SURFACE_H

#include "Voronoi.h"
#include "Mesh.h"
#include "LogFile.h"
#include <memory>

namespace vivid
{
//TODO **explanation**

//TODO Typedef has to be by const ref
    struct CSurfacePoint
    { // used to sort and clean the mVoronoi input points
        CPoint mPoint = {};
        quan_t mQuan = 0;
        bool mMaskIsTrue = false;

        CSurfacePoint() {};
        CSurfacePoint(const CPoint& arPoint, quan_t aQuan, bool aIsIn): mPoint(arPoint), mQuan(aQuan), mMaskIsTrue(aIsIn) {}
    };

// For the cleaning stage
    struct CSurfaceFace
    {
    public:
        std::vector<std::shared_ptr<CPoint> > mVertices = {};
        std::pair<size_t, size_t> mPairPoints = {};
        quan_t mColor = 0;

        CSurfaceFace(const std::vector<std::shared_ptr<CPoint>> &arPoints, quan_t aColor, const std::pair<size_t, size_t> &arPairPoints) :
                mVertices(arPoints), mColor(aColor), mPairPoints(arPairPoints){};
        CSurfaceFace() {};
        ~CSurfaceFace() {};
    };

//public CMesh
    class CSurface
    {
    private:
        CVoronoi mVoronoi;                       // CVoronoi class for running Voronoi Surfacing algorithm

        // Input Data:
        std::vector<CPoint> mInputPoints = {};
        std::vector<bool> mCreationMask = {};
        std::vector<quan_t> mQuan = {};

        // PreProcessing Data
        std::pair<CPoint, CPoint> mBoxPair = {}; // Holds min and max boxPoints for ComputeVoronoi
        CPoint mCenVector = {};                  // holds the center of the data (used to center the data by 000 and back to original upon export)
        coord_t mScale = 0;                      // holds value for scaling to original scale upon export to mesh

        // Voronoi Data
        std::vector<std::shared_ptr<CPoint> > mVertices = {};
        std::vector<CSurfaceFace> mSurfFaces = {};


        // Centralization Sub-Methods
        coord_t FindContainingRadius();          // Used for Scaling
        std::vector<CPoint> FindContainingBox(); // Find Box dimensions for RunVorn.

        // Handle Input Sub-Methods
        void PreProcessPoints(vector<CSurfacePoint> &arPoints);                 // Centering, scaling, adding noise.
        void CleanDoubleInputPoints(vector<CSurfacePoint> &arPoints);           // remove all the double input points

        // TODO: Should be in util, even better should be typedef with min 0. and max 1.
        std::vector<coord_t>& NormQuan(std::vector<quan_t>& arQuan, quan_t aVMin, quan_t aVMax); // normalize the values to be between 0 and 1, uses Vmin and Vmax

        //vorn function:
        void RunVorn();

        // Cleaning Sub-Methods
        void CleanFaces();        // clean the unneeded faces(by mCreationMask)
        void CleanPoints();       // removes all the unused points
        void CleanEdges();        // cleans faces that are out of the box radius (happens as a result of too little points as input)
        void CleanDoublePoints(); // remove all the double face points from the model

    public:
        /**
         * CSurface Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] arMask a boolean mask of true and false points
         * @param[in] arQuan a vector containing the quantity of each point
         * @param[in] aVMin the minimum value in arQuan, anything below will be set to aVMin
         * @param[in] aVMin the maximum value in arQuan, anything below will be set to aVMax
         */
        CSurface(const std::vector<CPoint> &arInputPoints, const std::vector<bool> &arMask,
                 std::vector<quan_t> &arQuan, quan_t aVMin, quan_t aVMax);
        /**
         * CSurface Copy-Constructor
         */
        CSurface(const CSurface &surf);
        // operator =

        /**
         * Create the surfaces using the input data
         */
        void CreateSurface(bool aPostProcessing = true);

        /**
         * Convert the CSurface object to CMesh object
         * @param[in] arLabel the label to assign to the new mesh
         * @param[in] aAlpha the alpha to assign to the new mesh
         * @returns CMesh converted mesh
         */
        CMesh ToMesh(const string& arLabel, coord_t aAlpha) const;

        std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);

        // Getters, Setters
        // Used primarily in deprecated smoothing
        inline const std::vector<CPoint>& GetInputPoints() const { return mInputPoints; }
        inline const std::vector<bool>& GetMask() { return mCreationMask; }
        inline const std::vector<quan_t>& GetQuan() { return mQuan; }
        inline const std::vector<CSurfaceFace>& GetFaces() const { return mSurfFaces; }
        inline const CVoronoi& GetVoronoiData() const { return mVoronoi; }

        inline void SetInputPoints(const std::vector<CPoint> &arPoints) { mInputPoints = arPoints; }
        inline void SetMask(const std::vector<bool> &arMask) { mCreationMask = arMask; }
        inline void SetQuan(const std::vector<quan_t> &arQuan) { mQuan = arQuan; }


        void WhatAreTheseThingsElad() {
            cout << mVoronoi.mData.GetAllVolumes().size() << endl;
            cout << mVoronoi.mData.GetAllArea().size() << endl;
            cout << mVoronoi.mData.GetAllCM().size() << endl;
            cout << mVoronoi.mData.GetAllCellFaces().size() << endl;
            cout << mVoronoi.mData.GetPointNo() << endl;
            // Useless data:
            cout << mVoronoi.mData.GetSelfIndex().size() << endl;
            cout << mVoronoi.mData.GetSentProcs().size() << endl;
            cout << mVoronoi.mData.GetSentPoints().size() << endl;
        } //?


    };

} // namespace vivid
#endif //VIVID_SURFACE_H