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
        bool mFacesAreTriangles = false;

    public:
        CMesh() {};
        CMesh(const std::vector<CPoint> &arPoints, const std::vector<CFace> &arFaces, const std::string &arLabel, coord_t aAlpha) :
                CModelComponent(aAlpha, arLabel, "f") {
            mPoints = arPoints; mFaces = arFaces;
            TriangulizeFaces();
            CalculatePointsNeighbours();
        }

        CMesh(const std::vector<CPoint> &arPoints, const std::vector<CFace> &arFaces, const std::string &arLabel, coord_t aAlpha, const std::vector<color_t> &arClm, const std::string &arCName) :
                CModelComponent(aAlpha, arLabel, "f", arClm, arCName){
            mPoints = arPoints; mFaces = arFaces;
            TriangulizeFaces();
            CalculatePointsNeighbours();
        }
        CMesh(const std::vector<CPoint> &arPoints, const std::vector<CFace> &arFaces, const std::string &arLabel, coord_t aAlpha, string &arClm) :
                CModelComponent(aAlpha, arLabel, "f", arClm){
            mPoints = arPoints; mFaces = arFaces;
            TriangulizeFaces();
            CalculatePointsNeighbours();
        }
        CMesh(const CMesh &arMesh) :
                CModelComponent(arMesh), mPointNeighbours(arMesh.mPointNeighbours){};

        //~CMesh() {};
        void CalculatePointsNeighbours();
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
         * @param[in] aAlphaFactor weighted value for smoothing, recommended range 0.2<a<0.8
         * @param[in] aBetaFactor weighted value for volume retention, recommended range 0.5<b<1.0
         */
        void LaplacianSmooth(size_t aNumIterations, double aAlphaFactor = 0.5, double aBetaFactor = 0.5);

        void ExportToObj(const std::string &arOutputFilePath, bool WithTexture = 1);

        /**
        * Assimp export. writes arModel in aFileType format at aOutputPath
        * @param[in] aOutputPath Path and name for output file
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        */
        int Export(const std::string &arOutputFilePath, std::string aFileType = "obj");


        // TODO: Figure out why the wrapper doesn't like it in modelcomponent
        /**
             * transform CMesh points by transformation function
             * @param[in] aTrans is a function(CPoint) -> CPoint
             */
        void TransformMesh(FTrans_t const &arTrans);
        /**
         * transform CMesh points by transformation matrix
         * @param[in] aTrans a 3x3 dimension matrix.
         */
        void TransformMesh(const std::array<CPoint, 3>& aTrans);
        /**
         * Rotate the CMesh points around a normal vector by an angel, counterclockwise
         * @param[in] arNormVec the x,y.z normal to rotate around.
         * @param[in] aRadAngel the angel to rotate by (in radians).
         */
        void RotateMesh(const CPoint& arNormVec, double aRadAngel);
        /**
         * Change CMesh points location by addition of x,y,z.
         * @param[in] arDirectionVec the x,y.z direction to move by it.
         */
        void MoveMesh(const CPoint& arDirectionVec);
        /**
         * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
         * @param[in] arScaleVec the x,y.z direction to move by it.
         */
        void ScaleMesh(const CPoint& arScaleVec);

    };

} // namespace vivid
#endif //VIVID_MESH_H
