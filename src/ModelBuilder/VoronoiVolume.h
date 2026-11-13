#ifndef VIVID_VORONOIVOLUME_H
#define VIVID_VORONOIVOLUME_H

#include "Voronoi.h"
#include "Mesh.h"
#include "LogFile.h"
#include <memory>
#include <cstdint>
#include <map>

namespace vivid
{
    struct CSurfacePoint
    { // used to sort and clean the mVoronoi input points
        CPoint mPoint = {};
        normal_float UVcoord = 0;
        bool mMaskIsTrue = false;

        CSurfacePoint() {};
        CSurfacePoint(const CPoint& arPoint, normal_float aUVcoord, bool aIsIn): mPoint(arPoint), UVcoord(aUVcoord), mMaskIsTrue(aIsIn) {}
        inline bool operator < (const CSurfacePoint &arObj) const {return mPoint < arObj.mPoint;}
    };

    // All faces of the Voronoi volume in flat arrays (no allocation per face, no per-vertex objects).
    // Face f owns the vertex ids mIndices[mOffsets[f] .. mOffsets[f+1]) and lies between the two input points mPairs[f].
    struct CSurfaceFaces
    {
        std::vector<uint32_t> mOffsets = {0};
        std::vector<uint32_t> mIndices = {};
        std::vector<std::pair<uint32_t, uint32_t>> mPairs = {};
        std::vector<coord_t> mUVcoords = {};

        inline size_t Size() const { return mPairs.size(); }
        inline size_t VertexCount(size_t aFace) const { return mOffsets[aFace + 1] - mOffsets[aFace]; }
        inline const uint32_t* Vertices(size_t aFace) const { return mIndices.data() + mOffsets[aFace]; }
        inline void Clear() { mOffsets.assign(1, 0); mIndices.clear(); mPairs.clear(); mUVcoords.clear(); }
    };

    class CVoronoiVolume
    {
    private:
        CVoronoi mVoronoi;                       // CVoronoi class for running Voronoi Surfacing algorithm

        // Input Data:
        std::vector<CPoint> mInputPoints = {};
        std::vector<size_t> mInputOutputMap = {}; // deduplicated point index -> index in the caller's input array
        size_t mNumCallerPoints = 0;              // size of the caller's input array (the mask must have this size)
        std::vector<bool> mCreationMask = {};
        std::vector<normal_float> mUVcoords = {};

        // PreProcessing Data
        std::pair<CPoint, CPoint> mBoxPair = {}; // Holds min and max boxPoints for ComputeVoronoi
        CPoint mCenVector = {};                  // holds the center of the data (used to center the data by 000 and back to original upon export)
        coord_t mScale = 0;                      // holds value for scaling to original scale upon export to mesh

        // Voronoi Data
        std::vector<CPoint> mVertices = {};      // Voronoi cell corners (vertex ids index this array)
        CSurfaceFaces mSurfFaces = {};


        // Centralization Sub-Methods
        coord_t FindContainingRadius();          // Used for Scaling
        std::vector<CPoint> FindContainingBox(); // Find Box dimensions for RunVorn.

        // Handle Input Sub-Methods
        void PreProcessPoints(std::vector<pair<CSurfacePoint, size_t>> &arPoints, coord_t aNoiseDisplacement);   // Centering, scaling, adding noise.
        void CleanDoubleInputPoints(std::vector<pair<CSurfacePoint, size_t>> &arPoints); // remove all the double input points

        /**
         * Create the surfaces using the input data
         */
        void CreateSurface();
        //vorn function:
        void RunVorn();

        // Cleaning Sub-Methods
        void CleanEdges();        // cleans faces that are out of the box radius (happens as a result of too little points as input)
    public:
        /**
         * CVoronoiVolume Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] arColorField a vector containing the color field of each point
         * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
         * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
         * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
         */
        CVoronoiVolume(const std::vector<CPoint> &arInputPoints, std::vector<normal_float> &arColorField,
                       normal_float aVMin = 0, normal_float aVMax = 0, coord_t aNoiseDisplacement = 0);
        /**
         * CVoronoiVolume Copy-Constructor
         */
        CVoronoiVolume(const CVoronoiVolume &surf);

        /**
         * Extract the iso-surface between the true and the false points of a mask as a CMesh.
         * Read-only: the volume is not copied or changed, so several masks can be extracted from one volume
         * (even concurrently). The cost depends on the size of the surface, not of the volume.
         * @param[in] arMask a boolean mask of true and false points, one entry per point given to the constructor
         * @param[in] arLabel the label to assign to the new mesh
         * @param[in] aOpacity the opacity to assign to the new mesh
         * @returns CMesh converted mesh
         */
        CMesh MaskMesh(const std::vector<bool> &arMask, const string& arLabel, normal_float aOpacity) const;
        /** Convert every face of the volume (no mask) to a CMesh. */
        CMesh ToMesh(const std::string& arLabel, normal_float aOpacity) const;

        // Getters, Setters
        // Used primarily in deprecated smoothing
        inline const std::vector<CPoint>& GetInputPoints() const { return mInputPoints; }
        inline const std::vector<bool>& GetMask() { return mCreationMask; }
        inline const std::vector<normal_float>& GetUVcoords() { return mUVcoords; }
        inline const CSurfaceFaces& GetFaces() const { return mSurfFaces; }
        inline const std::vector<CPoint>& GetVertices() const { return mVertices; }
        inline const CVoronoi& GetVoronoiData() const { return mVoronoi; }

        inline void SetInputPoints(const std::vector<CPoint> &arPoints) { mInputPoints = arPoints; }
        inline void SetMask(const std::vector<bool> &arMask) { mCreationMask = arMask; }
        inline void SetUVcoords(const std::vector<normal_float> &arUVcoords) { mUVcoords = arUVcoords; }
    };

} // namespace vivid
#endif //VIVID_VORONOIVOLUME_H