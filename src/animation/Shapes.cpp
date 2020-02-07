#include "Shapes.h"
#include "Animation.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;


Shapes::~Shapes() {}
Shapes::Shapes() {}

const size_t a1 = NULL;
const size_t b1 = NULL;

CMesh Shapes::CreateCubeMesh(double sizeX, double sizeY, double sizeZ, cord_t color, cord_t alpha, vector<double> position)
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

	faces[0] = CIndexedFace(vector<size_t>{0, 3, 2, 1}, 1);
	faces[1] = CIndexedFace(vector<size_t>{2, 3, 5, 4}, 1);
	faces[2] = CIndexedFace(vector<size_t>{1, 2, 4, 7}, 1);
	faces[3] = CIndexedFace(vector<size_t>{0, 1, 7, 6}, 1);
	faces[4] = CIndexedFace(vector<size_t>{3, 0, 6, 5}, 1);
	faces[5] = CIndexedFace(vector<size_t>{6, 7, 4, 5}, 1);

    for (int l = 0; l < 6; ++l)
    {
        faces[l].SetColor(color);
    }

    CMesh CMesh;
    CMesh.SetPoints(CPoints);
    CMesh.SetFaces(faces);
    CMesh.SetAlpha(1);
    CMesh.SetLabel("cube");

	return CMesh;
}

