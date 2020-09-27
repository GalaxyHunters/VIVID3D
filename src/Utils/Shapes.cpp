#include "Shapes.h"
//#include "Animation.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;


const size_t a1 = NULL;
const size_t b1 = NULL;

//TODO make position a cpoint object
CMesh CreateCubeMesh(double sizeX, double sizeY, double sizeZ, coord_t color, coord_t alpha, vector<double> position)
{
	vector<CPoint> CPoints(8);
	CPoints.clear();
	CPoints.push_back(CPoint(position[0] - 0.5*sizeX, position[1] + 0.5*sizeY, position[2] + 0.5*sizeZ)); //0
	CPoints.push_back(CPoint(position[0] + 0.5*sizeX, position[1] + 0.5*sizeY, position[2] + 0.5*sizeZ)); //1
	CPoints.push_back(CPoint(position[0] + 0.5*sizeX, position[1] + 0.5*sizeY, position[2] - 0.5*sizeZ)); //2
	CPoints.push_back(CPoint(position[0] - 0.5*sizeX, position[1] + 0.5*sizeY, position[2] - 0.5*sizeZ)); //3
	CPoints.push_back(CPoint(position[0] + 0.5*sizeX, position[1] - 0.5*sizeY, position[2] - 0.5*sizeZ)); //4
	CPoints.push_back(CPoint(position[0] - 0.5*sizeX, position[1] - 0.5*sizeY, position[2] - 0.5*sizeZ)); //5
	CPoints.push_back(CPoint(position[0] - 0.5*sizeX, position[1] - 0.5*sizeY, position[2] + 0.5*sizeZ)); //6
	CPoints.push_back(CPoint(position[0] + 0.5*sizeX, position[1] - 0.5*sizeY, position[2] + 0.5*sizeZ)); //7

	vector<CIndexedFace> faces(6);

	faces[0] = CIndexedFace(vector<size_t>{0, 3, 2, 1}, color);
	faces[1] = CIndexedFace(vector<size_t>{2, 3, 5, 4}, color);
	faces[2] = CIndexedFace(vector<size_t>{1, 2, 4, 7}, color);
	faces[3] = CIndexedFace(vector<size_t>{0, 1, 7, 6}, color);
	faces[4] = CIndexedFace(vector<size_t>{3, 0, 6, 5}, color);
	faces[5] = CIndexedFace(vector<size_t>{6, 7, 4, 5}, color);

    CMesh CMesh;
    CMesh.SetPoints(CPoints);
    CMesh.SetFaces(faces);
    CMesh.SetAlpha(alpha);
    CMesh.SetLabel("cube");

	return CMesh;
}

CMesh CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, vector<double> CenterPoint, coord_t Color, coord_t Alpha, string Label)
{
    vector<CPoint> vertices;
    vector<CIndexedFace> faces;
    double Theta;
    double Phi;
    double CenterX = CenterPoint[0];
    double CenterY = CenterPoint[1];
    double CenterZ = CenterPoint[2];


    vertices.push_back(CPoint(CenterX, CenterY, CenterZ + radius)); //Creating the top polar
    vertices.push_back(CPoint(CenterX, CenterY, CenterZ - radius)); //Creating the bottom polar

    double PhiStep = M_PI / (num_of_parallels + 1);
    double ThetaStep = (2 * M_PI) / num_of_meridians;

    for (int j = 0; j < num_of_meridians; ++j)
    {
        Theta = j * ThetaStep;
        for (int k = 1; k < num_of_parallels + 1; ++k)
        {
            Phi = (M_PI / 2) - (k * PhiStep);
            vertices.push_back(CPoint(CenterX + radius * cos(Phi) * cos(Theta), CenterY + radius * cos(Phi) * sin(Theta), CenterZ + radius * sin(Phi)));    //creating the vertices
        }
    }

    for (int l = 0; l < num_of_meridians - 1; ++l)
    {
        faces.push_back(CIndexedFace(vector<size_t>{0, 2 + (l + 1) * num_of_parallels, 2 + l * num_of_parallels}, Color)); //creates triangles
    }
    faces.push_back(CIndexedFace(vector<size_t>{0, 2, 2 + (num_of_meridians - 1) * num_of_parallels}, Color));

    for (int m = 2; m < 1 + num_of_parallels; ++m)
    {
        for (int l = 0; l < num_of_meridians - 1; ++l)
        {
            faces.push_back(CIndexedFace(vector<size_t>{m + l * num_of_parallels, m + (l + 1) * num_of_parallels, m + 1 + (l + 1) * num_of_parallels, m + 1 + l * num_of_parallels}, Color));
        }
        faces.push_back(CIndexedFace(vector<size_t>{m + (num_of_meridians - 1) * num_of_parallels, static_cast<unsigned long long>(m), static_cast<unsigned long long>(m + 1), m + 1 + (num_of_meridians - 1) * num_of_parallels}, Color));
    }

    for (int i1 = 0; i1 < num_of_meridians - 1; ++i1)
    {
        faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (i1 + 1) * num_of_parallels, 1 + (i1 + 2) * num_of_parallels}, Color));
    }
    faces.push_back(CIndexedFace(vector<size_t>{1, 1 + (num_of_meridians) * num_of_parallels, 1 + num_of_parallels}, Color));

    CMesh CMesh;
    CMesh.SetPoints(vertices);
    CMesh.SetFaces(faces);
    CMesh.SetAlpha(Alpha);
    CMesh.SetLabel(Label);
	return CMesh;
}
//

