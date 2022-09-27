#ifndef VIVID_LINE_H
#define VIVID_LINE_H

#include "ModelComponent.h"

namespace vivid
{
    constexpr auto DEFAULT_COLOR = "white";
/* 3D Lines class */
    class CLines : public CModelComponent{
    private:
        static std::vector<std::vector<size_t>> FormatPairPoints(const std::vector<size_t>& arLine);
    public:
        CLines() : CModelComponent(LINES) {}
        CLines(const std::vector<CPoint> &arLine, normal_float aOpacity, const std::string &arLabel);
        // probably not usable constructor, possible remake in python wrapper.
        CLines(const std::vector<std::vector<CPoint>> &arLines, normal_float aOpacity, const std::string &arLabel);
        CLines(const CLines &arLine) : CModelComponent(arLine) {}
        ~CLines(){}

        // Operator=
        inline CLines& operator= (const CLines& arLine) { CModelComponent::operator=(arLine); return *this; }

        void AddLine(const std::vector<CPoint> &arLine);
        // Probably worthless function below
        void AddLines(const std::vector<std::vector<CPoint>> &arLines);
    };

}; // namespace vivid
#endif //VIVID_LINE_H