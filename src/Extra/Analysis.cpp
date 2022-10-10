#include "Analysis.h"

using namespace vivid;

vector<double> GetParticlesVolume(const CVoronoiVolume &arSurf) {
    return arSurf.GetVoronoiData().mData.GetAllVolumes();
} //CalcCellCMVolume

vector<double> GetParticlesDensity(const CVoronoiVolume &arSurf, const std::vector<double> &arMassField) {
    vector<double> volumes = GetParticlesVolume(arSurf);

    vector<double> densities (arMassField.size());
    for (int i = 0; i < arMassField.size(); i++) {
        densities[i] = arMassField[i]/volumes[i];
    }
    return densities;
}

vector<double> GetSurfaceAreas(const CVoronoiVolume &arSurf)
{
    // Face Area Calculation by Triangles as in Mesh
    vector<coord_t> face_areas;
    for (auto & mFace : arSurf.GetFaces()) {
        // calculating areas faces
        vector<shared_ptr<CPoint>> verts = mFace.mVertices;
        if (verts.size() == 3) {
            CPoint ab_vec = (*verts[0]-*verts[1]).Normalize();
            CPoint ac_vec = (*verts[0]-*verts[2]).Normalize();
            face_areas.push_back(0.5 * ab_vec.Cross(ac_vec).Magnitude());
        }
        else {
            coord_t face_area = 0;
            for (size_t i = 1; i < verts.size()-1; i++) {
            // Subdivide by diagonals from 0 point
                CPoint ab_vec = (*verts[0]-*verts[i]).Normalize();
                CPoint ac_vec = (*verts[0]-*verts[i+1]).Normalize();
                face_area += 0.5 * ab_vec.Cross(ac_vec).Magnitude();
            }
            face_areas.push_back(face_area);
        }
    }

    return face_areas;
}


vector<double> CalculateFlux(const CVoronoiVolume &arSurf, const CPoint &arField) {
    vector<CSurfaceFace> faces = arSurf.GetFaces();
    vector<double> flux (faces.size());
    vector<double> areas = GetSurfaceAreas(arSurf);
    vector<CPoint> input_points = arSurf.GetInputPoints();
    for (int i = 0; i<flux.size(); i++) {
        // Face Normal Calculate
        CPoint norm = input_points[faces[i].mPairPoints.second] - input_points[faces[i].mPairPoints.first];
        // Local Flux calculation : loop over the arField, summing the flux of the ones that cross the surface
        //flux[i] = arField[i].Magnitude() * arField[i].Dot(norm) * areas[i];
    }
    return flux;
}