CMesh Shapes::CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, cord_t Color, cord_t Alpha, vector<double> CenterPoint)
{
    vector<CPoint> vertices;
    vector<CIndexedFace> faces;
    double Theta;
    double Phi;

    vertices.push_back(CPoint(CenterPoint[0], CenterPoint[1], CenterPoint[2] + radius)); //Creating the top polar
    vertices.push_back(CPoint(CenterPoint[0], CenterPoint[1], CenterPoint[2] - radius)); //Creating the bottom polar

    double PhiStep = M_PI / (num_of_parallels + 1);
    double ThetaStep = (2 * M_PI) / num_of_meridians;

    for (int j = 0; j < num_of_meridians; ++j)
    {
        Theta = j * ThetaStep;
        for (int k = 1; k < num_of_parallels + 1; ++k)
        {
            Phi = (M_PI / 2) - (k * PhiStep);
            vertices.push_back(CPoint(CenterPoint[0] + radius * cos(Phi) * cos(Theta), CenterPoint[1] + radius * cos(Phi) * sin(Theta), CenterPoint[2] + radius * sin(Phi)));    //creating the vertices
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
    CMesh.SetLabel("sphere");
	return CMesh;
}


CMesh Shapes::CreateArrowMesh(double x, double y, double z, double chest_length, double pointer_length, double width, cord_t color)
{
    vector<CPoint> points;
    points.clear();
    points.push_back(CPoint(x, y, z));
    points.push_back(CPoint(x + (1.5 * width), y, z - (1.5 * pointer_length)));
    points.push_back(CPoint(x, y + (1.5 * width), z - (1.5 * pointer_length)));
    points.push_back(CPoint(x - (1.5 * width), y, z - (1.5 * pointer_length)));
    points.push_back(CPoint(x, y  - (1.5 * width), z - (1.5 * pointer_length)));

    points.push_back(CPoint(x - width, y, z - pointer_length));
    points.push_back(CPoint(x + width, y, z - pointer_length));
    points.push_back(CPoint(x, y - width, z - pointer_length));
    points.push_back(CPoint(x, y  + width, z - pointer_length));

    points.push_back(CPoint(x - width, y, z - pointer_length - chest_length));
    points.push_back(CPoint(x + width, y, z - pointer_length - chest_length));
    points.push_back(CPoint(x, y - width, z - pointer_length - chest_length));
    points.push_back(CPoint(x, y + width, z - pointer_length - chest_length));

    vector<CIndexedFace> faces(10);
//    faces.clear();
    faces[0] = CIndexedFace(vector<size_t>{1, 2, 3, 4}, color);
    faces[1] = CIndexedFace(vector<size_t>{0, 1, 2}, color);
    faces[2] = CIndexedFace(vector<size_t>{0, 2, 3}, color);
    faces[3] = CIndexedFace(vector<size_t>{0, 3, 4}, color);
    faces[4] = CIndexedFace(vector<size_t>{0, 4, 1}, color);

    faces[5] = CIndexedFace(vector<size_t>{5, 9, 12, 8}, color);
    faces[6] = CIndexedFace(vector<size_t>{8, 12, 10, 6}, color);
    faces[7] = CIndexedFace(vector<size_t>{6, 10, 11, 7}, color);
    faces[8] = CIndexedFace(vector<size_t>{7, 11, 9, 5}, color);

    CMesh arrow;
    arrow.SetPoints(points);
    arrow.SetFaces(faces);
    arrow.SetAlpha(1);
    arrow.SetLabel("arrow");

    return arrow;
}

FbxMesh* Shapes::createCube(int sizeX, int sizeY, int sizeZ)
{
	FbxManager* manager = FbxManager::Create();
	//FbxScene* scene = FbxScene::Create(manager, "");
	//FbxNode* sceneRootNode = scene->GetRootNode();
	FbxMesh* CMesh0 = FbxMesh::Create(manager, "CMesh0");			//creates the CMesh
	//creating vertices for the CMesh
	FbxVector4 vertex0(-sizeX, -sizeY, sizeZ);
	FbxVector4 vertex1(sizeX, -sizeY, sizeZ);
	FbxVector4 vertex2(sizeX, sizeY, sizeZ);
	FbxVector4 vertex3(-sizeX, sizeY, sizeZ);
	FbxVector4 vertex4(-sizeX, -sizeY, -sizeZ);
	FbxVector4 vertex5(sizeX, -sizeY, -sizeZ);
	FbxVector4 vertex6(sizeX, sizeY, -sizeZ);
	FbxVector4 vertex7(-sizeX, sizeY, -sizeZ);

	//CMeshNode0->ConnectSrcObject(CMesh0);
	CMesh0->InitControlPoints(24);								//Initialize the control CPoint array of the CMesh.
	FbxVector4* CMesh0ControlCPoints = CMesh0->GetControlPoints();		//

	// Define each face of the cube.
	// Face 1
	CMesh0ControlCPoints[0] = vertex0;
	CMesh0ControlCPoints[1] = vertex1;
	CMesh0ControlCPoints[2] = vertex2;
	CMesh0ControlCPoints[3] = vertex3;
	// Face 2
	CMesh0ControlCPoints[4] = vertex1;
	CMesh0ControlCPoints[5] = vertex5;
	CMesh0ControlCPoints[6] = vertex6;
	CMesh0ControlCPoints[7] = vertex2;
	// Face 3
	CMesh0ControlCPoints[8] = vertex5;
	CMesh0ControlCPoints[9] = vertex4;
	CMesh0ControlCPoints[10] = vertex7;
	CMesh0ControlCPoints[11] = vertex6;
	// Face 4
	CMesh0ControlCPoints[12] = vertex4;
	CMesh0ControlCPoints[13] = vertex0;
	CMesh0ControlCPoints[14] = vertex3;
	CMesh0ControlCPoints[15] = vertex7;
	// Face 5
	CMesh0ControlCPoints[16] = vertex3;
	CMesh0ControlCPoints[17] = vertex2;
	CMesh0ControlCPoints[18] = vertex6;
	CMesh0ControlCPoints[19] = vertex7;
	// Face 6
	CMesh0ControlCPoints[20] = vertex1;
	CMesh0ControlCPoints[21] = vertex0;
	CMesh0ControlCPoints[22] = vertex4;
	CMesh0ControlCPoints[23] = vertex5;

	int i, j;
	int polygonVertices0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	for (i = 0; i < 6; i++)
	{
		// all faces of the cube have the same texture
		CMesh0->BeginPolygon(-1, -1, -1, false);																				/*this is a bit tricky. when creating a polygon for the CMesh you want to use BeginPolygon so the program																				will understand that when you started creating the polygon. afterwards you should create a loop to add
																															the desired vertices into the polygon you begun creating. after you have add all vertices, you should use
																															EndPolygon so the program knows you have finished creating your polygon.*/

		for (j = 0; j < 4; j++)
		{
			// Control CPoint index
			CMesh0->AddPolygon(polygonVertices0[i * 4 + j]);

		}

		CMesh0->EndPolygon();
	}

	/*FbxNode* CMeshNode0 = FbxNode::Create(scene, "node0");		//creates a node to connect to the CMesh (for later use).
	CMeshNode0->SetNodeAttribute(CMesh0);							//set the node atrribute to CMesh. this lets the node know that he is connected to a CMesh.
	sceneRootNode->AddChild(CMeshNode0); */


	return CMesh0;
}








