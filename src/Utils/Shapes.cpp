#include "Shapes.h"

#define _USE_MATH_DEFINES //TODO WTF???
//#include <math.h>

using namespace vivid;

using namespace std;




namespace vivid
{

//TODO make position a cpoint object
CMesh CreateBoxMesh(double sizeX, double sizeY, double sizeZ, coord_t color, coord_t alpha, vector<double> pos)
{
	vector<CPoint> CPoints(8);
	CPoints.clear();
	//TODO: Zohar MY OCD hurts here should start at  ---, --+, -+-, -++, +--, +-+ and I would say by for loop...
	//TODO  it is binary counting to 6 right? so as a PSAGON, you should know every issue that slightly related to it
	//TODO I promise to teach you binary kwakwa de la oma
	CPoints.push_back(CPoint(-1, +1, +1)); //0
	CPoints.push_back(CPoint(+1, +1, +1)); //1
	CPoints.push_back(CPoint(+1, +1, -1)); //2
	CPoints.push_back(CPoint(-1, +1, -1)); //3
	CPoints.push_back(CPoint(+1, -1, -1)); //4
	CPoints.push_back(CPoint(-1, -1, -1)); //5
	CPoints.push_back(CPoint(-1, -1, +1)); //6
	CPoints.push_back(CPoint(+1, -1, +1)); //7

	vector<CIndexedFace> faces(6);
	faces[0] = CIndexedFace(vector<size_t>{0, 3, 2, 1}, color);
	faces[1] = CIndexedFace(vector<size_t>{2, 3, 5, 4}, color);
	faces[2] = CIndexedFace(vector<size_t>{1, 2, 4, 7}, color);
	faces[3] = CIndexedFace(vector<size_t>{0, 1, 7, 6}, color);
	faces[4] = CIndexedFace(vector<size_t>{3, 0, 6, 5}, color);
	faces[5] = CIndexedFace(vector<size_t>{6, 7, 4, 5}, color);

    CMesh mesh;
    mesh.SetPoints(CPoints);
    mesh.SetFaces(faces);
    mesh.SetAlpha(alpha);
    mesh.SetLabel("cube");

    mesh.MoveMesh(CPoint(pos[0], pos[1], pos[2]));
	mesh.ScaleMesh(CPoint(0.5*sizeX, 0.5*sizeY, 0.5*sizeZ));
    return mesh;
}

CMesh CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, const vector<double> CenterPoint, coord_t Color, coord_t Alpha, const string Label)
{
    // TODO Zohar: VERYUGLY code, please use mesh.MoveMesh, mesh.ScaleMesh etc in this function
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
//    CMesh.SetLabel(Label); //TODO FIXME
	return CMesh;
}

// TODO: Zohar again VERY UGLY code, please use transformMat
CMesh CreateEllipsoidMesh(size_t NumOfMeridians, size_t NumOfParallels, vector<double> aRadius_vec, vector<double> CenterPoint, vector<double> MajorAxis, vector<double> MiddleAxis, vector<double> MinorAxis, coord_t Color, coord_t Alpha, string Label){
    //begin by asserting some conditions
    // TODO I'm not sure, I think we may start using exceptions, assert looks bad
//    assert(("Axis vectors cannot be (0,0,0)", MajorAxis != vector<double>{0,0,0} && MiddleAxis != vector<double>{0,0,0} && MinorAxis != vector<double>{0,0,0}));
//    assert(("Radius cannot be equal to 0", aRadius_vec[0] != 0 && aRadius_vec[1] != 0 && aRadius_vec[2] != 0));
//    assert(("Bro did you just input vectors that arent perpendicular? bro ngl thats kinda cringe", CheckIfPerpindicular(MajorAxis, MiddleAxis) == true && CheckIfPerpindicular(MajorAxis, MinorAxis) == true && CheckIfPerpindicular(MiddleAxis, MinorAxis) == true));
    //create a sphere to operate on

//    CMesh Ellipsoid = CreateSphereMesh(NumOfMeridians, NumOfParallels, 1.0, vector<double>{0.0,0.0,0.0}, Color, Alpha, Label); //TODO FIXME later
    CMesh Ellipsoid;
    //use aRadius_vec to create strech matrix and stretch the sphere
    vector<vector<double>> StretchMatrix;
    StretchMatrix.push_back(vector<double>{aRadius_vec[0], 0, 0});
    StretchMatrix.push_back(vector<double>{0, aRadius_vec[1], 0});
    StretchMatrix.push_back(vector<double>{0, 0, aRadius_vec[2]});
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
        CurrentVector = CPoint(CenterPoint[0] + CVX * RotateMatrix[0][0] + CVY * RotateMatrix[1][0] + CVZ * RotateMatrix[2][0],
                               CenterPoint[1] + CVX * RotateMatrix[0][1] + CVY * RotateMatrix[1][1] + CVZ * RotateMatrix[2][1],
                               CenterPoint[2] + CVX * RotateMatrix[0][2] + CVY * RotateMatrix[1][2] + CVZ * RotateMatrix[2][2]);
    }

    Ellipsoid.SetPoints(points);
    return Ellipsoid;
    //set new info to the mesh

}

