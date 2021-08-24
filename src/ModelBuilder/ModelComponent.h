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

    coord_t mAlpha = 1.;
    std::string mLabel = "";
    CColorMap mClm;
    //Constructor, Copy Constructor, Destructor
    CModelComponent(){}
    CModelComponent(const coord_t aAlpha, const std::string &arLabel) : mAlpha(aAlpha), mLabel(arLabel), mClm() {}
    CModelComponent(const coord_t aAlpha, const std::string &arLabel, const std::string &arClm) : mAlpha(aAlpha), mLabel(arLabel), mClm(arClm) {}
    CModelComponent(const coord_t aAlpha, const std::string &arLabel, const std::vector<color_t> &arClm, const std::string &arCName) : mAlpha(aAlpha), mLabel(arLabel), mClm(arClm, arCName) {}
    //virtual ~CModelComponent() = 0;

public:
    CModelComponent(const CModelComponent &arModel) : mPoints(arModel.mPoints), mFaces(arModel.mFaces), mAlpha(arModel.mAlpha), mLabel(arModel.mLabel), mClm(arModel.mClm) {}
    // Operator=
    inline CModelComponent& operator= (const CModelComponent& arModel) { mPoints=arModel.mPoints; mFaces=arModel.mFaces; mAlpha=arModel.mAlpha; mLabel=arModel.mLabel; mClm=arModel.mClm; return *this; }

    // Getters, Setters
    inline const std::vector<CPoint> GetPoints() { return mPoints; }
    inline const std::vector<CIndexedFace> GetFaces() { return mFaces; }
    inline const std::string GetLabel() { return mLabel; }
    inline const coord_t GetAlpha() { return mAlpha; }
    inline const CColorMap GetClm() const { return mClm; }

    inline void SetPoints(std::vector<CPoint> &arPoints) { mPoints = arPoints; }
    inline void SetFaces(std::vector<CIndexedFace> &arFaces) { mFaces = arFaces; }
    inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }
    inline void SetAlpha(coord_t aAlpha) {
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
//        void TransformMesh(coord_t const aTrans[3][3]);
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
