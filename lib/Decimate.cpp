#include "Decimate.h"

using namespace std;

namespace vivid
{

    void CallBack(const char * msg) //TODO Y
    {
        cout << msg; //TODO should be log
    }

    pair<vector<CPoint>, vector<CFace> > DecimateMesh(vector<CPoint>& aPoints, vector<CFace> aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError) { //TODO: aFaces is not by reference
        //write the data to vec3 format
        vector< Vec3<float> > vertices;
        vector< Vec3<size_t> > triangles;
        Vec3<size_t> temp_triangle;
        for (auto & aFace : aFaces) {
            temp_triangle = Vec3<size_t>(aFace[0], aFace[1], aFace[2], aFace.GetUVcoord());
            triangles.push_back(temp_triangle); //, fIt->GetUVcoords()
        }
        for (auto & aPoint : aPoints) {
            vertices.push_back(Vec3<float>(aPoint.X(), aPoint.Y(), aPoint.Z(), 0));
        }
        //decimate model
        MeshDecimator myMDecimator;
        myMDecimator.SetCallBack(&CallBack);
        myMDecimator.Initialize(vertices.size(), triangles.size(), &vertices[0], &triangles[0]);
        myMDecimator.Decimate(aTargetVerticesN, aTargetTrianglesN, aMaxError);

        //get decimated data
        vector< Vec3<Float> > decimated_points;
        vector< Vec3<size_t> > decimated_triangles;
        decimated_points.clear();
        decimated_triangles.clear();

        myMDecimator.GetMeshData(&decimated_points, &decimated_triangles);

        //write the data to Mesh format
        vector<CPoint> out_points;
        out_points.clear();
        vector<CFace> out_faces;
        out_faces.clear();
        for (auto & decimated_point : decimated_points) {
            out_points.push_back(CPoint(decimated_point.X(), decimated_point.Y(), decimated_point.Z()));
        }
        for (auto & decimated_triangle : decimated_triangles) {
            out_faces.push_back(CFace({decimated_triangle[0], decimated_triangle[1], decimated_triangle[2]}, decimated_triangle.mQuan)); //, fIt->quan
        }
        pair<vector<CPoint>, vector<CFace> > output(out_points, out_faces);
        return output;
    }

} // namespace vivid