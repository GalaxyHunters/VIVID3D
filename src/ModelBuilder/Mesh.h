#ifndef VIVID_MESH_H
#define VIVID_MESH_H

#include "ModelComponent.h"

#include "Point.h"
//#include "LogFile.h"
#include "Face.h"
#include "Decimate.h"
#include "ColorMap.h"

#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <unordered_set>

namespace vivid
{

    class CMesh : public CModelComponent
    {
    private:
        std::map<size_t, std::unordered_set<size_t>> mPointNeighbours = {};
        void TriangulizeFaces();
        void CalculatePointsNeighbours();
        bool mFacesAreTriangles = false;

    public:
        CMesh() {};
        CMesh(const std::vector<CPoint> &arPoints, const std::vector<CFace> &arFaces, const std::string &arLabel, normal_float aOpacity) :
                CModelComponent(aOpacity, arLabel, TRIANGLES) {
            mPoints = arPoints; mFaces = arFaces;
            TriangulizeFaces();
            CalculatePointsNeighbours();
        }
        CMesh(const CMesh &arMesh) : CModelComponent(arMesh), mPointNeighbours(arMesh.mPointNeighbours){};

        /**
         * Decimate faces on CMesh to reduce file size
         * @param[in] aVerticlePercent is a normalized double signifying how many vertices to retain
         * @param[in] aMaxError is a normalized double of how much to retain the original shape of the faces
         */
        void Reduce(coord_t aVerticlePercent, coord_t aMaxError);
        void SubdivideLargeFaces(coord_t aAboveAverageThreshold=5.);
        void RemoveLargeFaces(coord_t aAboveAverageThreshold=2.);
        void RemovePointyFaces(coord_t aThetaThreshold = 15.);

        /**
         * Smooth faces on CMesh by Laplacian Smooth
         * @param[in] aNumIterations number of iterations to run for, recommended to be even
         * @param[in] aOpacityFactor weighted value for smoothing, recommended range 0.2<a<0.8
         * @param[in] aBetaFactor weighted value for volume retention, recommended range 0.5<b<1.0
         */
        void LaplacianSmooth(size_t aNumIterations, double aAlphaFactor = 0.5, double aBetaFactor = 0.5);
    };

} // namespace vivid
#endif //VIVID_MESH_H
