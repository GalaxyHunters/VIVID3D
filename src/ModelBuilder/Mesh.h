#ifndef MESH_H
#define MESH_H


#include "Point.h"
#include "IndexedFace.h"
#include "../lib/External.h"
#include "Utils/ColorMap.h"

#include "boost/algorithm/string/predicate.hpp"
#include "int2str.hpp"
#include <string>
#include <fstream>
#include <vector>

namespace vivid
{

class CMesh {

private:
	coord_t mAlpha;
	string mLabel;
	vector<CPoint> mPoints;
	vector<CIndexedFace> mFaces;
    CPoint mCenVector; // holds the center of the data (used to center the data by 000 and back to original upon export)

    vector<CIndexedFace> GetFacesAsTriangles(); // TODO BADDD!!!

public:
	CMesh() {};
    CMesh(const CMesh &mesh);
	CMesh(std::vector<CPoint> aPoints, std::vector<CIndexedFace> aFaces, std::string aLabel, coord_t aAlpha, CPoint aCenVector = CPoint(0, 0, 0));
	~CMesh();
    std::string GetLabel();
	coord_t GetAlpha();
    std::vector<CPoint> GetPoints();
    std::vector<CIndexedFace> GetFaces();
    CPoint getCenVector();
	void SetFaces(std::vector<CIndexedFace> aFaces);
	void SetPoints(std::vector<CPoint> aPoints);
	void SetLabel(std::string aLabel);
	void SetAlpha(coord_t aAlpha);
	void setCenVector(const CPoint &vector);


    void Decimation(coord_t aVerticlePercent, coord_t aError);

    void ExportToObj(string aOutput, bool WithTexture = 1); // TODO (Naftali) Shouldbn't it be a stream too?


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
    void RotatewMesh(CPoint aNormVec, double aRadAngel);
    /**
     * Change CMesh points location by addition of x,y,z.
     * @param[in] aDirectionVec the x,y.z direction to move by it.
     */
    void MoveMesh(CPoint aDirectionVec);
    /**
     * Change XMesh points scale in x,y,z direction by x,y,z of the scale vector
     * @param[in] aScaleVec the x,y.z direction to move by it.
     */
    void ScaleMesh(CPoint aScaleVec);

};

} // namespace vivid
#endif

