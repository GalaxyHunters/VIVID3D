#include "Shapes.h"
#include <cassert>
#include <cmath>

using namespace vivid;
using namespace std;

constexpr coord_t ZERO_COMPARISON_THRESHOLD = 0.0001;
constexpr double ARROW_BASE = 0.4;
constexpr double ARROW_LENGTH = 0.8;

namespace vivid
{

CMesh CreateCubeMesh(const CPoint &arCenter, coord_t aSize, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    CMesh mesh = CreateBoxMesh(arCenter, CPoint(aSize, aSize, aSize), aColor, aAlpha, arLabel);
    return mesh;
}

CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    vector<CPoint> points= { CPoint(-1, -1, -1),
                             CPoint(-1, -1, +1),
                             CPoint(-1, +1, -1),
                             CPoint(-1, +1, +1),
                             CPoint(+1, -1, -1),
                             CPoint(+1, -1, +1),
                             CPoint(+1, +1, -1),
                             CPoint(+1, +1, +1)      };

    vector<CIndexedFace> faces = { CIndexedFace(vector<size_t>{3, 2, 6, 7}, aColor),
                                   CIndexedFace(vector<size_t>{6, 2, 0, 4}, aColor),
                                   CIndexedFace(vector<size_t>{7, 6, 4, 5}, aColor),
                                   CIndexedFace(vector<size_t>{3, 7, 5, 1}, aColor),
                                   CIndexedFace(vector<size_t>{2, 3, 1, 0}, aColor),
                                   CIndexedFace(vector<size_t>{1, 5, 4, 0}, aColor)     };


    CMesh mesh(points, faces, arLabel, aAlpha);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh(arSize);

    return mesh;
}

CMesh CreateSphereMesh(const CPoint &arCenter, coord_t aRadius, size_t aNumOfMeridians, size_t aNumOfParallels, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    vector<CPoint> points;
    vector<CIndexedFace> faces;

    /* Adds points around sphere*/
    points.push_back(CPoint(0, 0,  1)); //Creating the top polar
    points.push_back(CPoint(0, 0, -1)); //Creating the bottom polar

    double phi_step = M_PI / (aNumOfParallels + 1);
    double theta_step = (2 * M_PI) / aNumOfMeridians;

    for (int i = 0; i < aNumOfMeridians; i++)
    {
        double theta = i * theta_step;
        for (int j = 1; j < aNumOfParallels + 1; j++)
        {
            double phi = (M_PI / 2) - (j * phi_step);
            points.push_back(CPoint(cos(phi) * cos(theta), cos(phi) * sin(theta), sin(phi)));    //creating the points
        }
    }

    /* Add triangle faces around top point (0, 0, 1) */
    for (int i = 0; i < aNumOfMeridians - 1; i++)
    {
        faces.push_back(CIndexedFace(vector<size_t>{0, 2 + (i + 1) * aNumOfParallels, 2 + i * aNumOfParallels}, aColor)); //creates triangles
    }
    faces.push_back(CIndexedFace(vector<size_t>{0, 2, 2 + (aNumOfMeridians - 1) * aNumOfParallels}, aColor));

    /* Add rectangular faces around center */
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
    /* Add triangle faces around bottom point (0, 0, -1) */
    for (int i = 0; i < aNumOfMeridians - 1; i++)
    {
        faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (i + 1) * aNumOfParallels, 1 + (i + 2) * aNumOfParallels}, aColor));
    }
    faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (aNumOfMeridians) * aNumOfParallels, 1 + aNumOfParallels}, aColor));

    CMesh mesh(points, faces, arLabel, aAlpha);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh(CPoint(aRadius, aRadius, aRadius));

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
CMesh CreateEllipsoidByTransformMesh(const CPoint &arCenter, const CPoint &arScaleVec, size_t aNumOfMeridians, size_t aNumOfParallels, vector<coord_t> aMajorAxis, vector<coord_t> aMiddleAxis, vector<coord_t> aMinorAxis, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    /* Asserting conditions */
    if (aMajorAxis == vector<double>{0,0,0} || aMiddleAxis == vector<double>{0,0,0} || aMinorAxis == vector<double>{0,0,0}) {
        throw "Axis vectors cannot be (0,0,0)";
    }
//    if (!CheckIfPerpindicular(aMajorAxis, aMiddleAxis)) {
//        throw "Axis vectors must be perpendicular";
//    }
//    if (!CheckIfPerpindicular(aMiddleAxis, aMinorAxis)) {
//        throw "Axis vectors must be perpendicular";
//    }
//    if (!CheckIfPerpindicular(aMajorAxis, aMinorAxis)) {
//        throw "Axis vectors must be perpendicular";
//    }
    if (CPoint(aMajorAxis).Dot(CPoint(aMiddleAxis)) < ZERO_COMPARISON_THRESHOLD) {
        throw "Axis vectors must be perpendicular";
    }
//    if (CPoint(aMajorAxis[0], aMajorAxis[1], aMajorAxis[2]).Dot(CPoint(aMinorAxis[0], aMinorAxis[1], aMinorAxis[2])) < ZERO_COMPARISON_THRESHOLD) {
//        throw "Axis vectors must be perpendicular";
//    }
//    if (CPoint(aMinorAxis[0], aMinorAxis[1], aMinorAxis[2]).Dot(CPoint(aMiddleAxis[0], aMiddleAxis[1], aMiddleAxis[2])) < ZERO_COMPARISON_THRESHOLD) {
//        throw "Axis vectors must be perpendicular";
//    }

    cout << "Passed Checks " << endl;
    CMesh Ellipsoid = CreateSphereMesh(arCenter, 1.0, aNumOfMeridians, aNumOfParallels, aColor, aAlpha, arLabel);
    cout << "Created Ellipsoid " << endl;
    //use arScaleVec to create strech matrix and stretch the sphere
    Ellipsoid.ScaleMesh(arScaleVec);
    cout << "Stretched Ellipsoid " << endl;
    // use Axis vecs to rotate the Ellipsoid by matrix multiplication
    coord_t aMat[3][3];
    aMat[0][0] = aMajorAxis[0]; aMat[0][1] = aMiddleAxis[0]; aMat[0][2] = aMinorAxis[0];
    aMat[1][0] = aMajorAxis[1]; aMat[1][1] = aMiddleAxis[1]; aMat[1][2] = aMinorAxis[1];
    aMat[2][0] = aMajorAxis[2]; aMat[2][1] = aMiddleAxis[2]; aMat[2][2] = aMinorAxis[2];
    Ellipsoid.TransformMesh(aMat);
    cout << "Rotated Ellipsoid " << endl;

    return Ellipsoid;
}

