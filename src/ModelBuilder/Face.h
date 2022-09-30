#ifndef VIVID_FACE_H
#define VIVID_FACE_H

#include <ColorDefs.h>

namespace vivid
{

    class CFace
    {
    private:
        std::vector<size_t> mPoints = {};
        normal_float mUVcoord = 0;

    public:
        inline CFace(const std::vector<size_t> &arPoints, normal_float aColor) : mPoints(arPoints), mUVcoord(aColor){}
        //operator=
        inline ~CFace(){}

        inline std::vector<size_t> GetPoints()const  { return mPoints; }
        inline size_t operator[](size_t I) {return mPoints[I];}
        inline normal_float GetUVcoord() const { return mUVcoord; }

        // todo: is this needed?
        inline void SetUVcoord(const coord_t aColor) { mUVcoord = aColor; }
        inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }
    };

} // namespace vivid
#endif //VIVID_FACE_H