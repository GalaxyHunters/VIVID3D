#ifndef VIVID_MESH_H
#define VIVID_MESH_H

#include "ModelComponent.h"

#include "Point.h"
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


class CMesh : public CModelComponent
{
private:
    string mLabel = "";
	coord_t mAlpha = 0;
	vector<CPoint> mPoints ={};
	vector<CIndexedFace> mFaces ={};

    vector<CIndexedFace> GetFacesAsTriangles(); // TODO BADDD!!!

public:
	CMesh() {};
    CMesh(const CMesh &aMesh) :
        mLabel(aMesh.mLabel), mAlpha(aMesh.mAlpha), mPoints(aMesh.mPoints), mFaces(aMesh.mFaces), CModelComponent(aMesh){};
	CMesh(std::vector<CPoint> aPoints, std::vector<CIndexedFace> aFaces, std::string aLabel, coord_t aAlpha):
	    mPoints(aPoints), mFaces(aFaces), mLabel(aLabel), mAlpha(aAlpha), CModelComponent(aLabel){}
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

    // Getters, Setters
    std::string GetLabel() { return mLabel; }
    coord_t GetAlpha() { return mAlpha; }
    std::vector<CPoint> GetPoints() { return mPoints; }
    std::vector<CIndexedFace> GetFaces() { return mFaces; }

    void SetFaces(std::vector<CIndexedFace> aFaces) { mFaces = aFaces; }
    void SetPoints(std::vector<CPoint> aPoints) { mPoints = aPoints; }
    void SetLabel(std::string aLabel) { mLabel = aLabel; }
    void SetAlpha(coord_t aAlpha) {
        //check input valdilty
        if(aAlpha > 1 || aAlpha < 0){
            throw "Alpha must be between 0 and 1";
        }
        mAlpha = aAlpha;
    }

};

} // namespace vivid
#endif //VIVID_MESH_H

