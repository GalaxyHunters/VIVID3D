#ifndef VIVID_FACE_H
#define VIVID_FACE_H

#include "Point.h"

namespace vivid
{

class CFace
{
public:
	inline CFace(const std::vector<size_t> &arPoints, coord_t aColor) : mPoints(arPoints), mColor(aColor){}
	//operator=
	inline ~CFace(){}

	inline std::vector<size_t> GetPoints()const  { return mPoints; }
	inline size_t operator[](size_t I) {return mPoints[I];}
	inline coord_t GetColor() const { return mColor; }

	inline void SetColor(const coord_t aColor) { mColor = aColor; }
    inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }

private:
    std::vector<size_t> mPoints = {};
	// TODO: VERY BAD SHOULD BE mQuan BECAUSE OTHERWISE MULTIPLE FUNCTIONS SUCH AS GetColor() etc. BAD BAD BAD !!!
	coord_t mColor = 0;
};

} // namespace vivid
#endif //VIVID_FACE_H
