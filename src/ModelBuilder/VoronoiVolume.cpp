#include "VoronoiVolume.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "Noisify.h"

using namespace vivid;
using namespace std;

constexpr coord_t BOX_EXPAND_FACTOR = 1;
constexpr coord_t PARTICLE_SCALE_MAGNITUDE = 1000;
constexpr coord_t NOISE_PERCENTAGE = 0.0001;

CVoronoiVolume::CVoronoiVolume(const vector<CPoint> &arInputPoints, vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, coord_t aNoiseDisplacement)
{
    // Check input validity
    if (arInputPoints.empty()) {
        Log(LOG_ERROR, ARRAYS_EMPTY);
    }
    if (!arColorField.empty() && arInputPoints.size() != arColorField.size()) {
        Log(LOG_ERROR, ARRAYS_NOT_EQUAL);
    }
    vector<pair<CSurfacePoint, size_t>> points;
    vector<normal_float> normal_field = NormalizeField(arColorField, arInputPoints.size(), aVMin, aVMax);
    for (int i = 0; i < arInputPoints.size(); i++) {
        points.emplace_back(CSurfacePoint(CPoint(arInputPoints[i]), normal_field[i], false), i);
    }
    mNumCallerPoints = arInputPoints.size();
    mInputPoints = arInputPoints;
    PreProcessPoints(points, aNoiseDisplacement);
    CreateSurface();
}

// Member-wise copy (the Voronoi tessellation object itself is not copied, as before).
CVoronoiVolume::CVoronoiVolume(const CVoronoiVolume &surf) = default;

/* ------------------------------------------------ Public Methods ---------------------------------------------------- */

CMesh CVoronoiVolume::MaskMesh(const vector<bool> &arMask, const string& arLabel, normal_float aOpacity) const
{
    Log(LOG_DEBUG, "Mask Input Size: " + to_string(arMask.size()));
    if( (find(arMask.begin(),arMask.end(),true) == arMask.end()) || find(arMask.begin(), arMask.end(), false) == arMask.end() ){
        Log(LOG_ERROR, MISSING_BOOLEAN_VALUES);
    }
    if (arMask.size() != mNumCallerPoints) {
        Log(LOG_ERROR, ARRAYS_NOT_EQUAL);
    }
    // the mask of the points the Voronoi was computed on (duplicate input points were merged)
    vector<char> point_mask(mInputOutputMap.size());
    for (size_t i = 0; i < mInputOutputMap.size(); i++) {
        point_mask[i] = arMask[mInputOutputMap[i]];
    }

    // 1. Keep the faces that separate a true point from a false point, split into triangles (fan from the first
    //    corner) and oriented from true to false. Only these faces are ever touched: everything else is skipped.
    vector<uint32_t> tris;               // 3 vertex ids per triangle
    vector<coord_t> tri_uv;              // colour of each triangle
    vector<char> used(mVertices.size(), 0);
    for (size_t f = 0; f < mSurfFaces.Size(); f++) {
        const pair<uint32_t, uint32_t> pr = mSurfFaces.mPairs[f];
        if (point_mask[pr.first] == point_mask[pr.second]) {
            continue;
        }
        const uint32_t* v = mSurfFaces.Vertices(f);
        const size_t nv = mSurfFaces.VertexCount(f);
        // direction of the normal: from the true point to the false point
        const CPoint &p1 = mInputPoints[pr.first], &p2 = mInputPoints[pr.second];
        CPoint true_normal = point_mask[pr.first] ? (p2 - p1).Normalize() : (p1 - p2).Normalize();
        for (size_t i = 1; i + 1 < nv; i++) {
            uint32_t a = v[0], b = v[i], c = v[i + 1];
            CPoint face_normal = ((mVertices[b] - mVertices[a]).Cross(mVertices[c] - mVertices[a])).Normalize();
            CPoint oriented = true_normal;
            oriented *= face_normal;
            if (oriented.Dot({1, 1, 1}) < 0) {
                std::swap(b, c);
            }
            tris.push_back(a); tris.push_back(b); tris.push_back(c);
            tri_uv.push_back(mSurfFaces.mUVcoords[f]);
            used[a] = used[b] = used[c] = 1;
        }
    }

    // 2. The corners the surviving triangles use, in their original order; sort them (the same comparison and the same
    //    sequence as ever, so the same order) and merge corners that are on the same place.
    vector<uint32_t> order;
    for (uint32_t i = 0; i < used.size(); i++) {
        if (used[i]) order.push_back(i);
    }
    sort(order.begin(), order.end(), [this](uint32_t a, uint32_t b) { return mVertices[a] < mVertices[b]; });
    vector<uint32_t> new_id(mVertices.size(), 0);    // old vertex id -> index in the mesh
    vector<CPoint> points;
    points.reserve(order.size());
    for (size_t i = 0; i < order.size(); i++) {
        const CPoint &base = mVertices[order[i]];
        new_id[order[i]] = static_cast<uint32_t>(points.size());
        points.push_back((base * mScale) + mCenVector);
        size_t j = i + 1;
        while (j < order.size() && base.Dist(mVertices[order[j]]) <= POINT_SIMILARITY_THRESHOLD) {
            new_id[order[j]] = static_cast<uint32_t>(points.size() - 1);
            j++;
        }
        i = j - 1;
    }

    // 3. The faces of the mesh: corners that were merged into one appear once.
    vector<CFace> faces;
    faces.reserve(tri_uv.size());
    for (size_t t = 0; t < tri_uv.size(); t++) {
        vector<size_t> face_points;
        for (int k = 0; k < 3; k++) {
            size_t id = new_id[tris[3 * t + k]];
            if (find(face_points.begin(), face_points.end(), id) == face_points.end()) {
                face_points.push_back(id);
            }
        }
        faces.emplace_back(face_points, tri_uv[t]);
    }
    Log(LOG_DEBUG, "Converting To Mesh");
    return CMesh(points, faces, aOpacity, arLabel);
}

