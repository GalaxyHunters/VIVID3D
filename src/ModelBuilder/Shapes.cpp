#include "Shapes.h"
#include <cmath>

using namespace vivid;
using namespace std;

constexpr coord_t ZERO_COMPARISON_THRESHOLD = 0.0001;
constexpr double ARROW_BASE = 0.4;
constexpr double ARROW_LENGTH = 0.8;

namespace vivid
{

CMesh CreatePlaneMesh(const CPoint &arCenter, coord_t aSize, const string& arColor, normal_float aOpacity, const std::string &arLabel) {
    vector<CPoint> points= { CPoint(-1, -1, 0),
                             CPoint(-1, 1, 0),
                             CPoint(1, -1, 0),
                             CPoint(1, 1, 0)     };

    vector<CFace> faces = {CFace({0, 1, 2, 3}, 0)};


    CMesh mesh(points, faces, arLabel, aOpacity);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh({aSize, aSize, 0});
    mesh.SetColor(arColor);

    return mesh;
}

CMesh CreateCubeMesh(const CPoint &arCenter, coord_t aSize, const string& arColor, normal_float aOpacity, const string &arLabel)
{
    CMesh mesh = CreateBoxMesh(arCenter, {aSize, aSize, aSize}, arColor, aOpacity, arLabel);
    return mesh;
}

CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, const string& arColor, normal_float aOpacity, const string &arLabel)
{
    vector<CPoint> points= { CPoint(-1, -1, -1),
                             CPoint(-1, -1, +1),
                             CPoint(-1, +1, -1),
                             CPoint(-1, +1, +1),
                             CPoint(+1, -1, -1),
                             CPoint(+1, -1, +1),
                             CPoint(+1, +1, -1),
                             CPoint(+1, +1, +1)      };

    vector<CFace> faces = {CFace(vector<size_t>{3, 2, 6, 7}, 0),
                           CFace(vector<size_t>{6, 2, 0, 4}, 0),
                           CFace(vector<size_t>{7, 6, 4, 5}, 0),
                           CFace(vector<size_t>{3, 7, 5, 1}, 0),
                           CFace(vector<size_t>{2, 3, 1, 0}, 0),
                           CFace(vector<size_t>{1, 5, 4, 0}, 0)     };


    CMesh mesh(points, faces, arLabel, aOpacity);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh(arSize);
    mesh.SetColor(arColor);

    return mesh;
}

CMesh CreateSphereMesh(const CPoint &arCenter, coord_t aRadius, size_t aNumOfMeridians, size_t aNumOfParallels, const string& arColor, normal_float aOpacity, const string &arLabel)
{
    vector<CPoint> points;
    vector<CFace> faces;

    /* Adds points around sphere*/
    points.emplace_back(0, 0,  1); //Creating the top polar
    points.emplace_back(0, 0, -1); //Creating the bottom polar

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
        faces.push_back(CFace(vector<size_t>{0, 2 + (i + 1) * aNumOfParallels, 2 + i * aNumOfParallels}, 0)); //creates triangles
    }
    faces.push_back(CFace(vector<size_t>{0, 2, 2 + (aNumOfMeridians - 1) * aNumOfParallels}, 0));

    /* Add rectangular faces around center */
    for (int i = 2; i < 1 + aNumOfParallels; i++)
    {
        for (int j = 0; j < aNumOfMeridians - 1; j++)
        {
            faces.push_back(CFace({i + j * aNumOfParallels, i + (j + 1) * aNumOfParallels,
                                                 i + 1 + (j + 1) * aNumOfParallels, i + 1 + j * aNumOfParallels}, 0));
        }
        faces.push_back(CFace({i + (aNumOfMeridians - 1) * aNumOfParallels,
                                             (size_t)i, (size_t)(i + 1),
                                             i + 1 + (aNumOfMeridians - 1) * aNumOfParallels}, 0));
    }
    /* Add triangle faces around bottom point (0, 0, -1) */
    for (int i = 0; i < aNumOfMeridians - 1; i++)
    {
        faces.push_back(CFace({1, 1 + (i + 1) * aNumOfParallels, 1 + (i + 2) * aNumOfParallels}, 0));
    }
    faces.push_back(CFace({1, 1 + (aNumOfMeridians) * aNumOfParallels, 1 + aNumOfParallels}, 0));

    CMesh mesh(points, faces, arLabel, aOpacity);
    mesh.MoveMesh(arCenter);
    mesh.ScaleMesh({aRadius, aRadius, aRadius});
    mesh.SetColor(arColor);
    
    return mesh;
}

