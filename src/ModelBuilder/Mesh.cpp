#include "Mesh.h"
#include "Model.h" // TODO TOMER!! Y?
#include "External.h"

#include "boost/algorithm/string/predicate.hpp"
//#include <stdio.h>



using namespace vivid;
using namespace boost::algorithm;


// TODO: to place the methods in order and in substructure

CMesh::~CMesh() {}

CMesh::CMesh(const CMesh &mesh){
    mAlpha = mesh.mAlpha;
    mFaces = mesh.mFaces;
    mLabel = mesh.mLabel;
    mPoints = mesh.mPoints;
    mCenVector = mesh.mCenVector;

}


CMesh::CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, coord_t aAlpha, CPoint aCenVector) : mPoints(aPoints), mFaces(aFaces), mLabel(aLabel), mAlpha(aAlpha), mCenVector(aCenVector) {}



void CMesh::ExportToObj(string aOutput, bool WithTexture){
    CModel(*this).ExportToObj(aOutput, WithTexture); //TODO NAFTALI This is how it done.
}

void CMesh::Decimation(coord_t aVerticlePercent, coord_t aMaxError)
{
    //check input valdilty
    if( aVerticlePercent < 0 || aVerticlePercent > 1){
        throw "input error, DecimationPercent must be between 0 and 1";
    }
    if( aMaxError < 0 || aMaxError > 1){
        throw "input error, MaxError must be between 0 and 1";
    }

	//call decimation from External
	int targetVerticesN = int(aVerticlePercent * mPoints.size());
	int targetTrianglesN = int(aVerticlePercent * mFaces.size());
	pair<vector<CPoint>, vector<CIndexedFace> > temp = DecimateMesh(mPoints, GetFacesAsTriangles(), targetVerticesN, targetTrianglesN, aMaxError);
	mPoints = get<0>(temp);
	mFaces = get<1>(temp);
}

vector<CIndexedFace> CMesh::GetFacesAsTriangles() {
    vector<CIndexedFace> aTriangles = vector<CIndexedFace>();
	for (vector<CIndexedFace>::iterator fIt = mFaces.begin(); fIt != mFaces.end(); fIt++) {
		for (size_t i = 1; i < fIt->GetPoints().size()-1; i++) { // go over all the vertices from 1 to n-1 and connect them with vertice 0 to create triangles 
            aTriangles.push_back(CIndexedFace((*fIt)[0], (*fIt)[i], (*fIt)[i + 1], fIt->GetColor()));
		}
	}
	return aTriangles;
}
//geters seters

string CMesh::GetLabel() { return mLabel; }
coord_t CMesh::GetAlpha() { return mAlpha; }
vector<CPoint> CMesh::GetPoints() { return mPoints; }
vector<CIndexedFace> CMesh::GetFaces() { return mFaces; }
void CMesh::SetFaces(vector<CIndexedFace> aFaces) { mFaces = aFaces; }
void CMesh::SetPoints(vector<CPoint> aPoints) { mPoints = aPoints; }
void CMesh::SetLabel(string aLabel) { mLabel = aLabel; }
void CMesh::SetAlpha(coord_t aAlpha){
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
    mAlpha = aAlpha;
}
void CMesh::setCenVector(const CPoint &vector){ mCenVector = vector; }
CPoint CMesh::getCenVector() { return mCenVector; }




void CMesh::TransformMesh(coord_t const aTrans[3][3]){

    double px,py,pz;
    for (auto it = mPoints.begin(); it != mPoints.end(); it++)
    {

        px= it->X(); py= it->Y(); pz= it->Z();

        it->X(aTrans[0][0] * px + aTrans[0][1] * py + aTrans[0][2] * pz);
        it->Y(aTrans[1][0] * px + aTrans[1][1] * py + aTrans[1][2] * pz);
        it->Z(aTrans[2][0] * px + aTrans[2][1] * py + aTrans[2][2] * pz);

        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}

void CMesh::RotateMesh(CPoint aNormVec, double aRadAngel){
    // Trig operations are expansive
    auto cos_a = cos(aRadAngel);
    auto sin_a = sin(aRadAngel);
    // auto one_min_cos_a = 1-cos_a; for optimization it's better but it's less readable...
    auto nx = aNormVec.X();
    auto ny = aNormVec.Y();
    auto nz = aNormVec.Z();

    coord_t const rotation_mat[3][3] = {
            cos_a + nx*nx*(1-cos_a),        nx*ny*(1-cos_a) - nz*sin_a,     nx*nz*(1-cos_a) + ny*sin_a,

            ny*nx*(1-cos_a) + nz*sin_a,     cos_a + ny*ny*(1-cos_a),        ny*nz*(1-cos_a) - nx*sin_a,

            nz*nx*(1-cos_a) - ny*sin_a,     nz*ny*(1-cos_a) + nx*sin_a,     cos_a + nz*nz*(1-cos_a),
    };
    TransformMesh(rotation_mat);
}

void CMesh::MoveMesh(CPoint aDirectionVec){
//    auto x_movement = aDirectionVec.X();
//    auto y_movement = aDirectionVec.Y();
//    auto z_movement = aDirectionVec.Z();
    for (auto it = mPoints.begin(); it != mPoints.end(); it++)
    {
        *it += aDirectionVec;
//        it->X(x_movement + it->X());
//        it->Y(y_movement + it->Y());
//        it->Z(z_movement + it->Z());
        // The only reason I let it pass like that, is because this is an inline code.
        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}

void CMesh::ScaleMesh(CPoint aScaleVec){
    auto x_scale = aScaleVec.X();
    auto y_scale = aScaleVec.Y();
    auto z_scale = aScaleVec.Z();
    for (auto it = this->mPoints.begin(); it != this->mPoints.end(); it++)
    {
        it->X(x_scale * it->X());
        it->Y(y_scale * it->Y());
        it->Z(z_scale * it->Z());
        // The only reason I let it pass like that, is because this is an inline code.
        // We should change all to vectorized operators.
        // May the god of compilers forgive us all for our sins.
    }
}