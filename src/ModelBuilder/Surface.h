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
        coord_t UVcoord = 0;
        bool mMaskIsTrue = false;

        CSurfacePoint() {};
        CSurfacePoint(const CPoint& arPoint, coord_t aUVcoord, bool aIsIn): mPoint(arPoint), UVcoord(aUVcoord), mMaskIsTrue(aIsIn) {}
        inline bool operator < (const CSurfacePoint &arObj) {return mPoint < arObj.mPoint;}
    };

// For the cleaning stage
    struct CSurfaceFace
    {
    public:
        std::vector<std::shared_ptr<CPoint> > mVertices = {};
        std::pair<size_t, size_t> mPairPoints = {};
        coord_t mUVcoord = 0;

        CSurfaceFace(const std::vector<std::shared_ptr<CPoint>> &arPoints, coord_t aColorField, const std::pair<size_t, size_t> &arPairPoints) :
                mVertices(arPoints), mUVcoord(aColorField), mPairPoints(arPairPoints){};
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
        std::vector<normal_float> mUVcoords = {};

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
        void PreProcessPoints(vector<CSurfacePoint> &arPoints, coord_t aNoiseDisplacement);   // Centering, scaling, adding noise.
        void CleanDoubleInputPoints(vector<CSurfacePoint> &arPoints);           // remove all the double input points

        //vorn function:
        void RunVorn();

        // Cleaning Sub-Methods
        void CleanFaces();        // clean the unneeded faces(by mCreationMask)
        vector<CSurfaceFace> TriangulizeFace(const CSurfaceFace &arFace);
        void NormalizeFace(CSurfaceFace& arTriangleFace); // normalize face to face correct direction
        void CleanPoints();       // removes all the unused points
        void CleanEdges();        // cleans faces that are out of the box radius (happens as a result of too little points as input)
        void CleanDoublePoints(); // remove all the double face points from the model

    public:
        /**
         * CSurface Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] arMask a boolean mask of true and false points
         * @param[in] arColorField a vector containing the color field of each point
         * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
         * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
         * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
         */
        CSurface(const std::vector<CPoint> &arInputPoints, const std::vector<bool> &arMask,
                 std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, coord_t aNoiseDisplacement = 0.001);
        /**
         * CSurface Copy-Constructor
         */
        CSurface(const CSurface &surf);
        /**
         * Create the surfaces using the input data
         */
        void CreateSurface();

        /**
         * Convert the CSurface object to CMesh object
         * @param[in] arLabel the label to assign to the new mesh
         * @param[in] aAlpha the alpha to assign to the new mesh
         * @returns CMesh converted mesh
         */
        CMesh ToMesh(const string& arLabel, normal_float aOpacity) const;

        std::vector<CSurfacePoint> RemoveDoublesVornInput(std::vector<CSurfacePoint>& arData);

        // Getters, Setters
        // Used primarily in deprecated smoothing
        inline const std::vector<CPoint>& GetInputPoints() const { return mInputPoints; }
        inline const std::vector<bool>& GetMask() { return mCreationMask; }
        inline const std::vector<normal_float>& GetUVcoords() { return mUVcoords; }
        inline const std::vector<CSurfaceFace>& GetFaces() const { return mSurfFaces; }
        inline const CVoronoi& GetVoronoiData() const { return mVoronoi; }

        inline void SetInputPoints(const std::vector<CPoint> &arPoints) { mInputPoints = arPoints; }
        inline void SetMask(const std::vector<bool> &arMask) { mCreationMask = arMask; }
        inline void SetUVcoords(const std::vector<normal_float> &arUVcoords) { mUVcoords = arUVcoords; }
    };

} // namespace vivid
#endif //VIVID_SURFACE_H