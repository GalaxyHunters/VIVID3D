#ifndef VIVID_COLORDEFS_H
#define VIVID_COLORDEFS_H

#include <functional>
#include <map>
#include <vector>

using namespace std;

typedef double quan_t;

constexpr int RGB_MAX = 255;
//
// Created by ghil on 13/09/2021.
//

#include "ColorDefs.h"

struct color_t {
    quan_t R;
    quan_t G;
    quan_t B;

    inline color_t(quan_t R, quan_t G, quan_t B) : R(R), G(G), B(B) {}
};

typedef std::function<std::vector<color_t>()> FColor_t;

static std::vector<color_t> Black()
{
    return std::vector<color_t> {color_t(0., 0., 0.),};
}
static std::vector<color_t> White()
{
    return std::vector<color_t> {color_t(1., 1., 1.),};
}
static std::vector<color_t> Red()
{
    return std::vector<color_t> {color_t(1., 0., 0.),};
}
static std::vector<color_t> Green()
{
    return std::vector<color_t> {color_t(0., 1., 0.),};
}
static std::vector<color_t> Blue()
{
    return std::vector<color_t> {color_t(0., 0., 1.),};
}
static std::vector<color_t> Yellow()
{
    return std::vector<color_t> {color_t(1., 1., 0.),};
}
static std::vector<color_t> Purple()
{
    return std::vector<color_t> {color_t(1., 0., 1.),};
}
static std::vector<color_t> Cyan()
{
    return std::vector<color_t> {color_t(0., 1., 1.),};
}
static std::vector<color_t> Orange()
{
    return std::vector<color_t> {color_t(1., 0.6, 1.),};
}

std::vector<color_t> GetColorByString(const std::string &arCName);

#endif //VIVID_COLORDEFS_H