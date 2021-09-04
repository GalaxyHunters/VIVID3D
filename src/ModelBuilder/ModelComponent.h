#ifndef VIVID_MODELCOMPONENT_H
#define VIVID_MODELCOMPONENT_H

#include <string>
#include <functional>
#include <ColorMap.h>
#include <map>
#include "IndexedFace.h"

namespace vivid
{

// TODO: Which setters and getters are really needed?
/* Abstract Class defining 3D component for a model. Can be mesh, line, point cloud or anything else. */
//TODO splines?
class CModelComponent {
protected:
    vector<CPoint> mPoints = {};
    vector<CIndexedFace> mFaces = {};
    std::string mObjType = "f";
    quan_t mAlpha = 1.;
    std::string mLabel = "";
    CColorMap mClm;
    //Constructor, Copy Constructor, Destructor
    CModelComponent(){}
    CModelComponent(const quan_t aAlpha, const std::string &arLabel, const std::string &arObjType) : mAlpha(aAlpha), mLabel(arLabel), mObjType(arObjType), mClm() {}
    CModelComponent(const quan_t aAlpha, const std::string &arLabel, const std::string &arObjType, const std::string &arClm) : mAlpha(aAlpha), mLabel(arLabel), mObjType(arObjType), mClm(arClm) {}
    CModelComponent(const quan_t aAlpha, const std::string &arLabel, const std::string &arObjType, const std::vector<color_t> &arClm, const std::string &arCName) : mAlpha(aAlpha), mLabel(arLabel), mObjType(arObjType), mClm(arClm, arCName) {}
    //virtual ~CModelComponent() = 0;

public:
    CModelComponent(const CModelComponent &arModel) : mPoints(arModel.mPoints), mFaces(arModel.mFaces), mAlpha(arModel.mAlpha), mLabel(arModel.mLabel), mObjType(arModel.mObjType), mClm(arModel.mClm) {}
    // Operator=
    inline CModelComponent& operator= (const CModelComponent& arModel) { mPoints=arModel.mPoints; mFaces=arModel.mFaces; mAlpha=arModel.mAlpha; mLabel=arModel.mLabel; mObjType=arModel.mObjType; mClm=arModel.mClm; return *this; }

    // Getters, Setters
    inline std::vector<CPoint> GetPoints() const { return mPoints; }
    inline std::vector<CIndexedFace> GetFaces() const { return mFaces; }
    inline std::string GetLabel() const { return mLabel; }
    inline quan_t GetAlpha() const { return mAlpha; }
    inline CColorMap GetClm() const { return mClm; }
    inline std::string GetObjType() const { return mObjType; }

    inline void SetPoints(std::vector<CPoint> &arPoints) { mPoints = arPoints; }
    inline void SetFaces(std::vector<CIndexedFace> &arFaces) { mFaces = arFaces; }
    inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }
    inline void SetAlpha(quan_t aAlpha) {
        //check input valdilty
        if(aAlpha > 1 || aAlpha < 0){
            throw "Alpha must be between 0 and 1";
        }
        mAlpha = aAlpha;
    }
    inline void SetClm(const string &arClm) { mClm.SetColorMap(arClm); }
    inline void SetClm(const std::vector<color_t> &arClm, const std::string &arCName) { mClm.SetColorMap(arClm, arCName); }
//    virtual void ExportToObj(const std::string &aOutputFilePath, bool WithTexture = 1) = 0;

// set pure virtual
//        /**
//         * transform CMesh points by transformation matrix
//         * @param[in] aTrans a 3x3 dimension matrix.
//         */
//        void TransformMesh(quan_t const aTrans[3][3]);
//        /**
//         * Rotate the CMesh points around a normal vector by an angel, counterclockwise
//         * @param[in] aNormVec the x,y.z normal to rotate around.
//         * @param[in] aRadAngel the angel to rotate by (in radians).
//         */
//        void RotateMesh(CPoint aNormVec, double aRadAngel);
//        /**
//         * Change CMesh points location by addition of x,y,z.
//         * @param[in] aDirectionVec the x,y.z direction to move by it.
//         */
//        void MoveMesh(CPoint aDirectionVec);
//        /**
//         * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
//         * @param[in] aScaleVec the x,y.z direction to move by it.
//         */
//        void ScaleMesh(CPoint aScaleVec);


};

}; // namespace vivid
#endif //VIVID_MODELCOMPONENT_H
