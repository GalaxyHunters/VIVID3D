#include "Decimate.h"

using namespace std;

namespace vivid
{

    void CallBack(const char * msg) //TODO Y
    {
        cout << msg; //TODO should be log
    }

    void DecimateMesh(vector<CPoint>& aPoints, vector<CFace>& aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError) {
        //write the data to vec3 format
        vector< Vec3<float> > vertices;
        vector< Vec3<size_t> > triangles;
        vertices.reserve(aPoints.size());
        triangles.reserve(aFaces.size());
        for (const auto & aFace : aFaces) {
            triangles.emplace_back(aFace[0], aFace[1], aFace[2], aFace.GetUVcoord());
        }
        for (const auto & aPoint : aPoints) {
            vertices.emplace_back(aPoint.X(), aPoint.Y(), aPoint.Z(), 0);
        }
        // the mesh's own copy is not needed while decimating
        vector<CPoint>().swap(aPoints);
        vector<CFace>().swap(aFaces);

        //decimate model
        vector< Vec3<Float> > decimated_points;
        vector< Vec3<size_t> > decimated_triangles;
        {
            MeshDecimator myMDecimator;
            myMDecimator.SetCallBack(&CallBack);
            myMDecimator.Initialize(vertices.size(), triangles.size(), &vertices[0], &triangles[0]);
            myMDecimator.Decimate(aTargetVerticesN, aTargetTrianglesN, aMaxError);
            myMDecimator.GetMeshData(&decimated_points, &decimated_triangles);
        }   // the decimator (the biggest consumer of memory) is gone before the output is built
        vector< Vec3<float> >().swap(vertices);
        vector< Vec3<size_t> >().swap(triangles);

        //write the data to Mesh format
        aPoints.reserve(decimated_points.size());
        for (const auto & decimated_point : decimated_points) {
            aPoints.emplace_back(decimated_point.X(), decimated_point.Y(), decimated_point.Z());
        }
        aFaces.reserve(decimated_triangles.size());
        for (const auto & decimated_triangle : decimated_triangles) {
            aFaces.emplace_back(vector<size_t>{decimated_triangle[0], decimated_triangle[1], decimated_triangle[2]}, decimated_triangle.mQuan);
        }
    }

} // namespace vivid