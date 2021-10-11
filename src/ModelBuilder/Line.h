#ifndef VIVID_LINE_H
#define VIVID_LINE_H

#include "ModelComponent.h"

namespace vivid
{

/* 3D Lines class */
    class CLines : public CModelComponent{
    private:
        string mColor = "white";
    public:
        CLines(){}
        CLines(const std::vector<CPoint> &arPoints, coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", mColor){
            if(aAlpha > 1 || aAlpha <= 0){
                CLogFile::GetInstance().Write(ELogCode::LOG_WARNING, ELogMessage::INVALID_ALPHA_VALUE);
            }
            mPoints = arPoints;
            std::vector<size_t> points (arPoints.size());
            for (int i = 0; i < arPoints.size(); i++) { points[i] = i; }
            mFaces = {CFace(points, 0)};
        }
        // probably not usable constructor, possible remake in python wrapper.
        CLines(const std::vector<std::vector<CPoint>> &arPoints, coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", mColor){
            if(aAlpha > 1 || aAlpha <= 0){
                CLogFile::GetInstance().Write(ELogCode::LOG_WARNING, ELogMessage::INVALID_ALPHA_VALUE);
            }
            size_t size = 0;
            for (const auto & arPoint : arPoints) {
                std::vector<size_t> points (arPoint.size());
                for (size_t j = 0; j < arPoint.size(); j++) {
                    points[j] = size + j;
                    mPoints.push_back(arPoint.at(j));
                }
                mFaces.push_back(CFace(points, 0));
                size = size + arPoint.size();
            }
        }
        CLines(const CLines &arLine) : CModelComponent(arLine) {}
        ~CLines(){}

        // Operator=
        inline CLines& operator= (const CLines& arLine) { CModelComponent::operator=(arLine); return *this; }

        void AddLine(const std::vector<CPoint> &arPoints);
        // Probably worthless function below
        void AddLines(const std::vector<std::vector<CPoint>> &arPointsMatrix);
    };

}; // namespace vivid
#endif //VIVID_LINE_H