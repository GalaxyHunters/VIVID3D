#ifndef VIVID_MESH_H
#define VIVID_MESH_H

#include "ModelComponent.h"

#include "Point.h"
//#include "LogFile.h"
#include "IndexedFace.h"
#include "Decimate.h"
#include "Utils/ColorMap.h"

#include <string>
#include <fstream>
#include <vector>
#include <functional>

namespace vivid
{

/* FTrans_t is a function that changes CPoint to another CPoint */
typedef std::function<const CPoint(const CPoint)> FTrans_t; // Note: no ref use here to avoid unpredictable behavior.

// TODO: CSurface inherits CModelComponent, CMesh inherits CSurface, and CSurface gets hidden constructor so its virtual
// TODO: CSurface automatically converts ToMesh everytime
class CMesh : public CModelComponent
{
private:
    // TODO: maybe should be part of modelComponent
//    static void CallBack(const CLogFile::ELogCode aCode, const CLogFile::ELogMessage aMsg)
//    {
//        CLogFile::GetInstance().Write(aCode, aMsg);
//    }
//    CLogFile::LogCallBackFunction mLogFile = CallBack;

    vector<CIndexedFace> GetFacesAsTriangles(); // TODO BADDD!!!

public:
	CMesh() {};
    CMesh(std::vector<CPoint> aPoints, std::vector<CIndexedFace> aFaces, std::string aLabel, coord_t aAlpha) :
        CModelComponent(aAlpha, aLabel, "f"){
        mPoints = aPoints; mFaces = aFaces;
    }
    CMesh(std::vector<CPoint> aPoints, std::vector<CIndexedFace> aFaces, std::string aLabel, coord_t aAlpha, const std::vector<color_t> &arClm, const std::string &arCName) :
	   CModelComponent(aAlpha, aLabel, "f", arClm, arCName){
        mPoints = aPoints; mFaces = aFaces;
    }
    CMesh(std::vector<CPoint> aPoints, std::vector<CIndexedFace> aFaces, std::string aLabel, coord_t aAlpha, const string &arClm) :
	    CModelComponent(aAlpha, aLabel, "f", arClm){
        mPoints = aPoints; mFaces = aFaces;
    }
    CMesh(const CMesh &arMesh) :
	    CModelComponent(arMesh){
    };
	//operator =
	~CMesh();

    /**
     * Decimate faces on CMesh to reduce file size
     * @param[in] aVerticlePercent is a normalized double signifying how many vertices to retain
     * @param[in] aMaxError is a normalized double of how much to retain the original shape of the faces
     */
    void Reduce(coord_t aVerticlePercent, coord_t aMaxError);

    void ExportToObj(string aOutput, bool WithTexture = 1); //TODO const std::string &aOutputFilePath

    /**
     * transform CMesh points by transformation function
     * @param[in] aTrans is a function(CPoint) -> CPoint
     */
    void TransformMesh(FTrans_t const aTrans);
    /**
     * transform CMesh points by transformation matrix
     * @param[in] aTrans a 3x3 dimension matrix.
     */
    void TransformMesh(coord_t const aTrans[3][3]);
    /**
     * Rotate the CMesh points around a normal vector by an angel, counterclockwise
     * @param[in] aNormVec the x,y.z normal to rotate around.
     * @param[in] aRadAngel the angel to rotate by (in radians).
     */
    void RotateMesh(CPoint aNormVec, double aRadAngel);
    /**
     * Change CMesh points location by addition of x,y,z.
     * @param[in] aDirectionVec the x,y.z direction to move by it.
     */
    void MoveMesh(CPoint aDirectionVec);
    /**
     * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
     * @param[in] aScaleVec the x,y.z direction to move by it.
     */
    void ScaleMesh(CPoint aScaleVec);


};

} // namespace vivid
#endif //VIVID_MESH_H