// TODO ask Zohar what kind of arrow is it
// TODO, THINK about Jill Neiman's arrow and the scale thingy
CMesh CreateArrowMesh(double Width, double PCRatio, vector<double> aPos, vector<double> DirVec, double Color, double Alpha, string Label){
    //before we run, lets assert some conditions //TODO same comment on Exceptions
    assert(("Direction Vector cannot be (0,0,0)", DirVec != vector<double>{0,0,0}));
    assert(("Width and PCRatio must be different then 0", Width != 0 && PCRatio != 0));
    assert(("Width and PCRatio must be different then 0", Width != 0 && PCRatio != 0));
    // Create arrow pointing from (0,0,0) to (0,0,1)
    vector<CPoint> points;
    vector<CIndexedFace> faces;
    for (int j =-1; j<2; j++) { //start by creating an arrow that points up the z axis
        if (j == 0) continue;
        for (int i=-1; i<2; i++) { //creating the points for the bottom square
            if (0==i) continue;
            points.push_back(CPoint(0.4 * j * Width, 0.4 * i * Width, 0));
        }
        for (int k=-1; k<2; k++) { //creating the points for the top square
            if (0==k) continue;
            points.push_back(CPoint(0.4 * j * Width, 0.4 * k * Width, (1-PCRatio)));
        }
    }
    for (int l=-1; l<2; l++) { //creating the points for the pointer
        if (0==l) continue;
        for (int i = -1; i<2; i++) {
            if (0==i) continue;
            points.push_back(CPoint(0.8 * l * Width, 0.8 * i * Width, (1-PCRatio) ));
        }
    }
    points.push_back(CPoint(0, 0, 1)); //the pointer point

    //creating faces
    //creating the chest faces
    faces.push_back(CIndexedFace(vector<size_t>{0, 4, 5, 1}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{2, 6, 7, 3}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{0, 2, 6, 4}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{4, 6, 7, 5}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{5, 7, 3, 1}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{1, 3, 2, 0}, Color));
    //creating the pointer faces
    faces.push_back(CIndexedFace(vector<size_t>{8,  10, 11, 9}, Color));
    faces.push_back(CIndexedFace(vector<size_t>{8,  12, 10},    Color));
    faces.push_back(CIndexedFace(vector<size_t>{10, 12, 11},    Color));
    faces.push_back(CIndexedFace(vector<size_t>{11, 12, 9},     Color));
    faces.push_back(CIndexedFace(vector<size_t>{9,  12, 8},     Color));

    CMesh mesh;
    mesh.SetPoints(points);
    mesh.SetFaces(faces);
    mesh.SetAlpha(Alpha);
    mesh.SetLabel(Label);

    // Scale the arrow by DirVec and later rotating the arrow to DirVec direction
    CPoint direction_vec  = CPoint(DirVec[0],DirVec[1],DirVec[2]);
    auto direction_size = direction_vec.Norm();
    CPoint cross_vec = CPoint(0,0,1).Cross(direction_vec);
    CPoint normal_vec = cross_vec.Normalize();
    double rotation_angel = acos( CPoint(0,0,1).Dot(normal_vec)); //Note both vec are normalized so it's ok

//    cout << "normal_vec: " << normal_vec << "\t";
//    cout << "rotation_angel: " << rotation_angel << "\n";
//    cout << "direction_size: " << direction_size << "\n";
//    cout << "cross_product: " << cross_vec << "\n";
//    cout << "VectorSize(cross_vec): " << Norm(cross_vec) << "\n";

    if ( cross_vec.Norm() < 0.0001 ) // meaning direction_vec is on the z axis
    {
        if (direction_vec.Z() < 0)
        {
            direction_size = -1*direction_size;
        }
        mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
    }
    else{
        mesh.ScaleMesh(CPoint(direction_size, direction_size, direction_size));
        mesh.RotateMesh(normal_vec, rotation_angel);
    }

    mesh.MoveMesh(CPoint(aPos[0], aPos[1], aPos[2]));
    return mesh;

}

} // namespace vivid


////this function applies a matrix so that the input points is rotated in a way that vector1 is equal to vector2.
//vector<CPoint> RotateMatchVectors(vector<CPoint> Points, vector<double> &Vector1, vector<double> &Vector2){
//    //start by using the cross product to get a vector thats gonna be our rotation base, ie we are going to rotate around it.



