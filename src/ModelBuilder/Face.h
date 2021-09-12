#ifndef VIVID_FACE_H
#define VIVID_FACE_H

#include <ColorDefs.h>

namespace vivid
{

    class CFace
    {
    private:
        std::vector<size_t> mPoints = {};
        quan_t mQuan = 0;

    public:
        inline CFace(const std::vector<size_t> &arPoints, quan_t aColor) : mPoints(arPoints), mQuan(max(0.0 , min(aColor ,1.))){}
        //operator=
        inline ~CFace(){}

        inline std::vector<size_t> GetPoints()const  { return mPoints; }
        inline size_t operator[](size_t I) {return mPoints[I];}
        inline quan_t GetQuan() const { return mQuan; }

        // todo: is this needed?
        inline void SetQuan(const quan_t aColor) { mQuan = aColor; }
        inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }
    };

} // namespace vivid
#endif //VIVID_FACE_H