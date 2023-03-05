#include "ModelComponent.h"
#include "Model.h"

using namespace vivid;

void CModelComponent::SetDimensions() {
    coord_t x_max = (max_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t x_min = (min_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t y_max = (max_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t y_min = (min_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t z_max = (max_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    coord_t z_min = (min_element(mPoints.begin(), mPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    mBoundingBox = {CPoint(x_min, y_min, z_min), CPoint(x_max, y_max, z_max)};
    mCenter = (mBoundingBox.first + mBoundingBox.second) / 2.;
}

/*--------------------------------------------- Transformation Methods -----------------------------------------------*/

void CModelComponent::TransformMesh(FTrans_t const &arTrans){
    for (auto & mPoint : mPoints)
    {
        mPoint = arTrans(mPoint - mCenter) + mCenter;
    }
}

void CModelComponent::TransformMesh(const array<CPoint, 3>& aTrans)
{
    for (auto & mPoint : mPoints)
    {
        CPoint centered_point = mPoint - mCenter;
        mPoint = {centered_point.Dot(aTrans[0]), centered_point.Dot(aTrans[1]), centered_point.Dot(aTrans[2])} + mCenter;
    }
}

void CModelComponent::RotateMesh(const CPoint& arNormVec, float aRadAngel)
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

void CModelComponent::RotateMesh(float yaw, float pitch, float roll) {
    auto cos_yaw = cos(yaw);
    auto sin_yaw = sin(yaw);
    auto cos_pitch = cos(pitch);
    auto sin_pitch = sin(pitch);
    auto cos_roll = cos(roll);
    auto sin_roll = sin(roll);
    array<CPoint, 3> rotation_mat =  {
            CPoint(cos_yaw*cos_pitch, sin_yaw*sin_pitch*sin_roll-sin_yaw*cos_roll, cos_yaw*sin_pitch*cos_roll+sin_yaw*sin_roll),
            CPoint(sin_yaw*cos_pitch, sin_yaw*sin_pitch*sin_roll+cos_yaw*cos_roll, sin_yaw*sin_pitch*sin_roll-cos_yaw*sin_roll),
            CPoint(-sin_pitch, cos_pitch*sin_roll, cos_pitch*cos_roll),
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
       mPoint = ((mPoint - mCenter) * arScaleVec) + mCenter;
   }
}

/*--------------------------------------------- Export Methods -----------------------------------------------*/

void CModelComponent::ExportToObj(const std::string &arOutputFilePath, bool aWithTexture){
    CModel(*this).ExportToObj(arOutputFilePath, aWithTexture);
}

void CModelComponent::Export(const std::string &arOutputFilePath, const std::string& arFileType){
    CModel(*this).Export(arOutputFilePath, arFileType);
}

CBlobData CModelComponent::ExportToBlob(const std::string& arFileType) {
    return CModel(*this).ExportToBlob(arFileType);
}