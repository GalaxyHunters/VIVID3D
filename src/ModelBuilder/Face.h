#ifndef VIVID_FACE_H
#define VIVID_FACE_H

#include <ColorDefs.h>

namespace vivid
{

    class CFace
    {
    private:
        std::vector<size_t> mPoints = {};
        coord_t mUVcoord = 0;

    public:
        inline CFace(const std::vector<size_t> &arPoints, normal_float aColor) : mPoints(arPoints), mUVcoord(max(0.0 , min(aColor , 1.))){}
        //operator=
        inline ~CFace(){}

        inline std::vector<size_t> GetPoints()const  { return mPoints; }
        inline size_t operator[](size_t I) {return mPoints[I];}
        inline coord_t GetUVcoord() const { return mUVcoord; }

        // todo: is this needed?
        inline void SetUVcoord(const coord_t aColor) { mUVcoord = aColor; }
        inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }
    };

} // namespace vivid
#endif //VIVID_FACE_H