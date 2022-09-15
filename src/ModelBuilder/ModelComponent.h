#ifndef VIVID_MODELCOMPONENT_H
#define VIVID_MODELCOMPONENT_H

#include <string>
#include <functional>
#include <map>

#include "LogFile.h"

#include "Point.h"
#include "Face.h"
#include "ColorMap.h"

namespace vivid
{
/* FTrans_t is a function that changes CPoint to another CPoint */
    typedef std::function<const CPoint(const CPoint)> FTrans_t; // Note: no ref use here to avoid unpredictable behavior.

/* Abstract Class defining 3D component for a model. Can be mesh, line, point cloud or anything else. */
    class CModelComponent {
    protected:
        //CLogFile& mLogFile = CLogFile::GetInstance();

        vector<CPoint> mPoints = {};
        vector<CFace> mFaces = {};
        std::string mObjType = "f";
        coord_t mAlpha = 1.;
        std::string mLabel = "";
        CColorMap mClm;
        //Constructor, Copy Constructor, Destructor
        CModelComponent(){}
        CModelComponent(const coord_t aAlpha, const std::string &arLabel, const std::string &arObjType) : mAlpha(max(0.1, min(aAlpha ,1.))), mLabel(arLabel), mObjType(arObjType), mClm() {}
        CModelComponent(const coord_t aAlpha, const std::string &arLabel, const std::string &arObjType, std::string &arClm) : mAlpha(max(0.1, min(aAlpha ,1.))), mLabel(arLabel), mObjType(arObjType), mClm(arClm) {}
        CModelComponent(const coord_t aAlpha, const std::string &arLabel, const std::string &arObjType, const std::vector<color_t> &arClm, const std::string &arCName) : mAlpha(max(0.1, min(aAlpha ,1.))), mLabel(arLabel), mObjType(arObjType), mClm(arClm, arCName) {}
        //virtual ~CModelComponent() = 0;

    public:
        CModelComponent(const CModelComponent &arModel) : mPoints(arModel.mPoints), mFaces(arModel.mFaces), mAlpha(arModel.mAlpha), mLabel(arModel.mLabel), mObjType(arModel.mObjType), mClm(arModel.mClm) {}
        // Operator=
        inline CModelComponent& operator= (const CModelComponent& arModel) { mPoints=arModel.mPoints; mFaces=arModel.mFaces;
            mAlpha=arModel.mAlpha; mLabel=arModel.mLabel; mObjType=arModel.mObjType; mClm=arModel.mClm; return *this; }

        // Getters, Setters
        inline std::vector<CPoint> GetPoints() const { return mPoints; }
        inline std::vector<CFace> GetFaces() const { return mFaces; }
        inline std::string GetLabel() const { return mLabel; }
        inline coord_t GetAlpha() const { return mAlpha; }
        inline CColorMap GetClm() const { return mClm; }
        inline std::string GetObjType() const { return mObjType; }

        inline void SetPoints(std::vector<CPoint> &arPoints) { mPoints = arPoints; }
        inline void SetFaces(std::vector<CFace> &arFaces) { mFaces = arFaces; }
        inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }
        inline void SetAlpha(coord_t aAlpha) {
            //check input valdilty
            if(aAlpha > 1 || aAlpha < 0){

            }
            mAlpha = aAlpha;
        }
        inline void SetClm(string arClm) { mClm.SetColorMap(arClm); }
        inline void SetClm(const std::vector<color_t> &arClm, const std::string &arCName) { mClm.SetColorMap(arClm, arCName); }
//    virtual void ExportToObj(const std::string &aOutputFilePath, bool WithTexture = 1) = 0;

//        /**
//         * transform CMesh points by transformation function
//         * @param[in] aTrans is a function(CPoint) -> CPoint
//         */
//        void TransformMesh(FTrans_t const &arTrans);
//        /**
//         * transform CMesh points by transformation matrix
//         * @param[in] aTrans a 3x3 dimension matrix.
//         */
//        void TransformMesh(coord_t const aTrans[3][3]);
//        /**
//         * Rotate the CMesh points around a normal vector by an angel, counterclockwise
//         * @param[in] arNormVec the x,y.z normal to rotate around.
//         * @param[in] aRadAngel the angel to rotate by (in radians).
//         */
//        void RotateMesh(const CPoint& arNormVec, double aRadAngel);
//        /**
//         * Change CMesh points location by addition of x,y,z.
//         * @param[in] arDirectionVec the x,y.z direction to move by it.
//         */
//        void MoveMesh(const CPoint& arDirectionVec);
//        /**
//         * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
//         * @param[in] arScaleVec the x,y.z direction to move by it.
//         */
//        void ScaleMesh(const CPoint& arScaleVec);


    };

}; // namespace vivid
#endif //VIVID_MODELCOMPONENT_H