bool CheckIfPerpindicular(vector<double> VecA, vector<double> VecB){
    if (VecA[0] * VecB[0] + VecA[1] * VecB[1] + VecA[2] * VecB[2] == 0) return true;
    else return false;
}

CMesh CreateEllipsoidMesh(size_t NumOfMeridians, size_t NumOfParallels, vector<double> Radii, vector<double> CenterPoint, vector<double> MajorAxis, vector<double> MiddleAxis, vector<double> MinorAxis, coord_t Color, coord_t Alpha, string Label){
    //begin by asserting some conditions
    assert(("Axis vectors cannot be (0,0,0)", MajorAxis != vector<double>{0,0,0} && MiddleAxis != vector<double>{0,0,0} && MinorAxis != vector<double>{0,0,0}));
    assert(("Radius cannot be equal to 0", Radii[0] != 0 && Radii[1] != 0 && Radii[2] != 0));
    assert(("Bro did you just input vectors that arent perpendicular? bro ngl thats kinda cringe", CheckIfPerpindicular(MajorAxis, MiddleAxis) == true && CheckIfPerpindicular(MajorAxis, MinorAxis) == true && CheckIfPerpindicular(MiddleAxis, MinorAxis) == true));
    //create a sphere to operate on
    CMesh Ellipsoid = CreateSphereMesh(NumOfMeridians, NumOfParallels, 1,  vector<double>{0,0,0}, Color, Alpha, Label);
    //use Radii to create strech matrix and stretch the sphere
    vector<vector<double>> StretchMatrix;
    StretchMatrix.push_back(vector<double>{Radii[0], 0, 0});
    StretchMatrix.push_back(vector<double>{0, Radii[1], 0});
    StretchMatrix.push_back(vector<double>{0, 0, Radii[2]});
    //use all axis vectors to create a rotation matrix
    vector<vector<double>> RotateMatrix;
    RotateMatrix.push_back(MajorAxis);
    RotateMatrix.push_back(MiddleAxis);
    RotateMatrix.push_back(MinorAxis);
    //apply matrices to points
    vector<CPoint> points = Ellipsoid.GetPoints();
    for (int i1 = 0; i1 < points.size(); ++i1)
    {
        CPoint& CurrentVector = points[i1];
        //right now im using a refrence to change the vector according to the matrix
        double CVX = CurrentVector.GetX();
        double CVY = CurrentVector.GetY();
        double CVZ = CurrentVector.GetZ();
        //first we strech em
        CurrentVector = CPoint(CVX * StretchMatrix[0][0] + CVY * StretchMatrix[1][0] + CVZ * StretchMatrix[2][0],
                               CVX * StretchMatrix[0][1] + CVY * StretchMatrix[1][1] + CVZ * StretchMatrix[2][1],
                               CVX * StretchMatrix[0][2] + CVY * StretchMatrix[1][2] + CVZ * StretchMatrix[2][2]);
        //now we rotate em
        CVX = CurrentVector.GetX();
        CVY = CurrentVector.GetY();
        CVZ = CurrentVector.GetZ();
        CurrentVector = CPoint(CenterPoint[0] + CVX * RotateMatrix[0][0] + CVY * RotateMatrix[1][0] + CVZ * RotateMatrix[2][0],
                               CenterPoint[1] + CVX * RotateMatrix[0][1] + CVY * RotateMatrix[1][1] + CVZ * RotateMatrix[2][1],
                               CenterPoint[2] + CVX * RotateMatrix[0][2] + CVY * RotateMatrix[1][2] + CVZ * RotateMatrix[2][2]);
    }

    Ellipsoid.SetPoints(points);
    return Ellipsoid;
    //set new info to the mesh

}




