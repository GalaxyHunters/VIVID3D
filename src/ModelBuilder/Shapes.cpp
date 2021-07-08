#include "Shapes.h"
#include <cassert>
#include <math.h>
//#define _USE_MATH_DEFINES //TODO WTF???


using namespace vivid;
using namespace std;

constexpr double ARROW_WIDTH_FACTOR = 0.4;
constexpr double ARROW_LENGTH_FACTOR = 0.8;

namespace vivid
{

CMesh CreateCubeMesh(const CPoint &arCenter, coord_t aSize, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    CMesh mesh = CreateBoxMesh(arCenter, CPoint(aSize, aSize, aSize), aColor, aAlpha, arLabel);
    return mesh;
}


CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    vector<CPoint> points= { CPoint(-1, -1, -1), //5 -> 0
                             CPoint(-1, -1, +1), //6 -> 1
                             CPoint(-1, +1, -1), //3 -> 2
                             CPoint(-1, +1, +1), //0 -> 3
                             CPoint(+1, -1, -1), //4 -> 4
                             CPoint(+1, -1, +1), //7 -> 5
                             CPoint(+1, +1, -1), //2 -> 6
                             CPoint(+1, +1, +1) };//1 -> 7

    vector<CIndexedFace> faces = { CIndexedFace(vector<size_t>{3, 2, 6, 7}, aColor),
                                   CIndexedFace(vector<size_t>{6, 2, 0, 4}, aColor),
                                   CIndexedFace(vector<size_t>{7, 6, 4, 5}, aColor),
                                   CIndexedFace(vector<size_t>{3, 7, 5, 1}, aColor),
                                   CIndexedFace(vector<size_t>{2, 3, 1, 0}, aColor),
                                   CIndexedFace(vector<size_t>{1, 5, 4, 0}, aColor) };

    CMesh mesh(points, faces, arLabel, aAlpha);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh(arSize);

    return mesh;
}

CMesh CreateSphereMesh(const CPoint &arCenter, coord_t aRadius, size_t aNumOfMeridians, size_t aNumOfParallels, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    vector<CPoint> points;
    vector<CIndexedFace> faces;
    double theta, phi;

    points.push_back(CPoint(0, 0, aRadius)); //Creating the top polar
    points.push_back(CPoint(0, 0, -aRadius)); //Creating the bottom polar

    double phi_step = M_PI / (aNumOfParallels + 1);
    double theta_step = (2 * M_PI) / aNumOfMeridians;

    for (int i = 0; i < aNumOfMeridians; i++)
    {
        theta = i * theta_step;
        for (int j = 1; j < aNumOfParallels + 1; j++)
        {
            phi = (M_PI / 2) - (j * phi_step);
            //points.push_back(CPoint(arCenter.X() + aRadius * cos(phi) * cos(theta), arCenter.Y() + aRadius * cos(phi) * sin(theta), arCenter.Z() + aRadius * sin(phi)));
            /* TODO: (ADAM) Why the fuck doesn't this work without the aRadius??? */
            points.push_back(CPoint(aRadius*cos(phi) * cos(theta), aRadius*cos(phi) * sin(theta), aRadius*sin(phi)));    //creating the points
        }
    }

    for (int i = 0; i < aNumOfMeridians - 1; i++)
    {
        faces.push_back(CIndexedFace(vector<size_t>{0, 2 + (i + 1) * aNumOfParallels, 2 + i * aNumOfParallels}, aColor)); //creates triangles
    }
    faces.push_back(CIndexedFace(vector<size_t>{0, 2, 2 + (aNumOfMeridians - 1) * aNumOfParallels}, aColor));

    for (int i = 2; i < 1 + aNumOfParallels; i++)
    {
        for (int j = 0; j < aNumOfMeridians - 1; j++)
        {
            faces.push_back(CIndexedFace(vector<size_t>{i + j * aNumOfParallels, i + (j + 1) * aNumOfParallels,
                                                        i + 1 + (j + 1) * aNumOfParallels, i + 1 + j * aNumOfParallels}, aColor));
        }
        faces.push_back(CIndexedFace(vector<size_t>{i + (aNumOfMeridians - 1) * aNumOfParallels,
                                                    (size_t)i, (size_t)(i + 1),
                                                    i + 1 + (aNumOfMeridians - 1) * aNumOfParallels}, aColor));
    }

    for (int i = 0; i < aNumOfMeridians - 1; i++)
    {
        faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (i + 1) * aNumOfParallels, 1 + (i + 2) * aNumOfParallels}, aColor));
    }
    faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (aNumOfMeridians) * aNumOfParallels, 1 + aNumOfParallels}, aColor));

    CMesh mesh(points, faces, arLabel, aAlpha);
    mesh.MoveMesh(arCenter);
    //mesh.ScaleMesh(CPoint(aRadius, aRadius, aRadius));

	return mesh;
}

