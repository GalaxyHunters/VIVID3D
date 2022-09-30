#include <numeric>
#include "Mesh.h"
#include "boost/algorithm/string/predicate.hpp"
using namespace vivid;
using namespace boost::algorithm;

/*-------------------------------------------------- Public Methods --------------------------------------------------*/

void CMesh::Reduce(coord_t aVerticlePercent, coord_t aMaxError)
{
    //check input valdilty
    if( aVerticlePercent < 0 || aVerticlePercent > 1){
        aVerticlePercent = max(0.05, min(aVerticlePercent, .9));
        Log(LOG_WARNING, INVALID_ALPHA_VALUE);
    }
    if( aMaxError < 0 || aMaxError > 1){
        aMaxError = max(0.05, min(aMaxError, .9));
        Log(LOG_WARNING, INVALID_ALPHA_VALUE);
    }
    if (!mFacesAreTriangles) { TriangulizeFaces(); }
    //call decimation from External
    int targetVerticesN = int(aVerticlePercent * mPoints.size());
    int targetTrianglesN = int(aVerticlePercent * mFaces.size());
    auto temp = DecimateMesh(mPoints, mFaces, targetVerticesN, targetTrianglesN, aMaxError);
    mPoints = get<0>(temp);
    mFaces = get<1>(temp);
}

// TODO: Fix similar methods, fix RemovePointyFaces

void CMesh::SubdivideLargeFaces(coord_t aAboveAverageThreshold)
{
    if(!mFacesAreTriangles) { TriangulizeFaces(); }

    vector<coord_t> face_areas;
    vector<CFace> new_faces;
    for (auto & mFace : mFaces) {
        // calculating areas faces
        CPoint ab_vec = (mPoints[mFace[0]]-mPoints[mFace[1]]).Normalize();
        CPoint ac_vec = (mPoints[mFace[0]]-mPoints[mFace[2]]).Normalize();

        face_areas.push_back(0.5 * ab_vec.Cross(ac_vec).Magnitude());
    }
    coord_t average = accumulate(face_areas.begin(), face_areas.end(), 0.0) / face_areas.size();

    for (int i = 0; i < mFaces.size(); i++) {
        if (face_areas[i] <= average * aAboveAverageThreshold) {
            new_faces.push_back(mFaces[i]);
        } else {
            // Subdivide
            vector<size_t> vertices = mFaces[i].GetPoints();
            vector<CPoint> new_vertices (3);
            size_t last_size = mPoints.size();
            // 3 New points
            mPoints.push_back((mPoints[vertices[0]] + mPoints[vertices[1]])/2);
            mPoints.push_back((mPoints[vertices[1]] + mPoints[vertices[2]])/2);
            mPoints.push_back((mPoints[vertices[2]] + mPoints[vertices[0]])/2);

            // 4 New faces
            new_faces.push_back(CFace({vertices[0], last_size, last_size+2}, mFaces[i].GetUVcoord()));
            new_faces.push_back(CFace({vertices[1], last_size, last_size+1}, mFaces[i].GetUVcoord()));
            new_faces.push_back(CFace({vertices[2], last_size+1, last_size+2}, mFaces[i].GetUVcoord()));
            new_faces.push_back(CFace({last_size, last_size+1, last_size+2}, mFaces[i].GetUVcoord()));
        }
    }
    mFaces = new_faces;
    CalculatePointsNeighbours();
}

void CMesh::RemoveLargeFaces(coord_t aAboveAverageThreshold)
{
    if(!mFacesAreTriangles) { TriangulizeFaces(); }

    vector<coord_t> face_areas;
    vector<CFace> new_faces;
    for (auto it = mFaces.begin(); it != mFaces.end(); it++) {
        // calculating areas faces
        CPoint ab_vec = (mPoints[it->GetPoints()[0]]-mPoints[it->GetPoints()[1]]).Normalize();
        CPoint ac_vec = (mPoints[it->GetPoints()[0]]-mPoints[it->GetPoints()[2]]).Normalize();

        face_areas.push_back(0.5 * ab_vec.Cross(ac_vec).Magnitude());
    }
    coord_t average = accumulate(face_areas.begin(), face_areas.end(), 0.0) / face_areas.size();

    for (int i = 0; i < mFaces.size(); i++) {
        if (face_areas[i] <= average * aAboveAverageThreshold) {
            new_faces.push_back(mFaces[i]);
        }
    }
    mFaces = new_faces;
    CalculatePointsNeighbours();
}

