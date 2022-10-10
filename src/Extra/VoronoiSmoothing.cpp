#include "VoronoiSmoothing.h"

using namespace std;
using namespace vivid;

CMesh CVoronoiSmoothing::Smooth(CVoronoiVolume &arSurf, bool aSuperSmooth, int aSmoothFactor)
{
    mSurf = CVoronoiVolume(arSurf);
    //begin smooth part 1, collecting all the cpoints from the faces on the surf
    SetPinPout( );
    UpdateInput( );
    mSurf.CreateSurface();
    CalculatePointNeighbours();
    //begin smooth  part 2, adding new points between the cpoints by aSmoothFactor
    UpdatePoutPin();
    if (aSuperSmooth)
    {
        Stage2ModifyPoints();
    }
    else
    {
        Stage2AddPoints(aSmoothFactor);
    }

    //begin smooth part 3, running the model and cleaning it
    UpdateInput();
    MakeMask();
    arSurf.CreateSurface();

    return arSurf.ToMesh("", 1.);
}


/*---------------------------------------------- Smoothing Sub-Methods -----------------------------------------------*/
/*----------------------------------------------------- Part 1 -------------------------------------------------------*/

void CVoronoiSmoothing::SetPinPout()
{
    map<size_t, bool> p_in_map;
    map<size_t, bool> p_out_map;
    size_t c_point1, c_point2;
    for (auto & mVecFace : mSurf.GetFaces()) {
        c_point1 = get<0>(mVecFace.mPairPoints);
        c_point2 = get<1>(mVecFace.mPairPoints);
        if (mSurf.GetMask()[c_point1]) {
            if (p_in_map.count(c_point1) == 0) {
                mInPoints.push_back(c_point1);
                p_in_map[c_point1] = true;
            }
            if (p_out_map.count(c_point2) == 0) {
                mOutPoints.push_back(c_point2);
                p_out_map[c_point2] = true;
            }
        }
        else
        {
            if (p_in_map.count(c_point2) == 0) {
                mInPoints.push_back(c_point2);
                p_in_map[c_point2] = true;
            }
            if (p_out_map.count(c_point1) == 0) {
                mOutPoints.push_back(c_point1);
                p_out_map[c_point1] = true;
            }
        }
    }
}

void CVoronoiSmoothing::UpdateInput()
{
    vector<CPoint> new_points;
    vector<normal_float> new_quan;
    for (auto & it : mOutPoints) {
        new_points.push_back(mSurf.GetInputPoints()[it]);
        new_quan.push_back(mSurf.GetUVcoords()[it]);
    }
    for (auto & it : mInPoints) {
        new_points.push_back(mSurf.GetInputPoints()[it]);
        new_quan.push_back(mSurf.GetUVcoords()[it]);
    }
    mSurf.SetInputPoints(new_points);
    mSurf.SetUVcoords(new_quan);
}

/*----------------------------------------------------- Part 2 -------------------------------------------------------*/

void CVoronoiSmoothing::UpdatePoutPin()
{
    for (size_t i = 0; i < mOutPoints.size(); i++) {
        mOutPoints[i] = i;
    }
    for (size_t i = 0; i < mInPoints.size(); i++) {
        mInPoints[i] = i + mOutPoints.size();
    }
}

void CVoronoiSmoothing::CalculatePointNeighbours() {
    for (size_t i = 0; i < mSurf.GetFaces().size(); i++) {
        pair<size_t, size_t> n_points = mSurf.GetFaces()[i].mPairPoints;
        mPointNeighbours[n_points.first].insert(i);
        mPointNeighbours[n_points.second].insert(i);
    }
}

// Alt Stage2AddPoints
void CVoronoiSmoothing::Stage2ModifyPoints()
{
    size_t c_point1;
    size_t c_point2;
    size_t c_point3;

    vector<CPoint> new_points;
    vector<normal_float> new_quan;
    size_t p_out_size = mOutPoints.size();
    size_t p_in_size = p_out_size + mInPoints.size();
    mInPoints.clear();
    mOutPoints.clear();
    size_t new_index = 0; // the index for the new point to be added
    //go over pout
    for (auto & mVecFace : mSurf.GetFaces()) {
        c_point1 = get<0>(mVecFace.mPairPoints);
        c_point2 = get<1>(mVecFace.mPairPoints);
        FindPairPoints(c_point1, c_point2, mInPoints, mOutPoints, p_out_size, p_in_size, mVecFace, new_points, new_quan, new_index);
        FindPairPoints(c_point2, c_point1, mInPoints, mOutPoints, p_out_size, p_in_size, mVecFace, new_points, new_quan, new_index);
    }
    CleanDoublePointsVorn(new_points, new_quan, mInPoints, mOutPoints);
}

