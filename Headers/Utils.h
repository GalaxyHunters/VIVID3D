#pragma once
#include "Surf.h"

Surf computeVoronoi(vector<double[3]> inputPoints, vector<float> quan);
Mesh computeSimplify(float errorPercent);