CMesh CreateEllipsoidMesh(const CPoint &arCenter, const CPoint &arScaleVec, size_t aNumOfMeridians, size_t aNumOfParallels, const CPoint &arMajorAxis, const CPoint &arMiddleAxis, const CPoint &arMinorAxis, const string& arColor, normal_float aOpacity, const string &arLabel)
{
    /* Asserting conditions */
    if (!(arMajorAxis.Orthogonal(arMiddleAxis) && arMinorAxis.Orthogonal(arMiddleAxis) && arMajorAxis.Orthogonal(arMinorAxis))) {
        throw "Axis vectors must be perpendicular";
    }

    CMesh Ellipsoid = CreateSphereMesh(arCenter, 1.0, aNumOfMeridians, aNumOfParallels, arColor, aOpacity, arLabel);
    Ellipsoid.ScaleMesh(arScaleVec); //Scaling before the rotation
    array<CPoint, 3> transform_mat = {
            CPoint(arMajorAxis.X(), arMiddleAxis.X(), arMinorAxis.X()),
            CPoint(arMajorAxis.Y(), arMiddleAxis.Y(), arMinorAxis.Y()),
            CPoint(arMajorAxis.Z(), arMiddleAxis.Z(), arMinorAxis.Z())
    };
    Ellipsoid.TransformMesh(transform_mat);

    return Ellipsoid;
}

CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, const string& arColor, normal_float aOpacity, const string &arLabel)
{
    //before we run, lets check Exceptions
    if (ZERO_COMPARISON_THRESHOLD >= arDirVec.Magnitude())  {
        throw "Direction Vector cannot be (0,0,0)";
    }
    if (ZERO_COMPARISON_THRESHOLD >= aWidth || ZERO_COMPARISON_THRESHOLD >= aPCRatio) {
        throw "aWidth and aPCRatio must be different then 0";
    }

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
    vector<CFace> faces={CFace(vector<size_t>{0, 4, 5, 1}, 0),
                         CFace(vector<size_t>{2, 6, 7, 3}, 0),
                         CFace(vector<size_t>{0, 2, 6, 4}, 0),
                         CFace(vector<size_t>{4, 6, 7, 5}, 0),
                         CFace(vector<size_t>{5, 7, 3, 1}, 0),
                         CFace(vector<size_t>{1, 3, 2, 0}, 0),
            //creating the pointer faces
                         CFace(vector<size_t>{8, 10, 11, 9}, 0),
                         CFace(vector<size_t>{8, 12, 10}, 0),
                         CFace(vector<size_t>{10, 12, 11}, 0),
                         CFace(vector<size_t>{11, 12, 9}, 0),
                         CFace(vector<size_t>{9, 12, 8}, 0) };

    CMesh mesh(points, faces, arLabel, aOpacity);

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
    mesh.SetColor(arColor);

    return mesh;
}


pair<CLines, CLines> CreateGrid(coord_t aScale, size_t aNumOfTicks, coord_t aTickSize) {
    aScale = round(aScale);
    pair<CLines, CLines> grid_lines;
    // Major axes
    grid_lines.first = CLines({{CPoint(-aScale, 0, 0), CPoint(aScale, 0, 0)},
                               {CPoint(0,0,-aScale),   CPoint(0, 0, aScale)},
                               {CPoint(0, -aScale, 0), CPoint(0, aScale, 0)}}, 1., "GridLines");

    // Add x,y,z tick lines
    vector<vector<CPoint>> tick_lines;
    coord_t increase_by = aScale/(double)aNumOfTicks;

    for (int i = 1; i <= aNumOfTicks; i++) {
        // Drawing ticks equally on + and -, along xyz
        for (int j = -1; j <= 1; j++){
            coord_t pos = j * i * increase_by;
            tick_lines.push_back({CPoint(-aTickSize, pos, 0), CPoint(aTickSize, pos, 0)});
            tick_lines.push_back({CPoint(-aTickSize, 0, pos), CPoint(aTickSize, 0, pos)});
            tick_lines.push_back({CPoint(pos, -aTickSize, 0), CPoint(pos, aTickSize, 0)});
            tick_lines.push_back({CPoint(0, -aTickSize, pos), CPoint(0, aTickSize, pos)});
            tick_lines.push_back({CPoint(pos, 0, -aTickSize), CPoint(pos, 0, aTickSize)});
            tick_lines.push_back({CPoint(0, pos, -aTickSize), CPoint(0, pos, aTickSize)});
        }
    }
    grid_lines.second = CLines(tick_lines, 0.15, "GridTicks");

    return grid_lines;
}

} // namespace vivid


////this function applies a matrix so that the input points is rotated in a way that vector1 is equal to vector2.
//vector<CPoint> RotateMatchVectors(vector<CPoint> Points, vector<double> &Vector1, vector<double> &Vector2){
//    //start by using the cross product to get a vector thats gonna be our rotation base, ie we are going to rotate around it.


