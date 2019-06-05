#ifndef UTILS_H
#define UTILS_H

#include "Point.h"
#include "Voronoi3D.hpp"
#include "MeshDecimation/mdMeshDecimator.h"

using namespace std;
using namespace MeshDecimation;

inline vector<Vector3D> convertToVorn(vector<Point> inputPoints) {
	vector<Vector3D> newVec;
	for (vector<Point>::iterator it = inputPoints.begin(); it != inputPoints.end(); it++) {
		newVec.push_back(Vector3D(it->getX(), it->getY(), it->getZ()));
	}
	return newVec;
}

inline pair<vector<Vector3D>, vector<vector<size_t>>> compute_vornoi(vector<Point> inputPoints, double box_R) {
	vector<Vector3D> vornPoints = convertToVorn(inputPoints);

	/////////////////////////////////////debug////////////////////////////////////////////////////
	//cout << vornPoints.size() << endl;
	//ofstream o;
	//o.open("../../test_models/voroni_input.txt");
	//o << "box size input:\n";
	//o << to_string(-box_R) + " " + to_string(-box_R) + " " + to_string(-box_R) + "\n";
	//o << to_string(box_R) + " " + to_string(box_R) + " " + to_string(box_R) + "\n";
	//o << "points input:\n";
	//for (vector<Vector3D>::iterator it = vornPoints.begin(); it != vornPoints.end(); it++) {
	//	o << to_string(it->x) + " " + to_string(it->y) + " " + to_string(it->z) + "\n";
	//}
	//o.close();
	/////////////////////////////////////////debug over/////////////////////////////////////////////
	Voronoi3D temp(Vector3D(-box_R, -box_R, -box_R), Vector3D(box_R, box_R, box_R));
	temp.Build(vornPoints);
	vornPoints = temp.GetFacePoints();

	vector<vector<size_t>> faces;
	size_t totalCells = temp.GetAllCellFaces().size();
	face_vec cell;
	point_vec face_points;
	size_t Cpoint1, Cpoint2;
	for (size_t i = 0; i < totalCells; i++) {
		cell = temp.GetCellFaces(i);
		for (face_vec::iterator face = cell.begin(); face != cell.end(); face++) {
			Cpoint1 = get<0>(temp.GetFaceNeighbors(*face));
			Cpoint2 = get<1>(temp.GetFaceNeighbors(*face));
			if (!(Cpoint1 < i) && !(Cpoint2 < i)) { //the face doent belong to a cell we read already
				faces.push_back(vector<size_t>());
				face_points = temp.GetPointsInFace(*face);
				for (point_vec::iterator point = face_points.begin(); point != face_points.end(); point++){
					faces.back().push_back(*point);
				}
				faces.back().push_back(Cpoint1);
				faces.back().push_back(Cpoint2);
			}
		}
	}
	pair<vector<Vector3D>, vector<vector<size_t>>> output(vornPoints, faces);
	return output;
}

inline void CallBack(const char * msg)
{
	cout << msg;
}

inline pair<vector<Point>, vector<IndexedFace>> decimateMesh(vector<Point> points, vector<IndexedFace> faces, int targetVerticesN, int targetTrianglesN, float maxError) {
	//write the data to vec3 format
	vector< Vec3<float>> vertices;
	vector< Vec3<int>> triangles;
	for (vector<Point>::iterator pIt = points.begin(); pIt != points.end(); pIt++) {
		vertices.push_back(Vec3<float>(pIt->getX(), pIt->getY(), pIt->getZ()));
	}
	for (vector<IndexedFace>::iterator fIt = faces.begin(); fIt != faces.end(); fIt++) {
		triangles.push_back(Vec3<int>((*fIt)[0], (*fIt)[1], (*fIt)[2], fIt->getColor()));
	}
	//decimate model
	MeshDecimator myMDecimator;
	myMDecimator.SetCallBack(&CallBack);
	myMDecimator.Initialize(vertices.size(), triangles.size(), &vertices[0], &triangles[0]);
	myMDecimator.Decimate(targetVerticesN, targetTrianglesN, maxError);

	//get decimated data
	vector< Vec3<Float> > decimatedPoints;
	vector< Vec3<int> > decimatedtriangles;
	decimatedPoints.resize(myMDecimator.GetNVertices());
	decimatedtriangles.resize(myMDecimator.GetNTriangles());

	myMDecimator.GetMeshData(&decimatedPoints[0], &decimatedtriangles[0]);

	myMDecimator.~MeshDecimator();
	//write the data to Mesh format
	vector<Point> outPoints;
	vector<IndexedFace> outFaces;
	for (vector<Vec3<float>>::iterator pIt = decimatedPoints.begin(); pIt != decimatedPoints.end(); pIt++) {
		outPoints.push_back(Point(pIt->X(), pIt->Y(), pIt->Z()));
	}
	for (vector<Vec3<int>>::iterator fIt = decimatedtriangles.begin(); fIt != decimatedtriangles.end(); fIt++) {
		outFaces.push_back(IndexedFace((*fIt)[0], (*fIt)[1], (*fIt)[2], fIt->quan));
	}
	pair<vector<Point>, vector<IndexedFace>> output(outPoints, outFaces);
	return output;
}

#endif