// TODO ask Zohar what kind of arrow is it
// TODO, THINK about Jill Neiman's arrow and the scale thingy
CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, coord_t aColor, coord_t aAlpha, const string &arLabel)
{
    //before we run, lets check Exceptions
    if (0 == arDirVec.Magnitude())  {
        throw "Direction Vector cannot be (0,0,0)";
    }
    if (0 == aWidth || 0 == aPCRatio) {
        throw "aWidth and aPCRatio must be different then 0";
    }
//    assert(("Direction Vector cannot be (0,0,0)", aDirVec != vector<double>{0,0,0}));
//    assert(("aWidth and aPCRatio must be different then 0", aWidth != 0 && aPCRatio != 0));
    // Create arrow pointing from (0,0,0) to (0,0,1)
    vector<CPoint> points ={};

    for (int i =-1; i < 2; i++) { //start by creating an arrow that points up the z axis
        if (0 != i) {
            for (int j = -1; j < 2; j++) { //creating the points for the bottom square
                if (0 != j) {
                    points.push_back(CPoint(ARROW_BASE * i * aWidth, ARROW_BASE * j * aWidth, 0));
                }
            }
            for (int k = -1; k < 2; k++) { //creating the points for the top square
                if (0 != k) {
                    points.push_back(CPoint(ARROW_BASE * i * aWidth, ARROW_BASE * k * aWidth, (1 - aPCRatio)));
                }
            }
        }
    }
    for (int i=-1; i < 2; i++) { //creating the points for the pointer
        if (0 != i) {
            for (int j = -1; j < 2; j++) {
                if (0 != j) {
                    points.push_back(
                            CPoint(ARROW_LENGTH * i * aWidth, ARROW_LENGTH * j * aWidth, (1 - aPCRatio)));
                }
            }
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

    // Scale the arrow by aDirVec and later rotating the arrow to aDirVec direction
    auto direction_size = arDirVec.Magnitude();
    CPoint cross_vec = CPoint(0,0,1).Cross(arDirVec);
    CPoint normal_vec = cross_vec.Normalize();
    double rotation_angel = acos( CPoint(0,0,1).Dot(normal_vec)); //Note both vec are normalized so it's ok

    if (cross_vec.Magnitude() < ZERO_COMPARISON_THRESHOLD ) // meaning direction_vec is on the z axis
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

} // namespace vivid


////this function applies a matrix so that the input points is rotated in a way that vector1 is equal to vector2.
//vector<CPoint> RotateMatchVectors(vector<CPoint> Points, vector<double> &Vector1, vector<double> &Vector2){
//    //start by using the cross product to get a vector thats gonna be our rotation base, ie we are going to rotate around it.



