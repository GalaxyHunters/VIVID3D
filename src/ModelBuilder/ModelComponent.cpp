#include "ModelComponent.h"

using namespace vivid;

//CModelComponent::~CModelComponent() {}

///*--------------------------------------------- Transformation Methods -----------------------------------------------*/
//
//void CModelComponent::TransformMesh(FTrans_t const &arTrans){
//    for (auto & mPoint : mPoints)
//    {
//        mPoint = arTrans(mPoint);
//    }
//}
//
//void CModelComponent::TransformMesh(coord_t const aTrans[3][3]){
//
//    double px,py,pz;
//    for (auto & mPoint : mPoints)
//    {
//
//        px= mPoint.X(); py= mPoint.Y(); pz= mPoint.Z();
//
//        mPoint.X(aTrans[0][0] * px + aTrans[0][1] * py + aTrans[0][2] * pz);
//        mPoint.Y(aTrans[1][0] * px + aTrans[1][1] * py + aTrans[1][2] * pz);
//        mPoint.Z(aTrans[2][0] * px + aTrans[2][1] * py + aTrans[2][2] * pz);
//
//        // We should change all to vectorized operators.
//        // May the god of compilers forgive us all for our sins.
//    }
//}
//
//void CModelComponent::RotateMesh(const CPoint& arNormVec, double aRadAngel){
//    auto cos_a = cos(aRadAngel);
//    auto sin_a = sin(aRadAngel);
//    // auto one_min_cos_a = 1-cos_a; for optimization it's better but it's less readable...
//    auto nx = arNormVec.X();
//    auto ny = arNormVec.Y();
//    auto nz = arNormVec.Z();
//
//    coord_t const rotation_mat[3][3] = {
//            cos_a + nx*nx*(1-cos_a),        nx*ny*(1-cos_a) - nz*sin_a,     nx*nz*(1-cos_a) + ny*sin_a,
//
//            ny*nx*(1-cos_a) + nz*sin_a,     cos_a + ny*ny*(1-cos_a),        ny*nz*(1-cos_a) - nx*sin_a,
//
//            nz*nx*(1-cos_a) - ny*sin_a,     nz*ny*(1-cos_a) + nx*sin_a,     cos_a + nz*nz*(1-cos_a),
//    };
//    TransformMesh(rotation_mat);
//}
//
//void CModelComponent::MoveMesh(const CPoint& arDirectionVec){
//    for (auto & mPoint : mPoints)
//    {
//        mPoint += arDirectionVec;
//    }
//}
//
//void CModelComponent::ScaleMesh(const CPoint& arScaleVec){
//    for (auto & mPoint : mPoints)
//    {
//        mPoint.Scale(arScaleVec);
//    }
//}