CMesh CVoronoiVolume::ToMesh(const string& arLabel, normal_float aOpacity) const {
    vector<CPoint> points;
    points.reserve(mVertices.size());
    for (const auto & vertex : mVertices) {
        points.push_back((vertex * mScale) + mCenVector);
    }
    vector<CFace> faces;
    faces.reserve(mSurfFaces.Size());
    for (size_t f = 0; f < mSurfFaces.Size(); f++) {
        vector<size_t> face_points;
        const uint32_t* v = mSurfFaces.Vertices(f);
        for (size_t i = 0; i < mSurfFaces.VertexCount(f); i++) {
            if (find(face_points.begin(), face_points.end(), v[i]) == face_points.end()) {
                face_points.push_back(v[i]);
            }
        }
        faces.emplace_back(face_points, mSurfFaces.mUVcoords[f]);
    }
    return CMesh(points, faces, aOpacity, arLabel);
}

/*------------------------------------------------- Private Methods --------------------------------------------------*/

void CVoronoiVolume::CreateSurface()
{
    RunVorn();
    CleanEdges();
}

/* ---------------------------------------------- Handle RunVorn Methods ---------------------------------------------*/

void CVoronoiVolume::RunVorn()
{
    Log(LOG_VIVID, "Begin Compute Voronoi with "  + to_string(mInputPoints.size()) + " points");
    mVoronoi.ComputeVoronoi(mInputPoints, mBoxPair);
    //set the points
    {
        const vector<Vector3D> corners = mVoronoi.mData.GetFacePoints();
        mVertices.clear();
        mVertices.reserve(corners.size());
        for (const auto & corner : corners) {
            mVertices.emplace_back(corner.x, corner.y, corner.z);
        }
    }
    //set the faces
    vector<vector<size_t> > vorn_faces = mVoronoi.GetFaces();
    if (mVertices.size() >= UINT32_MAX || mInputPoints.size() >= UINT32_MAX) {
        Log(LOG_ERROR, "Too many points: more than 4 billion vertices are not supported");
    }
    mSurfFaces.Clear();
    mSurfFaces.mPairs.reserve(vorn_faces.size());
    mSurfFaces.mUVcoords.reserve(vorn_faces.size());
    mSurfFaces.mOffsets.reserve(vorn_faces.size() + 1);
    size_t total = 0;
    for (const auto & vorn_face : vorn_faces) total += vorn_face.size() - 2;
    mSurfFaces.mIndices.reserve(total);

    for (auto & vorn_face : vorn_faces) {
        // the last two numbers of every face are the two input points it lies between, the others are its corners
        const size_t c_point1 = vorn_face[vorn_face.size() - 1];
        const size_t c_point2 = vorn_face[vorn_face.size() - 2];
        for (size_t i = 0; i + 2 < vorn_face.size(); i++) {
            mSurfFaces.mIndices.push_back(static_cast<uint32_t>(vorn_face[i]));
        }
        mSurfFaces.mOffsets.push_back(static_cast<uint32_t>(mSurfFaces.mIndices.size()));
        mSurfFaces.mPairs.emplace_back(static_cast<uint32_t>(c_point1), static_cast<uint32_t>(c_point2));
        mSurfFaces.mUVcoords.push_back(0);     // defined later in the program (CleanEdges), this is a placeholder
        vector<size_t>().swap(vorn_face);      // free it now, not when the whole list goes out of scope
    }

    Log(LOG_VIVID, "Completed Voronoi Computation");
}

/* -------------------------------------------- Centralization Sub-Methods -------------------------------------------*/

vector<CPoint> CVoronoiVolume::FindContainingBox()
{
    coord_t x_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t x_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t y_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t y_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t z_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    coord_t z_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    CPoint box_dim = CPoint(x_max-x_min, y_max-y_min, z_max-z_min);
    CPoint box_min = CPoint(x_min, y_min, z_min);
    CPoint box_max = CPoint(x_max, y_max, z_max);

    return vector<CPoint>{box_dim, box_min, box_max};
}