//struct MathVector{double size;
//    vector<double> direction;};

inline double Calc3DVectorSize(vector<double> &Vector){ return sqrt(pow(Vector[0], 2) + pow(Vector[1], 2) + pow(Vector[2], 2)) ;}
inline double VectorsDotProduct3D(vector<double> &Vector1, vector<double> &Vector2){ return (Vector1[0] * Vector2[0] + Vector1[1] * Vector2[1] + Vector1[2] * Vector2[2]);}
inline vector<double> VectorsCrossProduct3D(vector<double> &Vector1, vector<double> &Vector2){return vector<double>{Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1],
                                                                                                                    Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2],
                                                                                                                    Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0]};}
inline vector<double> Normalize3DVector(vector<double> &Vector){
    double VectorSize = Calc3DVectorSize(Vector);
    for (int l = 0; l < Vector.size(); ++l) {
        Vector[l] /= VectorSize;
    }
    return Vector;
}

//this function applies a matrix so that the input points is rotated in a way that vector1 is equal to vector2.
vector<CPoint> RotateMatchVectors(vector<CPoint> Points, vector<double> &Vector1, vector<double> &Vector2){
    //start by using the cross product to get a vector thats gonna be our rotation base, ie we are going to rotate around it.
     vector<double> RotVector;
     RotVector = VectorsCrossProduct3D(Vector1, Vector2);
    //now we have a vector to rotate around. we normalize its size (turning it to 1).
    Normalize3DVector(RotVector);
    //compute our rotation angle theta
    double Theta = acos(float(VectorsDotProduct3D(Vector1, Vector2))/(Calc3DVectorSize(Vector1) * Calc3DVectorSize(Vector2)));
    //build the matrix
    double Ux = RotVector[0];
    double Uy = RotVector[1];
    double Uz = RotVector[2];
    vector<vector<double>> RotMatrix{vector<double>{cos(Theta) + pow(Ux, 2) * (1 - cos(Theta)),
                                                    Uy * Ux * (1 - cos(Theta) + Uz * sin(Theta)),
                                                    Uz * Ux * (1 - cos(Theta)) - Uy * sin(Theta)},
                                     vector<double>{Ux * Uy * (1 - cos(Theta)) - Uz * sin(Theta),
                                                    cos(Theta) + pow(Uy, 2) * (1 - cos(Theta)),
                                                    Uz * Uy * (1 - cos(Theta) + Ux * sin(Theta))},
                                     vector<double>{Ux * Uz * (1 - cos(Theta)) + Uy * sin(Theta),
                                                    Uy * Uz * (1 - cos(Theta)) - Ux * sin(Theta),
                                                    cos(Theta) + pow(Uz, 2) * (1 - cos(Theta))}};
    //apply matrix to points
        for (int i1 = 0; i1 < Points.size(); ++i1)
    {
        CPoint& CurrentVector = Points[i1];
        //right now im using a refrence to change the vector according to the matrix
        double CVX = CurrentVector.GetX();
        double CVY = CurrentVector.GetY();
        double CVZ = CurrentVector.GetZ();

        //now we rotate em
        CurrentVector = CPoint(CVX * RotMatrix[0][0] + CVY * RotMatrix[0][1] + CVZ * RotMatrix[0][2],
                               CVX * RotMatrix[1][0] + CVY * RotMatrix[1][1] + CVZ * RotMatrix[1][2],
                               CVX * RotMatrix[2][0] + CVY * RotMatrix[2][1] + CVZ * RotMatrix[2][2]);
    }

    return Points;
}