void CVoronoiSmoothing::FindPairPoints(size_t aCPoint1, size_t aCPoint2, vector<size_t> &mInPoints, vector<size_t> &mOutPoints, size_t mOutPointsSize, size_t mInPointsSize,
                                       const CSurfaceFace &arFace, vector<CPoint> &arNewPoints, vector<normal_float> &arNewQuan, size_t &arIndex)
{
    size_t c_point3;
    for (auto & f_index : mPointNeighbours[aCPoint1])
    {
        CSurfaceFace face = mSurf.GetFaces()[f_index];
        if (face.mPairPoints != arFace.mPairPoints) {
            if (face.mPairPoints.first == aCPoint1) {
                c_point3 = get<1>(face.mPairPoints);
            } else {
                c_point3 = get<0>(face.mPairPoints);
            }
            if (aCPoint1 < mOutPointsSize && aCPoint2 < mOutPointsSize && c_point3 < mOutPointsSize) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
            {
                AddPointsAlt(mInPoints, arNewPoints, arNewQuan, arIndex, aCPoint1, aCPoint2, c_point3);
            }
            //go over pin
            if ((mInPointsSize > aCPoint1 && aCPoint1 >= mOutPointsSize) && (mInPointsSize > aCPoint2 && aCPoint2 >= mOutPointsSize) &&
                (mInPointsSize > c_point3 && c_point3 >= mOutPointsSize)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
            {
                AddPointsAlt(mOutPoints, arNewPoints, arNewQuan, arIndex, aCPoint1, aCPoint2, c_point3);
            }
        }
    }
}

void CVoronoiSmoothing::AddPointsAlt(vector<size_t> &arPVec, vector<CPoint> &arNewPoints, vector<normal_float> &arNewQuan,
                                     size_t &arNewIndex, size_t aCPoint1, size_t aCPoint2, size_t aCPoint3)
{
    arPVec.push_back(arNewIndex);
    CPoint new_point = (mSurf.GetInputPoints()[aCPoint1] * 2 + mSurf.GetInputPoints()[aCPoint2] + mSurf.GetInputPoints()[aCPoint3]) / 4.;
    arNewPoints.push_back(new_point);
    arNewQuan.push_back((mSurf.GetUVcoords()[aCPoint1] * 2 + mSurf.GetUVcoords()[aCPoint2] + mSurf.GetUVcoords()[aCPoint3]) / 4); // Results in color washing, need to consider
    arNewIndex++;
}

void CVoronoiSmoothing::Stage2AddPoints(size_t aSmoothFactor)
{
    size_t c_point1;
    size_t c_point2;
    size_t p_out_size = mOutPoints.size();
    size_t p_in_size = p_out_size + mInPoints.size();
    vector<CPoint> new_points;
    vector<normal_float> new_quan;
    mInPoints.clear();
    mOutPoints.clear();
    size_t new_index = 0; // the index for the new point to be added
    //go over pout
    for (auto & mVecFace : mSurf.GetFaces()) {
        c_point1 = get<0>(mVecFace.mPairPoints);
        c_point2 = get<1>(mVecFace.mPairPoints);
        if (c_point1 < p_out_size && c_point2 < p_out_size) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
        {
            AddPoints(&mInPoints, &new_points, &new_quan, &new_index, c_point1, c_point2, aSmoothFactor);
        }
        //go over pin
        if ((p_in_size > c_point1 && c_point1 >= p_out_size) && (p_in_size > c_point2 && c_point2 >= p_out_size)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
        {
            AddPoints(&mOutPoints, &new_points, &new_quan, &new_index, c_point1, c_point2, aSmoothFactor);
        }
    }
    CleanDoublePointsVorn(new_points, new_quan, mInPoints, mOutPoints);
}

void CVoronoiSmoothing::AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<normal_float> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2, int aSmoothFactor)
{
    coord_t x, y, z;
    for (int i = 1; i <= aSmoothFactor; i++){
        (*apPVec).push_back(*apNewIndex);
        x = (mSurf.GetInputPoints()[aCPoint1].X() * (aSmoothFactor + 1 - i) + mSurf.GetInputPoints()[aCPoint2].X() * i) / (aSmoothFactor+1);
        y = (mSurf.GetInputPoints()[aCPoint1].Y() * (aSmoothFactor + 1 - i) + mSurf.GetInputPoints()[aCPoint2].Y() * i) / (aSmoothFactor+1);
        z = (mSurf.GetInputPoints()[aCPoint1].Z() * (aSmoothFactor + 1 - i) + mSurf.GetInputPoints()[aCPoint2].Z() * i) / (aSmoothFactor+1);
        (*apNewPoints).push_back(CPoint(x, y, z));
        (*apNewQuan).push_back((mSurf.GetUVcoords()[aCPoint1] * (aSmoothFactor + 1 - i) + mSurf.GetUVcoords()[aCPoint2] * i) / (aSmoothFactor+1));
        (*apNewIndex)++;
    }
}

void CVoronoiSmoothing::CleanDoublePointsVorn(vector<CPoint>& arNewPoints, vector<normal_float>& arNewQuan, vector<size_t>& arNewIn, vector<size_t>& arNewOut)
{
    vector<CSurfacePoint> data;
    data.clear();
    for (auto & it : arNewIn) {
        data.push_back(CSurfacePoint(arNewPoints[it], arNewQuan[it], true));
    }
    for (auto & it : arNewOut) {
        data.push_back(CSurfacePoint(arNewPoints[it], arNewQuan[it], false));
    }
    data = mSurf.RemoveDoublesVornInput(data);
    arNewPoints.clear();
    arNewQuan.clear();
    arNewIn.clear();
    arNewOut.clear();
    for (size_t i = 0; i < data.size(); i++) {
        arNewPoints.push_back(data[i].mPoint);
        arNewQuan.push_back(data[i].UVcoord);
        if (data[i].mMaskIsTrue) {
            arNewIn.push_back(i);
        }
        else
        {
            arNewOut.push_back(i);
        }
    }
    mSurf.SetInputPoints(arNewPoints);
    mSurf.SetUVcoords(arNewQuan);
}


/*----------------------------------------------------- Part 3 -------------------------------------------------------*/

void CVoronoiSmoothing::MakeMask()
{
    vector<bool> new_mask;
    for (size_t i = 0; i < mOutPoints.size(); i++) { //TODO might be faster using fill or something similar in vector
        new_mask.push_back(false);

    }
    for (size_t i = 0; i < mInPoints.size(); i++) {
        new_mask.push_back(true);
    }
    mSurf.SetMask(new_mask);
}