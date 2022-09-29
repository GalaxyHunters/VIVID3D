#include "ModelComponent.h"
#include "Model.h"

using namespace vivid;

/*--------------------------------------------- Transformation Methods -----------------------------------------------*/

void CModelComponent::TransformMesh(FTrans_t const &arTrans){
    for (auto & mPoint : mPoints)
    {
        mPoint = arTrans(mPoint);
    }
}

void CModelComponent::TransformMesh(const array<CPoint, 3>& aTrans)
{
    for (auto & mPoint : mPoints)
    {
        mPoint = {mPoint.Dot(aTrans[0]), mPoint.Dot(aTrans[1]), mPoint.Dot(aTrans[2])};
    }
}

void CModelComponent::RotateMesh(const CPoint& arNormVec, double aRadAngel)
{
    auto cos_a = cos(aRadAngel);
    auto sin_a = sin(aRadAngel);
    auto one_min_cos_a = 1-cos_a;
    auto nx = arNormVec.X();
    auto ny = arNormVec.Y();
    auto nz = arNormVec.Z();

    array<CPoint, 3> rotation_mat =  {
            CPoint(cos_a + nx*nx*one_min_cos_a,    nx*ny*one_min_cos_a - nz*sin_a, nx*nz*one_min_cos_a + ny*sin_a),
            CPoint(ny*nx*one_min_cos_a + nz*sin_a, cos_a + ny*ny*one_min_cos_a,    ny*nz*one_min_cos_a - nx*sin_a),
            CPoint(nz*nx*one_min_cos_a - ny*sin_a, nz*ny*one_min_cos_a + nx*sin_a, cos_a + nz*nz*one_min_cos_a),
    };
    TransformMesh(rotation_mat);
}

void CModelComponent::MoveMesh(const CPoint& arDirectionVec){
   for (auto & mPoint : mPoints)
   {
       mPoint += arDirectionVec;
   }
}

void CModelComponent::ScaleMesh(const CPoint& arScaleVec){
   for (auto & mPoint : mPoints)
   {
       mPoint.Scale(arScaleVec);
   }
}

/*--------------------------------------------- Export Methods -----------------------------------------------*/

void CModelComponent::ExportToObj(const std::string &arOutputFilePath, bool aWithTexture){
    CModel(*this).ExportToObj(arOutputFilePath, aWithTexture);
}

int CModelComponent::Export(const std::string &arOutputFilePath, const std::string& arFileType){
    return CModel(*this).Export(arOutputFilePath, arFileType);
}