CMesh CreateArrowMesh(double Length, double Width, double PCRatio, vector<double> Base, vector<double> DirVec, double Color, double Alpha, string Label){
    //before we run, lets assert some conditions
    assert(("Direction Vector cannot be (0,0,0)", DirVec != vector<double>{0,0,0}));
    assert(("Length, Width and PCRatio must be diffrent then 0", Length != 0 && Width != 0 && PCRatio != 0));

    CMesh arrow;
    vector<CPoint> points;
    vector<CIndexedFace> faces;
    double &baseX = Base[0];
    double &baseY = Base[1];
    double &baseZ = Base[2];
    //create points for the vector
    //start by creating an arrow that points up the z axis


    for (int j = -1; j < 2; ++j) {
        if (j == 0) continue;
        //creating the points for the bottom square
        for (int i = -1; i < 2; ++i) {
            if (i == 0) continue;
//            cout << i << endl;
            points.push_back(CPoint(baseX + 0.5 * j * Width, baseZ, baseY + 0.5 * i * Width));
        }
        //creating the points for the top square
        for (int k = -1; k < 2; ++k) {
            if (k == 0) continue;
            points.push_back(CPoint(baseX + 0.5 * j * Width, baseZ + Length * (1 - PCRatio), baseY + 0.5 * k * Width));
        }
    }
    //creating the points for the pointer
    for (int l = -1; l < 2; ++l) {
        if (l == 0) continue;
        for (int i = -1; i < 2; ++i) {
            if (i == 0) continue;
//            cout << i << endl;
            points.push_back(CPoint(baseX + 0.75 * l * Width, baseZ + Length * (1 - PCRatio), baseY + 0.75 * i * Width));
        }
    }
    //the pointer point
    points.push_back(CPoint(baseX, baseZ + Length, baseY));

    //creating faces
        //creating the chest faces
    faces.push_back(CIndexedFace(vector<size_t>{0, 4, 5, 1}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{2, 6, 7, 3}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{0, 2, 6, 4}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{4, 6, 7, 5}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{5, 7, 3, 1}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{1, 3, 2, 0}, Color));
        //creating the pointer faces
    faces.push_back(CIndexedFace(vector<size_t>{8, 10, 11, 9}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{8, 12, 10}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{10, 12, 11}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{11, 12, 9}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{9, 12, 8}, Color));
    //rotating the arrow using a rotation matrix

//    points = RotateMatchVectors(points, vector<double>{0, 0, 1}, DirVec);

    arrow.SetPoints(points);
    arrow.SetFaces(faces);
    arrow.SetAlpha(Alpha);
    arrow.SetLabel(Label);
    return arrow;
}


//CMesh CreateArrowMesh(double x, double y, double z, double chest_length, double pointer_length, double width, coord_t color)
//{
//    vector<CPoint> points;
//    points.clear();
//    points.push_back(CPoint(x, y, z));
//    points.push_back(CPoint(x + (1.5 * width), y, z - (1.5 * pointer_length)));
//    points.push_back(CPoint(x, y + (1.5 * width), z - (1.5 * pointer_length)));
//    points.push_back(CPoint(x - (1.5 * width), y, z - (1.5 * pointer_length)));
//    points.push_back(CPoint(x, y  - (1.5 * width), z - (1.5 * pointer_length)));
//
//    points.push_back(CPoint(x - width, y, z - pointer_length));
//    points.push_back(CPoint(x + width, y, z - pointer_length));
//    points.push_back(CPoint(x, y - width, z - pointer_length));
//    points.push_back(CPoint(x, y  + width, z - pointer_length));
//
//    points.push_back(CPoint(x - width, y, z - pointer_length - chest_length));
//    points.push_back(CPoint(x + width, y, z - pointer_length - chest_length));
//    points.push_back(CPoint(x, y - width, z - pointer_length - chest_length));
//    points.push_back(CPoint(x, y + width, z - pointer_length - chest_length));
//
//    vector<CIndexedFace> faces(10);
////    faces.clear();
//    faces[0] = CIndexedFace(vector<size_t>{1, 2, 3, 4}, color);
//    faces[1] = CIndexedFace(vector<size_t>{0, 1, 2}, color);
//    faces[2] = CIndexedFace(vector<size_t>{0, 2, 3}, color);
//    faces[3] = CIndexedFace(vector<size_t>{0, 3, 4}, color);
//    faces[4] = CIndexedFace(vector<size_t>{0, 4, 1}, color);
//
//    faces[5] = CIndexedFace(vector<size_t>{5, 9, 12, 8}, color);
//    faces[6] = CIndexedFace(vector<size_t>{8, 12, 10, 6}, color);
//    faces[7] = CIndexedFace(vector<size_t>{6, 10, 11, 7}, color);
//    faces[8] = CIndexedFace(vector<size_t>{7, 11, 9, 5}, color);
//
//    CMesh arrow;
//    arrow.SetPoints(points);
//    arrow.SetFaces(faces);
//    arrow.SetAlpha(1);
//    arrow.SetLabel("arrow");
//
//    return arrow;
//}