// TODO: Zohar again VERY UGLY code, please use transformMat
CMesh CreateEllipsoidMesh(size_t aNumOfMeridians, size_t aNumOfParallels, vector<double> aRadiusVec, const CPoint &arCenter, vector<double> aMajorAxis, vector<double> aMiddleAxis, vector<double> aMinorAxis, coord_t aColor, coord_t aAlpha, string aLabel){
    //begin by asserting some conditions
    if (aMajorAxis == vector<double>{0,0,0} || aMiddleAxis == vector<double>{0,0,0} || aMinorAxis == vector<double>{0,0,0}) {
        throw "Axis vectors cannot be (0,0,0)";
    }
    CMesh Ellipsoid;
    //use aRadiusVec to create strech matrix and stretch the sphere
    vector<vector<double>> StretchMatrix;
    StretchMatrix.push_back(vector<double>{aRadiusVec[0], 0, 0});
    StretchMatrix.push_back(vector<double>{0, aRadiusVec[1], 0});
    StretchMatrix.push_back(vector<double>{0, 0, aRadiusVec[2]});
    //use all axis vectors to create a rotation matrix
    vector<vector<double>> RotateMatrix;
    RotateMatrix.push_back(aMajorAxis);
    RotateMatrix.push_back(aMiddleAxis);
    RotateMatrix.push_back(aMinorAxis);
    //apply matrices to points
    vector<CPoint> points = Ellipsoid.GetPoints();
    for (int i = 0; i < points.size(); i++)
    {
        CPoint& CurrentVector = points[i];
        //right now im using a refrence to change the vector according to the matrix
        double CVX = CurrentVector.X();
        double CVY = CurrentVector.Y();
        double CVZ = CurrentVector.Z();
        //first we strech em
        CurrentVector = CPoint(CVX * StretchMatrix[0][0] + CVY * StretchMatrix[1][0] + CVZ * StretchMatrix[2][0],
                               CVX * StretchMatrix[0][1] + CVY * StretchMatrix[1][1] + CVZ * StretchMatrix[2][1],
                               CVX * StretchMatrix[0][2] + CVY * StretchMatrix[1][2] + CVZ * StretchMatrix[2][2]);
        //now we rotate em
        CVX = CurrentVector.X();
        CVY = CurrentVector.Y();
        CVZ = CurrentVector.Z();
        CurrentVector = CPoint(arCenter.X() + CVX * RotateMatrix[0][0] + CVY * RotateMatrix[1][0] + CVZ * RotateMatrix[2][0],
                               arCenter.Y() + CVX * RotateMatrix[0][1] + CVY * RotateMatrix[1][1] + CVZ * RotateMatrix[2][1],
                               arCenter.Z() + CVX * RotateMatrix[0][2] + CVY * RotateMatrix[1][2] + CVZ * RotateMatrix[2][2]);
    }

    Ellipsoid.SetPoints(points);
    return Ellipsoid;
    //set new info to the mesh

}
// TODO: Zohar again VERY UGLY code, please use transformMat
CMesh CreateEllipsoidByTransformMesh(const CPoint &arCenter, size_t aNumOfMeridians, size_t aNumOfParallels, const CPoint &arRadiusVec, vector<coord_t> aMajorAxis, vector<coord_t> aMiddleAxis, vector<coord_t> aMinorAxis, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    if (aMajorAxis == vector<double>{0,0,0} || aMiddleAxis == vector<double>{0,0,0} || aMinorAxis == vector<double>{0,0,0}) {
        throw "Axis vectors cannot be (0,0,0)";
    }
    CMesh Ellipsoid = CreateSphereMesh(arCenter, 1.0, aNumOfMeridians, aNumOfParallels, aColor, aAlpha, arLabel);
    //use aRadiusVec to create strech matrix and stretch the sphere
    Ellipsoid.ScaleMesh(arRadiusVec);
    // I'm not sure this has the same result as Zohars code.
    coord_t aMat[3][3];
    aMat[0][0] = aMajorAxis[0]; aMat[0][1] = aMajorAxis[1]; aMat[0][2] = aMajorAxis[2];
    aMat[1][0] = aMiddleAxis[0]; aMat[1][1] = aMiddleAxis[1]; aMat[1][2] = aMiddleAxis[2];
    aMat[2][0] = aMinorAxis[0]; aMat[2][1] = aMinorAxis[1]; aMat[2][2] = aMinorAxis[2];
    Ellipsoid.TransformMesh(aMat);

    return Ellipsoid;
}

