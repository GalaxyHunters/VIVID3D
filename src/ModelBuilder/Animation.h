#ifndef VIVID_ANIMATION_H
#define VIVID_ANIMATION_H

#include <string>
#include "Model.h"

namespace vivid
{

    class CAnimation {
    private:
        coord_t mDuration = 300; //in ticks
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
        virtual inline ~CAnimation() {};

        // Operator=
        inline CAnimation& operator= (const CAnimation& arAnim) {  mDuration = arAnim.mDuration; mModels = arAnim.mModels;
            mTicksPerSecond = arAnim.mTicksPerSecond; mScaleAnim = arAnim.mScaleAnim; mMoveAnim = arAnim.mMoveAnim; mRotateAnim = arAnim.mRotateAnim;return *this; }

        // Set and Get
        inline const coord_t GetDuration() const{ return mDuration; }
        inline void SetDuration(const coord_t &arDuration) { mDuration = arDuration; }


        inline std::vector<CModel> GetModels() const{ return mModels; }
        inline void SetModels(const std::vector<CModel> &arModels) { mModels = arModels; }

        inline int GetTicksPerSecond() const {return mTicksPerSecond;}
        inline void SetTicksPerSecond(int mTicksPerSecond){CAnimation::mTicksPerSecond = mTicksPerSecond;}


        inline size_t GetNumMeshes() const{
            size_t num_meshes = 0;
            for(auto it = mModels.begin(); it != mModels.end(); ++it) {num_meshes += it->GetNumMeshes();}
            return num_meshes;
        }
        inline size_t GetNumModels() const {return mModels.size();}

        /**
        * Sets Rotate animation for a specific frame
        * @param[in] aFrame what frame to affect (must be within mModels.size())
        * @param[in] arSpin CPoint value for animation (angular)
        */
        inline void SetRotateAnim(size_t aFrame, CPoint const &arSpin){
            if(aFrame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            mRotateAnim[aFrame] = arSpin;
        }

        /**
        * set movement animation at a set aFrame.
        * @param[in] aFrame what aFrame to affect (must be within mModels.size())
        * @param[in] arSpin CPoint value for animation
        */
        inline void SetMoveAnim(size_t aFrame, CPoint const &arMove){
            if(aFrame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            mMoveAnim[aFrame] = arMove;
        }

        /**
        * set Scale animation at a set aFrame
        * @param[in] aFrame what aFrame to affect (must be within mModels.size())
        * @param[in] arSpin CPoint value for animation
        */
        inline void SetScaleAnim(size_t aFrame, CPoint const &arScale){
            if(aFrame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            mScaleAnim[aFrame] = arScale;
        }

        inline CPoint& GetScaleAnim(size_t frame){
            if(frame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            return mScaleAnim[frame];
        }

        inline CPoint& GetRotateAnim(size_t frame){
            if(frame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            return mRotateAnim[frame];
        }

        inline CPoint& GetMoveAnim(size_t frame){
            if(frame > mMoveAnim.size()){
                Log(LOG_ERROR, INVALID_FRAME_VALUE);
            }
            return mMoveAnim[frame];
        }


        // Add
        void AddModels(const std::vector<CModel> &arModels);

        void AddModels(const CModel &arModels);

        //export animation
        void Export(const std::string &arOutputFilePath, std::string aFileType = "gltf2");

    };

}; // namespace vivid
#endif //VIVID_ANIMATION_H