//FbxMesh* createCube(int sizeX, int sizeY, int sizeZ)
//{
//	FbxManager* manager = FbxManager::Create();
//	//FbxScene* scene = FbxScene::Create(manager, "");
//	//FbxNode* sceneRootNode = scene->GetRootNode();
//	FbxMesh* CMesh0 = FbxMesh::Create(manager, "CMesh0");			//creates the CMesh
//	//creating vertices for the CMesh
//	FbxVector4 vertex0(-sizeX, -sizeY, sizeZ);
//	FbxVector4 vertex1(sizeX, -sizeY, sizeZ);
//	FbxVector4 vertex2(sizeX, sizeY, sizeZ);
//	FbxVector4 vertex3(-sizeX, sizeY, sizeZ);
//	FbxVector4 vertex4(-sizeX, -sizeY, -sizeZ);
//	FbxVector4 vertex5(sizeX, -sizeY, -sizeZ);
//	FbxVector4 vertex6(sizeX, sizeY, -sizeZ);
//	FbxVector4 vertex7(-sizeX, sizeY, -sizeZ);
//
//	//CMeshNode0->ConnectSrcObject(CMesh0);
//	CMesh0->InitControlPoints(24);								//Initialize the control CPoint array of the CMesh.
//	FbxVector4* CMesh0ControlCPoints = CMesh0->GetControlPoints();		//
//
//	// Define each face of the cube.
//	// Face 1
//	CMesh0ControlCPoints[0] = vertex0;
//	CMesh0ControlCPoints[1] = vertex1;
//	CMesh0ControlCPoints[2] = vertex2;
//	CMesh0ControlCPoints[3] = vertex3;
//	// Face 2
//	CMesh0ControlCPoints[4] = vertex1;
//	CMesh0ControlCPoints[5] = vertex5;
//	CMesh0ControlCPoints[6] = vertex6;
//	CMesh0ControlCPoints[7] = vertex2;
//	// Face 3
//	CMesh0ControlCPoints[8] = vertex5;
//	CMesh0ControlCPoints[9] = vertex4;
//	CMesh0ControlCPoints[10] = vertex7;
//	CMesh0ControlCPoints[11] = vertex6;
//	// Face 4
//	CMesh0ControlCPoints[12] = vertex4;
//	CMesh0ControlCPoints[13] = vertex0;
//	CMesh0ControlCPoints[14] = vertex3;
//	CMesh0ControlCPoints[15] = vertex7;
//	// Face 5
//	CMesh0ControlCPoints[16] = vertex3;
//	CMesh0ControlCPoints[17] = vertex2;
//	CMesh0ControlCPoints[18] = vertex6;
//	CMesh0ControlCPoints[19] = vertex7;
//	// Face 6
//	CMesh0ControlCPoints[20] = vertex1;
//	CMesh0ControlCPoints[21] = vertex0;
//	CMesh0ControlCPoints[22] = vertex4;
//	CMesh0ControlCPoints[23] = vertex5;
//
//	int i, j;
//	int polygonVertices0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
//		14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
//	for (i = 0; i < 6; i++)
//	{
//		// all faces of the cube have the same texture
//		CMesh0->BeginPolygon(-1, -1, -1, false);																				/*this is a bit tricky. when creating a polygon for the CMesh you want to use BeginPolygon so the program																				will understand that when you started creating the polygon. afterwards you should create a loop to add
//																															the desired vertices into the polygon you begun creating. after you have add all vertices, you should use
//																															EndPolygon so the program knows you have finished creating your polygon.*/
//
//		for (j = 0; j < 4; j++)
//		{
//			// Control CPoint index
//			CMesh0->AddPolygon(polygonVertices0[i * 4 + j]);
//
//
//		}
//
//		CMesh0->EndPolygon();
//	}
//
//	/*FbxNode* CMeshNode0 = FbxNode::Create(scene, "node0");		//creates a node to connect to the CMesh (for later use).
//	CMeshNode0->SetNodeAttribute(CMesh0);							//set the node atrribute to CMesh. this lets the node know that he is connected to a CMesh.
//	sceneRootNode->AddChild(CMeshNode0); */
//
//
//	return CMesh0;
//}








