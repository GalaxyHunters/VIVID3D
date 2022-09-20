#ifndef VIVID_ANIMATION_H
#define VIVID_ANIMATION_H

#include <string>
#include "Model.h"

namespace vivid
{

typedef double duration_t;

class CAnimation {
private:
    duration_t mDuration = 300; //in ticks
    int mTicksPerSecond = 30;
    std::vector<CModel> mModels = {};
    std::vector<CPoint> mScaleAnim = {};
    std::vector<CPoint> mMoveAnim = {};
    std::vector<CPoint> mRotateAnim = {};
    // Camera route (location, angel etc)
public:
    //Constructor, Copy Constructor, Destructor
    CAnimation(){}

    CAnimation(const CModel &arModel){
        mModels.push_back(arModel);
        mScaleAnim.push_back(CPoint());
        mMoveAnim.push_back(CPoint());
        mRotateAnim.push_back(CPoint());
    }

    CAnimation(const std::vector<CModel> &arModels){
        mModels = arModels;
        mScaleAnim = vector<CPoint>(mModels.size(), CPoint());
        mMoveAnim = vector<CPoint>(mModels.size(), CPoint());
        mRotateAnim = vector<CPoint>(mModels.size(), CPoint());
    }



    CAnimation(const CAnimation &arAnim) : mModels(arAnim.mModels), mDuration(arAnim.mDuration), mTicksPerSecond(arAnim.mTicksPerSecond),
    mScaleAnim(arAnim.mScaleAnim), mMoveAnim(arAnim.mMoveAnim), mRotateAnim(arAnim.mRotateAnim){}
    inline ~CAnimation() {};

    // Operator=
    inline CAnimation& operator= (const CAnimation& arAnim) {  mDuration = arAnim.mDuration; mModels = arAnim.mModels;
        mTicksPerSecond = arAnim.mTicksPerSecond; mScaleAnim = arAnim.mScaleAnim; mMoveAnim = arAnim.mMoveAnim; mRotateAnim = arAnim.mRotateAnim;return *this; }

    // Set and Get
    inline const duration_t GetDuration() const{ return mDuration; }
    inline void SetDuration(const duration_t &arDuration) { mDuration = arDuration; }


    inline std::vector<CModel> GetModels() const{ return mModels; }
    inline void SetModels(const std::vector<CModel> &arModels) { mModels = arModels; }

    inline int GetTicksPerSecond() const {return mTicksPerSecond;}
    inline void SetTicksPerSecond(int mTicksPerSecond){CAnimation::mTicksPerSecond = mTicksPerSecond;}

    /*
     * set rotate animation at a set frame. value is in EULER angles
     */
    inline void SetRotateAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mRotateAnim[frame] = spin;
    }

    /*
     * set movement animation at a set frame. value is movement vector
     */
    inline void SetMoveAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mMoveAnim[frame] = spin;
    }

    /*
     * set Scale animation at a set frame. value is scale change vector
     */
    inline void SetScaleAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mScaleAnim[frame] = spin;
    }

    inline CPoint GetScaleAnim(size_t frame) const{
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mScaleAnim[frame];
    }

    inline CPoint GetRotateAnim(size_t frame) const{
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mRotateAnim[frame];
    }

    inline CPoint GetMoveAnim(size_t frame) const {
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mMoveAnim[frame];
    }

    inline size_t GetNumMeshes() const{
        size_t num_meshes = 0;
        for(auto it = mModels.begin(); it != mModels.end(); ++it) {num_meshes += it->GetNumMeshes();}
        return num_meshes;
    }
    // Add
    void AddModels(const std::vector<CModel> &arModels);

    void AddModels(const CModel &arModels);

    //export animation
    void Export(const std::string &arOutputFilePath, std::string aFileType = "obj");

    //used for stop motion
    virtual double GetSecondsPerFrame() const {};
};

}; // namespace vivid
#endif //VIVID_ANIMATION_H