// TODO ask Zohar what kind of arrow is it
// TODO, THINK about Jill Neiman's arrow and the scale thingy
//CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, coord_t aColor, coord_t aAlpha, const string &arLabel)

    CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, coord_t aColor, coord_t aAlpha, const string &arLabel){
        //before we run, lets assert some conditions //TODO same comment on Exceptions
        //assert(("Direction Vector cannot be (0,0,0)", DirVec != vector<double>{0,0,0}));
        assert(("aWidth and aPCRatio must be different then 0", aWidth != 0 && aPCRatio != 0));
        assert(("aWidth and aPCRatio must be different then 0", aWidth != 0 && aPCRatio != 0));
        // Create arrow pointing from (0,0,0) to (0,0,1)
        vector<CPoint> points ={};
        // TODO (ADAM) Why the fuck doesn't it work when inverting to 0!=i and removing continue????
        for (int j =-1; j<2; j++) { //start by creating an arrow that points up the z axis
            if (0==j) continue;
            for (int i=-1; i<2; i++) { //creating the points for the bottom square
                if (0==i) continue;
                points.push_back(CPoint(ARROW_WIDTH_FACTOR * j * aWidth, ARROW_WIDTH_FACTOR * i * aWidth, 0));
            }
            for (int k=-1; k<2; k++) { //creating the points for the top square
                if (0==k) continue;
                points.push_back(CPoint(ARROW_WIDTH_FACTOR * j * aWidth, ARROW_WIDTH_FACTOR * k * aWidth, (1 - aPCRatio)));
            }
        }
        for (int l=-1; l<2; l++) { //creating the points for the pointer
            if (0==l) continue;
            for (int i = -1; i<2; i++) {
                if (0==i) continue;
                points.push_back(CPoint(ARROW_LENGTH_FACTOR * l * aWidth, ARROW_LENGTH_FACTOR * i * aWidth, (1 - aPCRatio) ));
            }
        }
        points.push_back(CPoint(0, 0, 1)); //the pointer point

        //creating faces
        //creating the chest faces
        vector<CIndexedFace> faces={ CIndexedFace(vector<size_t>{0, 4, 5, 1}, aColor),
                                 CIndexedFace(vector<size_t>{2, 6, 7, 3}, aColor),
                                 CIndexedFace(vector<size_t>{0, 2, 6, 4}, aColor),
                                 CIndexedFace(vector<size_t>{4, 6, 7, 5}, aColor),
                                 CIndexedFace(vector<size_t>{5, 7, 3, 1}, aColor),
                                 CIndexedFace(vector<size_t>{1, 3, 2, 0}, aColor),
                                 //creating the pointer faces
                                 CIndexedFace(vector<size_t>{8,  10, 11, 9}, aColor),
                                 CIndexedFace(vector<size_t>{8,  12, 10}, aColor),
                                 CIndexedFace(vector<size_t>{10, 12, 11}, aColor),
                                 CIndexedFace(vector<size_t>{11, 12, 9}, aColor),
                                 CIndexedFace(vector<size_t>{9,  12, 8}, aColor) };

        CMesh mesh(points, faces, arLabel, aAlpha);


        // Scale the arrow by DirVec and later rotating the arrow to DirVec direction
        //CPoint direction_vec  = CPoint(DirVec[0],DirVec[1],DirVec[2]);
        auto direction_size = arDirVec.Magnitude();
        CPoint cross_vec = CPoint(0,0,1).Cross(arDirVec);
        CPoint normal_vec = cross_vec.Normalize();
        double rotation_angel = acos( CPoint(0,0,1).Dot(normal_vec)); //Note both vec are normalized so it's ok

        if (cross_vec.Magnitude() < 0.0001 ) // meaning direction_vec is on the z axis
        {
            if (arDirVec.Z() < 0)
            {
                direction_size = -1*direction_size;
            }
            mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
        }
        else{
            mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
            mesh.RotateMesh(normal_vec, rotation_angel);
        }

        mesh.MoveMesh(arCenter);
        return mesh;
    }

//CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, coord_t aColor, coord_t aAlpha, const string &arLabel)
//{
//    //before we run, lets check Exceptions
//    if (0 == arDirVec.Magnitude())  {
//        throw "Direction Vector cannot be (0,0,0)";
//    }
//    if (0 == aWidth || 0 == aPCRatio) {
//        throw "aWidth and aPCRatio must be different then 0";
//    }
////    assert(("Direction Vector cannot be (0,0,0)", aDirVec != vector<double>{0,0,0}));
////    assert(("aWidth and aPCRatio must be different then 0", aWidth != 0 && aPCRatio != 0));
//    // Create arrow pointing from (0,0,0) to (0,0,1)
//    vector<CPoint> points ={};
//
//    for (int i =-1; i < 2; i++) { //start by creating an arrow that points up the z axis
//        if (0 != i)  {
//            for (int j =-1; j < 2; j++) { //creating the points for the bottom square
//                if (0 != j) {
//                    points.push_back(CPoint(ARROW_FACTOR * i * aWidth, ARROW_FACTOR * j * aWidth, 0));
//                }
//            }
//        }
//        for (int k=-1; k<2; k++) { //creating the points for the top square
//            if (0 != k) {
//                points.push_back(CPoint(ARROW_FACTOR * i * aWidth, ARROW_FACTOR * k * aWidth, (1 - aPCRatio)));
//            }
//        }
//    }
//    for (int i=-1; i < 2; i++) { //creating the points for the pointer
//        if (0 != i) {
//            for (int j = -1; j < 2; j++) {
//                if (0 != j) {
//                    points.push_back(
//                            CPoint(2 * ARROW_FACTOR * i * aWidth, 2 * ARROW_FACTOR * j * aWidth, (1 - aPCRatio)));
//                }
//            }
//        }
//    }
//    points.push_back(CPoint(0, 0, 1)); //the pointer point
//
//    //creating faces
//    //creating the chest faces
//    vector<CIndexedFace> faces={ CIndexedFace(vector<size_t>{0, 4, 5, 1}, aColor),
//                                 CIndexedFace(vector<size_t>{2, 6, 7, 3}, aColor),
//                                 CIndexedFace(vector<size_t>{0, 2, 6, 4}, aColor),
//                                 CIndexedFace(vector<size_t>{4, 6, 7, 5}, aColor),
//                                 CIndexedFace(vector<size_t>{5, 7, 3, 1}, aColor),
//                                 CIndexedFace(vector<size_t>{1, 3, 2, 0}, aColor),
//                                 //creating the pointer faces
//                                 CIndexedFace(vector<size_t>{8,  10, 11, 9}, aColor),
//                                 CIndexedFace(vector<size_t>{8,  12, 10}, aColor),
//                                 CIndexedFace(vector<size_t>{10, 12, 11}, aColor),
//                                 CIndexedFace(vector<size_t>{11, 12, 9}, aColor),
//                                 CIndexedFace(vector<size_t>{9,  12, 8}, aColor) };
//
//    CMesh mesh(points, faces, arLabel, aAlpha);
//
//    // Scale the arrow by aDirVec and later rotating the arrow to aDirVec direction
//    auto direction_size = arDirVec.Magnitude();
//    CPoint cross_vec = CPoint(0,0,1).Cross(arDirVec);
//    CPoint normal_vec = cross_vec.Normalize();
//    double rotation_angel = acos( CPoint(0,0,1).Dot(normal_vec)); //Note both vec are normalized so it's ok
//
//    if (cross_vec.Magnitude() < 0.0001 ) // meaning direction_vec is on the z axis
//    {
//        if (arDirVec.Z() < 0)
//        {
//            direction_size = -1*direction_size;
//        }
//        mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
//    }
//    else{
//        mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
//        mesh.RotateMesh(normal_vec, rotation_angel);
//    }
//
//    mesh.MoveMesh(arCenter);
//    return mesh;
//}

} // namespace vivid


////this function applies a matrix so that the input points is rotated in a way that vector1 is equal to vector2.
//vector<CPoint> RotateMatchVectors(vector<CPoint> Points, vector<double> &Vector1, vector<double> &Vector2){
//    //start by using the cross product to get a vector thats gonna be our rotation base, ie we are going to rotate around it.