coord_t CVoronoiVolume::FindContainingRadius()
{
    return (max_element(mInputPoints.begin(),mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Magnitude() < arV2.Magnitude();}))->Magnitude();
}

/*----------------------------------------------- Cleaning Sub-Methods -----------------------------------------------*/

void CVoronoiVolume::CleanEdges()
{
    Log(LOG_DEBUG, "Cleaning Edges");
    coord_t clean_by = 0.35;
    size_t points_size = mInputPoints.size();
    // compact the faces in place: the ones that stay keep their order
    size_t out_face = 0, out_index = 0;
    for (size_t f = 0; f < mSurfFaces.Size(); f++) {
        const pair<uint32_t, uint32_t> pr = mSurfFaces.mPairs[f];
        if (points_size > pr.first && points_size > pr.second) { //the indexs are both in range and not a part of the box
            const uint32_t* v = mSurfFaces.Vertices(f);
            const size_t nv = mSurfFaces.VertexCount(f);
            bool is_out_of_radius = false;
            for (size_t i = 0; i < nv; i++) {
                const CPoint &point = mVertices[v[i]];
                if (abs(point.X()) > (mBoxPair.second.X() * clean_by) || abs(point.Y()) > (mBoxPair.second.Y() * clean_by) || abs(point.Z()) > (mBoxPair.second.Z() * clean_by)) {
                    is_out_of_radius = true;
                }
            }
            if (!is_out_of_radius) { //the face is inside the box Radius and should be kept
                const coord_t uv = mUVcoords[pr.first] * 0.5 + mUVcoords[pr.second] * 0.5;
                const size_t begin = mSurfFaces.mOffsets[f];
                for (size_t i = 0; i < nv; i++) {      // out_index <= the read position, so this never overwrites unread data
                    mSurfFaces.mIndices[out_index++] = mSurfFaces.mIndices[begin + i];
                }
                mSurfFaces.mOffsets[out_face + 1] = static_cast<uint32_t>(out_index);
                mSurfFaces.mPairs[out_face] = pr;
                mSurfFaces.mUVcoords[out_face] = uv;
                out_face++;
            }
        }
    }
    mSurfFaces.mOffsets.resize(out_face + 1);
    mSurfFaces.mIndices.resize(out_index);
    mSurfFaces.mPairs.resize(out_face);
    mSurfFaces.mUVcoords.resize(out_face);
    mSurfFaces.mIndices.shrink_to_fit();
    Log(LOG_DEBUG, "Cleaned Edges");
}

bool sortPair(const pair<CSurfacePoint, size_t>& arobj1, const pair<CSurfacePoint, size_t>& arobj2) {
    return (arobj1.first < arobj2.first);
}

void CVoronoiVolume::CleanDoubleInputPoints(vector<pair<CSurfacePoint, size_t>> &arPoints)
{
    Log(LOG_DEBUG, "n_input_points: " + to_string(arPoints.size()));
    sort(arPoints.begin(), arPoints.end(), sortPair);

    vector<CSurfacePoint> new_points;

    vector<size_t> map;
    map.reserve(arPoints.size());
    for (int i = 0; i < arPoints.size(); i++) {
        map.push_back(arPoints[i].second);
        new_points.push_back(arPoints[i].first);
        if (i != arPoints.size() - 1) {
            while (arPoints[i].first.mPoint.Dist(arPoints[i + 1].first.mPoint) <= POINT_SIMILARITY_THRESHOLD) {
                i++;
            }
        }
    }
    mInputOutputMap = std::move(map);
    Log(LOG_DEBUG, "n_output_points: " + to_string(new_points.size()));
    mInputPoints.clear();
    mUVcoords.clear();
    for (auto & point : new_points)
    {
        mInputPoints.push_back(point.mPoint);
        mUVcoords.push_back(point.UVcoord);
    }
}



/*-------------------------------------------- Handle-Input Sub-Methods -------------------------------------------*/

void CVoronoiVolume::PreProcessPoints(vector<pair<CSurfacePoint, size_t>> &arPoints, coord_t aNoiseDisplacement)
{
    Log(ELogCode::LOG_VIVID, "Preprocessing Data");
    CleanDoubleInputPoints(arPoints);

    CPoint box_dim, box_min, box_max;
    vector<CPoint> box_dimensions = FindContainingBox();
    box_dim = box_dimensions.at(0); box_min = box_dimensions.at(1); box_max = box_dimensions.at(2);
    mCenVector = (box_min + box_max) / 2.;
    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] -= mCenVector;
    }
    mScale = FindContainingRadius() / PARTICLE_SCALE_MAGNITUDE;

    vector<CPoint> noise_vec (mInputPoints.size());
    if (aNoiseDisplacement) {
        mInputPoints = NoisifyPoints(mInputPoints, aNoiseDisplacement);
    }
    for (auto& point : mInputPoints){
        point /= mScale;
    }

    //CleanDoubleInputPoints(arPoints);

    box_dim = box_dim / mScale; box_min = (box_min - mCenVector) / mScale; box_max = (box_max - mCenVector) / mScale;
    mBoxPair = {box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR};
}