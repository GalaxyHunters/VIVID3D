#include "Noisify.h"
#include <random>

using namespace std;

namespace vivid {
    std::vector<CPoint> NoisifyPoints(const std::vector<CPoint>& arPoints, coord_t aDistance) {
        // Consider generator here.
        random_device rd;
        mt19937 e2(rd());
        uniform_real_distribution<coord_t> dist(-1,1);
        vector<CPoint> noise_points;

        for (const auto & point : arPoints){
             noise_points.push_back(point + CPoint(aDistance * dist(e2), aDistance * dist(e2), aDistance * dist(e2)));
        }
        return noise_points;
    }
}
