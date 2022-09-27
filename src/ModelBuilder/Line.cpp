#include "Line.h"

using namespace vivid;
using namespace std;

CLines::CLines(const std::vector<CPoint> &arLine, normal_float aOpacity, const std::string &arLabel) : CModelComponent(aOpacity, arLabel, LINES)
{
    AddLine(arLine);
    SetColor(DEFAULT_COLOR);
}

CLines::CLines(const std::vector<std::vector<CPoint>> &arLines, normal_float aOpacity, const std::string &arLabel) : CModelComponent(aOpacity, arLabel, LINES)
{
    for (const auto & line : arLines) {
        AddLine(line);
    }
    SetColor(DEFAULT_COLOR);
}

void CLines::AddLine(const vector<CPoint> &arLine)
{
    size_t size = mPoints.size();
    vector<size_t> points (arLine.size());
    for (size_t i = 0; i < arLine.size(); i++)
    {
        mPoints.push_back(arLine[i]);
        points[i] = size + i;
    }
    auto formatted_line = FormatPairPoints(points);
    for (auto &pairPoints : formatted_line) {
        mFaces.emplace_back(pairPoints, 0);
    }
}

void CLines::AddLines(const vector<vector<CPoint>> &arLines) {
    for (auto & line : arLines) {
        AddLine(line);
    }
}

vector<vector<size_t>> CLines::FormatPairPoints(const vector<size_t>& arLine)
{
    vector<vector<size_t>> formatted_lines;
    for (int i = 0; i < arLine.size() - 1; i++) {
        formatted_lines.push_back({arLine[i], arLine[i+1]});
    }
    return formatted_lines;
};