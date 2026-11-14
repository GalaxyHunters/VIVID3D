#include "Analysis.h"

using namespace vivid;

vector<double> GetParticlesVolume(const CVoronoiVolume &arSurf) {
    return arSurf.GetVoronoiData().mData.GetAllVolumes();
} //CalcCellCMVolume

vector<coord_t> GetParticlesDensity(const CVoronoiVolume &arSurf, const std::vector<double> &arMassField) {
    vector<double> volumes = GetParticlesVolume(arSurf);

    vector<coord_t> densities (arMassField.size());
    for (int i = 0; i < arMassField.size(); i++) {
        densities[i] = arMassField[i]/volumes[i];
    }
    return densities;
}

vector<coord_t> GetSurfaceAreas(const CVoronoiVolume &arSurf)
{
    // Face Area Calculation by Triangles as in Mesh
    vector<coord_t> face_areas;
    const CSurfaceFaces &faces = arSurf.GetFaces();
    const vector<CPoint> &vertices = arSurf.GetVertices();
    for (size_t f = 0; f < faces.Size(); f++) {
        // calculating areas faces
        const uint32_t *verts = faces.Vertices(f);
        const size_t n_verts = faces.VertexCount(f);
        if (n_verts == 3) {
            CPoint ab_vec = (vertices[verts[0]] - vertices[verts[1]]).Normalize();
            CPoint ac_vec = (vertices[verts[0]] - vertices[verts[2]]).Normalize();
            face_areas.push_back(0.5 * ab_vec.Cross(ac_vec).Magnitude());
        }
        else {
            coord_t face_area = 0;
            for (size_t i = 1; i + 1 < n_verts; i++) {
            // Subdivide by diagonals from 0 point
                CPoint ab_vec = (vertices[verts[0]] - vertices[verts[i]]).Normalize();
                CPoint ac_vec = (vertices[verts[0]] - vertices[verts[i + 1]]).Normalize();
                face_area += 0.5 * ab_vec.Cross(ac_vec).Magnitude();
            }
            face_areas.push_back(face_area);
        }
    }

    return face_areas;
}


vector<coord_t> CalculateFlux(const CVoronoiVolume &arSurf, const CPoint &arField) {
    const CSurfaceFaces &faces = arSurf.GetFaces();
    vector<coord_t> flux (faces.Size());
    vector<coord_t> areas = GetSurfaceAreas(arSurf);
    const vector<CPoint> &input_points = arSurf.GetInputPoints();
    for (int i = 0; i<flux.size(); i++) {
        // Face Normal Calculate
        CPoint norm = input_points[faces.mPairs[i].second] - input_points[faces.mPairs[i].first];
        // Local Flux calculation : loop over the arField, summing the flux of the ones that cross the surface
        //flux[i] = arField[i].Magnitude() * arField[i].Dot(norm) * areas[i];
    }
    return flux;
}