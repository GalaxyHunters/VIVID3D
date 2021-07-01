#include "External.h"

using namespace std;

namespace vivid
{

void CallBack(const char * msg) //TODO Y
{
	cout << msg;
}

pair<vector<CPoint>, vector<CIndexedFace> > DecimateMesh(vector<CPoint>& aPoints, vector<CIndexedFace> aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError) { //TODO: aFaces is not by reference
	//write the data to vec3 format
	vector< Vec3<float> > vertices;
	vector< Vec3<int> > triangles;
	Vec3<int> temp_triangle;
	for (vector<CIndexedFace>::iterator fIt = aFaces.begin(); fIt != aFaces.end(); fIt++) {
		temp_triangle = Vec3<int>((*fIt)[0], (*fIt)[1], (*fIt)[2], fIt->GetColor());
		triangles.push_back(temp_triangle); //, fIt->GetColor()
	}
	for (vector<CPoint>::iterator pIt = aPoints.begin(); pIt != aPoints.end(); pIt++) {
		vertices.push_back(Vec3<float>(pIt->X(), pIt->Y(), pIt->Z(), 0));
	}
	//decimate model
	MeshDecimator myMDecimator;
	myMDecimator.SetCallBack(&CallBack);
	myMDecimator.Initialize(vertices.size(), triangles.size(), &vertices[0], &triangles[0]);
	myMDecimator.Decimate(aTargetVerticesN, aTargetTrianglesN, aMaxError);

	//get decimated data
	vector< Vec3<Float> > decimated_points;
	vector< Vec3<int> > decimated_triangles;
	decimated_points.clear();
	decimated_triangles.clear();

	myMDecimator.GetMeshData(&decimated_points, &decimated_triangles);

	//write the data to Mesh format
	vector<CPoint> out_points;
	out_points.clear();
	vector<CIndexedFace> out_faces;
	//out_faces.clear();
	for (auto pIt = decimated_points.begin(); pIt != decimated_points.end(); pIt++) {
		out_points.push_back(CPoint(pIt->X(), pIt->Y(), pIt->Z()));
	}
	for (auto fIt = decimated_triangles.begin(); fIt != decimated_triangles.end(); fIt++) {
		out_faces.push_back(CIndexedFace((*fIt)[0], (*fIt)[1], (*fIt)[2], fIt->mQuan)); //, fIt->quan
	}
	pair<vector<CPoint>, vector<CIndexedFace> > output(out_points, out_faces);
	return output;
}

} // namespace vivid