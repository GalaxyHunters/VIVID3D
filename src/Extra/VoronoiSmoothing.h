#ifndef VIVID_VORONOISMOOTHING_H
#define VIVID_VORONOISMOOTHING_H
#include "VoronoiVolume.h"

namespace vivid
{

// Deprecated smoothing algorithm using Voronoi, essentially a overcomplicated subdivide algorithm thats just a it better at dealing with pointy faces than laplacian smooth
// Supplemented by RemovePointyFaces and LaplacianSmooth
    class CVoronoiSmoothing {
    public:
        /**
         * Smooth method
         * @param[in] aSuperSmooth boolean value for activating SuperSmooth Algorithm
         * @param[in] aSmoothFactor An integer value between 1 and 8
         */
        CMesh Smooth(CVoronoiVolume &arSurf, bool aSuperSmooth = false, int aSmoothFactor = 2);

    private:
        CVoronoiVolume mSurf;

        // Calculation data needed for VoronoiSmoothing
        std::map<size_t, std::unordered_set<size_t>> mPointNeighbours = {};
        std::vector<size_t> mInPoints, mOutPoints = {};

        // Smoothing Sub-Methods
        void CalculatePointNeighbours();
        // Part 1
        void SetPinPout ();
        void UpdateInput();
        // Part 2
        void UpdatePoutPin();
        void Stage2ModifyPoints();
        // TODO: Way too many fucking arguments, at least clean it up a bit by making them class variables. Rename the supersmooth functions as well just for better readability
        void FindPairPoints(size_t aCPoint1, size_t aCPoint2, std::vector<size_t> &arPIn, std::vector<size_t> &arPOut, size_t aPOutSize, size_t aPInSize,
                            const CSurfaceFace &arFace, std::vector<CPoint> &arNewPoints, std::vector<normal_float> &arNewQuan, size_t &arIndex);
        void AddPointsAlt(std::vector<size_t> &arPVec, std::vector<CPoint> &arNewPoints, std::vector<normal_float> &arNewQuan,
                          size_t &arNewIndex, size_t aCPoint1, size_t aCPoint2, size_t aCPoint3); // Adds points between every pair by aSmoothFactor
        void Stage2AddPoints(size_t aSmoothFactor);
        void AddPoints(std::vector<size_t> * apPVec, std::vector<CPoint> * apNewPoints, std::vector<normal_float> * apNewQuan,
                       size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2, int aSmoothFactor); // Adds points between every pair by aSmoothFactor
        void CleanDoublePointsVorn(std::vector<CPoint>& arNewPoints, std::vector<normal_float>& arNewQuan,
                                   std::vector<size_t>& arNewIn, std::vector<size_t>& arNewOut); // Handles the reallocation after RemoveDoublesVornInput
        // Part 3
        void MakeMask();
    };

}; //namespace vivid

#endif //VIVID_VORONOISMOOTHING_H