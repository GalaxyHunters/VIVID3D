#ifndef VIVID_ANIMATION_H
#define VIVID_ANIMATION_H

#include <string>
#include "Model.h"

namespace vivid
{

typedef double duration_t;

class CAnimation {
private:
    std::string mLabel = "";
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

    CAnimation(const CModel &arModel, const std::string &arLabel = "animation"){
        mModels.push_back(arModel);
        mScaleAnim.push_back(CPoint());
        mMoveAnim.push_back(CPoint());
        mRotateAnim.push_back(CPoint());
        mLabel = arLabel;
    }

    CAnimation(const std::vector<CModel> &arModels, const std::string &arLabel = "animation"){
        mModels = arModels;
        mScaleAnim = vector<CPoint>(mModels.size(), CPoint());
        mMoveAnim = vector<CPoint>(mModels.size(), CPoint());
        mRotateAnim = vector<CPoint>(mModels.size(), CPoint());
        mLabel = arLabel;
    }

    CAnimation(const CAnimation &arAnim) : mModels(arAnim.mModels), mLabel(arAnim.mLabel), mDuration(arAnim.mDuration), mTicksPerSecond(arAnim.mTicksPerSecond),
    mScaleAnim(arAnim.mScaleAnim), mMoveAnim(arAnim.mMoveAnim), mRotateAnim(arAnim.mRotateAnim){}
    ~CAnimation();

    // Operator=
    inline CAnimation& operator= (const CAnimation& arAnim) { mLabel = arAnim.mLabel; mDuration = arAnim.mDuration; mModels = arAnim.mModels;
        mTicksPerSecond = arAnim.mTicksPerSecond; mScaleAnim = arAnim.mScaleAnim; mMoveAnim = arAnim.mMoveAnim; mRotateAnim = arAnim.mRotateAnim;return *this; }

    // Set and Get
    inline const std::string GetLabel() const{ return mLabel; }
    inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }

    inline const duration_t GetDuration() const{ return mDuration; }
    inline void SetDuration(const duration_t &arDuration) { mDuration = arDuration; }


    inline std::vector<CModel> GetModels() const{ return mModels; }
    inline void SetModels(const std::vector<CModel> &arModels) { mModels = arModels; }

    int GetTicksPerSecond() const;

    void SetTicksPerSecond(int mTicksPerSecond);

    void SetRotateAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mRotateAnim[frame] = spin;
    }

    void SetMoveAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mMoveAnim[frame] = spin;
    }

    void SetScaleAnim(size_t frame, CPoint spin){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        mScaleAnim[frame] = spin;
    }

    CPoint GetScaleAnim(size_t frame){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mScaleAnim[frame];
    }

    CPoint GetRotateAnim(size_t frame){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mRotateAnim[frame];
    }

    CPoint GetMoveAnim(size_t frame){
        if(frame > mMoveAnim.size()){
            CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::INVALID_FRAME_VALUE);
        }
        return mMoveAnim[frame];
    }

    inline size_t GetNumMeshes() {
        size_t num_meshes = 0;
        for(auto it = mModels.begin(); it != mModels.end(); ++it) {num_meshes += it->GetNumMeshes();}
        return num_meshes;
    }
    // Add
    inline void AddModels(const std::vector<CModel> &arModels) {
        mModels.insert(mModels.end(), arModels.begin(), arModels.end());
        for(int i = 0; i != arModels.size(); i++){
            mScaleAnim.push_back(CPoint());
            mMoveAnim.push_back(CPoint());
            mRotateAnim.push_back(CPoint());
        }
    }
};

}; // namespace vivid
#endif //VIVID_ANIMATION_H