void CMesh::RemovePointyFaces(coord_t aThetaThreshold)
{
    if(!mFacesAreTriangles) { TriangulizeFaces(); }

    vector<CPoint> next_points = mPoints;

    for (int i = 0; i < 40; i++) {

        for (auto it = mFaces.begin(); it != mFaces.end(); it++) {
            // Filtering pointy faces
            for (int k = 0; k < it->GetPoints().size(); k++){
                int prev = k-1;
                int next = k+1;
                if (k == 0) {
                    prev = 2;
                } else if (k == 2) {
                    next = 0;
                }

                CPoint ab_vec = (mPoints[it->GetPoints()[prev]]-mPoints[it->GetPoints()[k]]).Normalize();
                CPoint bc_vec = (mPoints[it->GetPoints()[next]]-mPoints[it->GetPoints()[k]]).Normalize();
                coord_t theta = acos(ab_vec.Dot(bc_vec)) * (180/M_PI);
                if (theta <= aThetaThreshold) {
                    // Laplacian Smooth
                    next_points[it->GetPoints()[k]] = CPoint(0,0,0);
                    for (auto jt = mPointNeighbours[it->GetPoints()[k]].begin(); jt != mPointNeighbours[it->GetPoints()[k]].end(); jt++) {
                        next_points[it->GetPoints()[k]] += mPoints[*jt];
                    }
                    next_points[it->GetPoints()[k]] = next_points[it->GetPoints()[k]] / (mPointNeighbours[it->GetPoints()[k]].size());
                    break;
                }
            }
        }
        mPoints = next_points;
    }
}

void CMesh::LaplacianSmooth(size_t aNumIterations, double aAlphaFactor, double aBetaFactor)
{
    vector<CPoint> curr_points = mPoints;

    for (int i = 0; i < aNumIterations; i++) {
        vector<CPoint> next_points (mPoints.size());
        vector<CPoint> curr_beta_points (mPoints.size());
        // Laplacian Smooth
        for (size_t j = 0; j < mPoints.size(); j++) {
            if (!mPointNeighbours[j].empty()) {
                for (auto it = mPointNeighbours[j].begin(); it != mPointNeighbours[j].end(); it++) {
                    next_points[j] += curr_points[*it];
                }
                next_points[j] = next_points[j] / (mPointNeighbours[j].size());

                curr_beta_points[j] = next_points[j] - (mPoints[j] * aAlphaFactor + curr_points[j] * (1. - aAlphaFactor));
            } else {
                next_points[j] = mPoints [j];
            }
        }
        // HC Algorithm
        if (aBetaFactor > 0) {
            for (size_t j = 0; j < mPoints.size(); j++) {
                if (!mPointNeighbours[j].empty()) {
                    CPoint next_beta;
                    for (auto it = mPointNeighbours[j].begin(); it != mPointNeighbours[j].end(); it++) {
                        next_beta += curr_beta_points[*it];
                    }
                    next_points[j] -= curr_beta_points[j] * aBetaFactor + next_beta*((1. - aBetaFactor) / mPointNeighbours[j].size());
                }
            }
        }

        curr_points = next_points;
    }
    mPoints = curr_points;
}

void CMesh::CalculatePointsNeighbours() {
    for (auto & mFace : mFaces) {

        size_t n_points = mFace.GetPoints().size();
        for (int j = 0; j < n_points; j++) {
            // Face contains point i, so get one before and one after,
            size_t prev = mFace[j-1];
            size_t next = mFace[j+1];
            if (j == 0) {
                prev = mFace[n_points-1];
            } else if (j == (n_points-1)) {
                next = mFace[0];
            }
            mPointNeighbours[mFace[j]].insert(prev);
            mPointNeighbours[mFace[j]].insert(next);
        }
    }
}

/*------------------------------------------------- Private Methods --------------------------------------------------*/

void CMesh::TriangulizeFaces() {
    vector<CFace> triangle_faces;
    for (auto &mFace : mFaces) {
        size_t prev_0=0, prev_1=1, prev_2=2;
        for (size_t i = 1; i < mFace.GetPoints().size()-1; i++) {
            // Add faces along alternating diagonals
//            if (i%2 == 1){
//                triangle_faces.push_back(CFace({mFace[prev_0], mFace[prev_1], mFace[prev_2]}, mFace.GetUVcoord()));
//            } else {
//                prev_1 = mFace.GetPoints().size()-prev_1;
//                triangle_faces.push_back(CFace({mFace[prev_0], mFace[prev_1], mFace[prev_2]}, mFace.GetUVcoord()));
//                if (prev_0 == 0) { prev_0 = 3;}
//                else { prev_0++;}
//                prev_1+=prev_2;
//                prev_2=prev_1-prev_2;
//                prev_1-=prev_2;
//            }
            triangle_faces.push_back(CFace({mFace[0], mFace[i], mFace[i + 1]}, mFace.GetUVcoord()));
        }
    }
    mFaces = triangle_faces;
    mFacesAreTriangles = true;
}