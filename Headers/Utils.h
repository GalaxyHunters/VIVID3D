#pragma once

#ifndef UTILS_H
#define UTILS_H

#include "Voronoi3D.hpp"
using namespace std;

static vector<Vector3D> convertToVorn(vector<Point> inputPoints) {
	vector<Vector3D> newVec;
	for (vector<Point>::iterator it = inputPoints.begin(); it != inputPoints.end(); it++) {
		newVec.push_back(Vector3D(it->getX(), it->getY(), it->getZ()));
	}
	return newVec;
}

static pair<vector<Vector3D>, vector<vector<size_t>>> compute_vornoi(vector<Point> inputPoints, double box_R) {
	vector<Vector3D> vornPoints = convertToVorn(inputPoints);
	Voronoi3D temp(Vector3D(box_R, box_R, box_R), Vector3D(-box_R, -box_R, -box_R));
	temp.Build(vornPoints);
	vornPoints = temp.GetFacePoints();

	vector<vector<size_t>> faces;
	vector<size_t> facePoints;
	size_t totalcells = temp.GetAllCellFaces().size();
	vector<size_t> cell;
	for (size_t i = 0; i < totalcells; i++) {
		cell = temp.GetCellFaces(i);
		for (vector<size_t>::iterator face = cell.begin(); face != cell.end(); face++) {
			size_t cpoint1 = get<0>(temp.GetFaceNeighbors(*face));
			size_t cpoint2 = get<1>(temp.GetFaceNeighbors(*face));
			if (!(cpoint1 < i) && !(cpoint2 < i)) { //the face doent belong to a cell we read already
				facePoints = temp.GetPointsInFace(*face);
				facePoints.push_back(cpoint1);
				facePoints.push_back(cpoint2);
				faces.push_back(facePoints);
			}
		}
	}
	pair<vector<Vector3D>, vector<vector<size_t>>> output(vornPoints, faces);
	return output;
}

#endif