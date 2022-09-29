#include <algorithm>
#include "SurfByFunc.h"

#define SUBDIVISION_FACTOR 10.

namespace vivid
{

CMesh SurfByFunc(const F3D_t &func, int aBoxSize, normal_float aOpacity, const std::string &arLabel, bool RemoveFlatSurfaces) {
    // Creating Wireframe
    aBoxSize *= SUBDIVISION_FACTOR;
    vector<CPoint> points = {};
//    for (int i = -aBoxSize; i < aBoxSize; i += 1) {
//        for (int j = -aBoxSize; j < aBoxSize; j += 1) {
//            points.push_back(CPoint(i,i,0.));
//        }
//    }
    vector<normal_float> quan = {};
    coord_t temp_x, temp_y;

    for (int x = -aBoxSize; x <= aBoxSize; x += 1) {
        for (int y = -aBoxSize; y <= aBoxSize; y += 1) {
            temp_x = x/SUBDIVISION_FACTOR; temp_y = y/SUBDIVISION_FACTOR;
            coord_t z = func(temp_x, temp_y);
            if (z > 2*aBoxSize/SUBDIVISION_FACTOR) {
                z = 2*aBoxSize/SUBDIVISION_FACTOR;
            }
            else if (z < -2*aBoxSize/SUBDIVISION_FACTOR) {
                z = -2*aBoxSize/SUBDIVISION_FACTOR;
            }
            else if (isnan(z)) {
                z = 0;
            }
            CPoint point = {temp_x,z,temp_y};
            points.push_back(point);
            quan.push_back((float)z);
        }
    }

    for (int i = 0; i < points.size(); i++) {
        if (isnan(points[i].Y())){
            points[i] = CPoint(points[i].X(), 0, points[i].Z());
        }
    }
    coord_t v_max = *max_element(quan.begin(), quan.end());
    coord_t v_min = *min_element(quan.begin(), quan.end());
    if (v_max <= 0) {
        v_max = v_min;
        v_min = *max_element(quan.begin(), quan.end());
    }
    coord_t divide_by = 1. / (v_max - v_min);

    for (int i = 0; i < points.size(); i++) {
        quan[i] = (quan[i] - v_min) * divide_by;
    }
    vector<CFace> faces;
    for (size_t i = 0; i < (2*aBoxSize)*(2*aBoxSize); i += aBoxSize*2+1) {
        for (size_t j = 0; j < 2*(aBoxSize); j += 1) {
            coord_t color = (quan[i + j] + quan[i + j + 1] + quan[i + 2 * aBoxSize + j + 1] + quan[i + 2 * aBoxSize + j]) / 4.;
            //            if (!(isinf(points[i+j].Y()) & isinf(points[i+j+1].Y()) & isinf(points[i+j+2+2*aBoxSize].Y()) & isinf(points[i+j+1+2*aBoxSize].Y()))) {
            //                faces.push_back(CFace({i + j, i + (j + 1), i + 2*aBoxSize + (j + 2), i + 2*aBoxSize + j + 1}, color));
            //                cout << i + j << " " << i + (j + 1) << " " << i + 2*aBoxSize + (j + 2) << " " << i + 2*aBoxSize + j + 1<< endl;
            //            }
            if (!(isnan(points[i+j].Y()) & isnan(points[i+j+1].Y()) & isnan(points[i+j+2+2*aBoxSize].Y()) & isnan(points[i+j+1+2*aBoxSize].Y()))) {
                if (RemoveFlatSurfaces) {
                    if ((points[i+j].Y() != 0 & points[i+j+1].Y() != 0 & points[i+j+2+2*aBoxSize].Y() != 0 & points[i+j+1+2*aBoxSize].Y() != 0)) {
                        faces.push_back(CFace({i + j, i + (j + 1), i + 2 * aBoxSize + (j + 2), i + 2 * aBoxSize + j + 1}, color));
                    }
                } else {
                    faces.push_back(CFace({i + j, i + (j + 1), i + 2 * aBoxSize + (j + 2), i + 2 * aBoxSize + j + 1}, color));
                }
            }
        }
    }

    CMesh mesh = CMesh(points, faces, arLabel, aOpacity);
    return mesh;
}

CMesh ParametricSurface(const FParametric_t &func, int aNumberOfSteps, coord_t aThetaMin, coord_t aThetaMax,
                        coord_t aPhiMin, coord_t aPhiMax, normal_float aOpacity, const std::string &arLabel)
{
    vector<CPoint> points;
    vector<CFace> faces;
    coord_t aColor = 0.5;

    /* Adds points around sphere*/
    points.push_back(CPoint(0, 0,  1)); //Creating the top polar
    points.push_back(CPoint(0, 0, -1)); //Creating the bottom polar

//    double phi_step = (M_PI/2) / (aNumberOfSteps + 1);
//    double theta_step = (2 * M_PI) / aNumberOfSteps;
//
//    for (int i = 0; i < aNumberOfSteps; i++)
//    {
//        double theta = i * theta_step;
//        for (int j = 1; j < aNumberOfSteps + 1; j++)
//        {
//            double phi = M_PI - (j * phi_step);
//            points.push_back(func(phi, theta));    //creating the points
//        }
//    }
    double phi_step = (aPhiMax - aPhiMin) / (aNumberOfSteps + 1);
    double theta_step = aThetaMax / aNumberOfSteps;
    for (int i = 0; i < aNumberOfSteps; i ++) {
        coord_t theta = aThetaMin + i * theta_step;
        for (int j = 1; j <= aNumberOfSteps; j ++) {
            coord_t phi = aPhiMax - j * phi_step;
//            CPoint point = func(phi, theta);
            points.push_back(func(phi, theta));
        }
    }

    /* Add triangle faces around top point (0, 0, 1) */
    for (size_t i = 0; i < aNumberOfSteps - 1; i++)
    {
        faces.push_back(CFace(vector<size_t>{0, 2 + (i + 1) * aNumberOfSteps, 2 + i * aNumberOfSteps}, aColor)); //creates triangles
    }
    faces.push_back(CFace(vector<size_t>{0, 2, (size_t)2 + (aNumberOfSteps - 1) * aNumberOfSteps}, aColor));

    /* Add rectangular faces around center */
    for (size_t i = 2; i < 1 + aNumberOfSteps; i++)
    {
        for (size_t j = 0; j < aNumberOfSteps - 1; j++)
        {
            faces.push_back(CFace(vector<size_t>{i + j * aNumberOfSteps, i + (j + 1) * aNumberOfSteps,
                                                 i + 1 + (j + 1) * aNumberOfSteps, i + 1 + j * aNumberOfSteps}, aColor));
        }
        faces.push_back(CFace(vector<size_t>{i + (aNumberOfSteps - 1) * aNumberOfSteps,
                                             (size_t)i, (size_t)(i + 1),
                                             i + 1 + (aNumberOfSteps - 1) * aNumberOfSteps}, aColor));
    }
    /* Add triangle faces around bottom point (0, 0, -1) */
    for (size_t i = 0; i < aNumberOfSteps - 1; i++)
    {
        faces.push_back(CFace(vector<size_t>{1, 1 + (i + 1) * aNumberOfSteps, 1 + (i + 2) * aNumberOfSteps}, aColor));
    }
    faces.push_back(CFace(vector<size_t>{1, (size_t)1 + (aNumberOfSteps) * aNumberOfSteps, (size_t)1 + aNumberOfSteps}, aColor));

    CMesh mesh(points, faces, arLabel, aOpacity);
//    mesh.MoveMesh(arCenter);
//    mesh.ScaleMesh(CPoint(aRadius, aRadius, aRadius));

    return mesh;

//    vector<CPoint> points = {};
//    vector<coord_t> quan = {};
//
//    double phi_step = aPhiMax / aNumberOfSteps;
//    double theta_step = aThetaMax / aNumberOfSteps;
//    for (int i = 0; i <= aNumberOfSteps; i += 1) {
//        coord_t theta = aThetaMin + i * theta_step;
//        for (int j = 0; j <= aNumberOfSteps; j += 1) {
//            coord_t phi = aPhiMin + j * phi_step;
//            CPoint point = func(phi, theta);
//            points.push_back(point);
//            quan.push_back(point.Magnitude());
//        }
//    }
//
//    coord_t v_max = *max_element(quan.begin(), quan.end());
//    coord_t v_min = *min_element(quan.begin(), quan.end());
//
//    coord_t divide_by = 1. / (v_max - v_min);
//
//    for (int i = 0; i < points.size(); i++) {
//        quan[i] = (quan[i] - v_min) * divide_by;
//    }
//    vector<CFace> faces;
//
//    /* Add rectangular faces around center */
//    for (size_t i = 0; i < aNumberOfSteps; i++)
//    {
//        for (size_t j = 0; j < aNumberOfSteps; j++)
//        {
//            coord_t color = (quan[i + j * aNumberOfSteps] + quan[i + (j + 1) * aNumberOfSteps] +
//                             quan[ i + 1 + (j + 1) * aNumberOfSteps] + quan[i + 1 + j * aNumberOfSteps]) / 4.;
//
//            faces.push_back(CFace({i + j * aNumberOfSteps, i + (j + 1) * aNumberOfSteps,
//                                          i + 1 + (j + 1) * aNumberOfSteps, i + 1 + j * aNumberOfSteps}, color));
//        }
////        faces.push_back(CFace({i + (aNumberOfSteps - 1) * aNumberOfSteps,
////                                                    i, (i + 1),
////                                                    i + 1 + (aNumberOfSteps - 1) * aNumberOfSteps}, 0));
//    }
//
//    CMesh mesh = CMesh(points, faces, arLabel, aAlpha);
//    return mesh;
}

} // namespace vivid