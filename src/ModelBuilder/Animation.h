#ifndef VIVID_ANIMATION_H
#define VIVID_ANIMATION_H

#include <string>
#include "Model.h"

namespace vivid
{
    struct CFrame
    {
    public:
        CModel mModel;
        CPoint mScaleAnimation;
        CPoint mMoveAnimation;
        CPoint mRotateAnimation;

        CFrame(const CModel &arModel, const CPoint &arScaleAnimation = CPoint(),const CPoint &arMoveAnimation = CPoint(),
               const CPoint &arRotateAnimation = CPoint()): mModel(arModel), mScaleAnimation(arScaleAnimation), mMoveAnimation(arMoveAnimation), mRotateAnimation(arRotateAnimation){}

    };
    class CAnimation {
    private:
        coord_t mDuration = 300; //in ticks
        int mTicksPerSecond = 30;
        std::vector<CFrame> mFrames = {};

//        std::vector<CModel> mModels = {};
//        std::vector<CPoint> mScaleAnim = {};
//        std::vector<CPoint> mMoveAnim = {};
//        std::vector<CPoint> mRotateAnim = {};
        // Camera route (location, angel etc)
    public:
        //Constructor, Copy Constructor, Destructor
        CAnimation(){}

        CAnimation(const CModel &arModel){
            mFrames.emplace_back(CFrame(arModel));
//            mModels.push_back(arModel);
//            mScaleAnim.push_back(CPoint());
//            mMoveAnim.push_back(CPoint());
//            mRotateAnim.push_back(CPoint());
        }

        CAnimation(const std::vector<CModel> &arModels){
            for(auto model: arModels){
                mFrames.emplace_back(CFrame(model));
            }
//            mModels = arModels;
//            mScaleAnim = vector<CPoint>(mModels.size(), CPoint());
//            mMoveAnim = vector<CPoint>(mModels.size(), CPoint());
//            mRotateAnim = vector<CPoint>(mModels.size(), CPoint());
        }



        CAnimation(const CAnimation &arAnim) : mFrames(arAnim.mFrames), mDuration(arAnim.mDuration), mTicksPerSecond(arAnim.mTicksPerSecond){}
        virtual inline ~CAnimation() {};

        inline CFrame& operator[] (const size_t index){return mFrames[index];}
        // Operator=
        inline CAnimation& operator= (const CAnimation& arAnim) {  mDuration = arAnim.mDuration; mFrames = arAnim.mFrames;
            mTicksPerSecond = arAnim.mTicksPerSecond;return *this; }

        // Set and Get
        inline const coord_t GetDuration() const{ return mDuration; }
        inline void SetDuration(const coord_t &arDuration) { mDuration = arDuration; }


        inline std::vector<CFrame>& GetFrames() { return mFrames; }
        inline void SetFrames(const std::vector<CFrame> &arFrames) { mFrames = arFrames; }

        inline int GetTicksPerSecond() const {return mTicksPerSecond;}
        inline void SetTicksPerSecond(const int aTicksPerSecond){CAnimation::mTicksPerSecond = aTicksPerSecond;}


        inline size_t GetNumMeshes() const{
            size_t num_meshes = 0;
            for(auto it = mFrames.begin(); it != mFrames.end(); ++it) {num_meshes += it->mModel.GetNumMeshes();}
            return num_meshes;
        }
        inline size_t GetNumModels() const {return mFrames.size();}

//        /**
//        * Sets Rotate animation for a specific frame
//        * @param[in] aFrame what frame to affect (must be within mModels.size())
//        * @param[in] arSpin CPoint value for animation (angular)
//        */
//        inline void SetRotateAnim(size_t aFrame, CPoint const &arSpin){
//            if(aFrame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            mRotateAnim[aFrame] = arSpin;
//        }
//
//        /**
//        * set movement animation at a set aFrame.
//        * @param[in] aFrame what aFrame to affect (must be within mModels.size())
//        * @param[in] arSpin CPoint value for animation
//        */
//        inline void SetMoveAnim(size_t aFrame, CPoint const &arMove){
//            if(aFrame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            mMoveAnim[aFrame] = arMove;
//        }
//
//        /**
//        * set Scale animation at a set aFrame
//        * @param[in] aFrame what aFrame to affect (must be within mModels.size())
//        * @param[in] arSpin CPoint value for animation
//        */
//        inline void SetScaleAnim(size_t aFrame, CPoint const &arScale){
//            if(aFrame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            mScaleAnim[aFrame] = arScale;
//        }
//
//        inline CPoint& GetScaleAnim(size_t frame){
//            if(frame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            return mScaleAnim[frame];
//        }
//
//        inline CPoint& GetRotateAnim(size_t frame){
//            if(frame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            return mRotateAnim[frame];
//        }
//
//        inline CPoint& GetMoveAnim(size_t frame){
//            if(frame > mMoveAnim.size()){
//                Log(LOG_ERROR, INVALID_FRAME_VALUE);
//            }
//            return mMoveAnim[frame];
//        }


        // Add
        void AddModels(const std::vector<CModel> &arModels);

        void AddModel(const CModel &arModel);

        //export animation
        void Export(const std::string &arOutputFilePath, std::string aFileType = "glb");

        /**
        * Assimp export. returns arModel in aFileType format
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        * @returns const void* file blob encoded in provided aFileType
        */
        CBlobData ExportToBlob(const std::string& arFileType = "glb");

    };

}; // namespace vivid
#endif //VIVID_ANIMATION_H