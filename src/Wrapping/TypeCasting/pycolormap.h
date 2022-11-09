#ifndef TYPECASTING_COLORMAP_H
#define TYPECASTING_COLORMAP_H

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "ColorMap.h"

using namespace vivid;
namespace py = pybind11;

inline std::string GetTypeString(const py::handle arObj) {
    std::string type = py::str(arObj.get_type()).cast<std::string>();
    return type.substr(8, type.size() - 10);
}
constexpr auto STR = "str";
constexpr auto MPL_LINEARCOLORMAP = "matplotlib.colors.ListedColormap";
constexpr auto MPL_SEGMENTEDLINEARCOLORMAP = "matplotlib.colors.LinearSegmentedColormap";

namespace pybind11 {
    namespace detail {
        /* ------------------------------------------------- COLORMAP -------------------------------------------------*/
        template <> struct type_caster<CColorMap>
        {
        public:
            PYBIND11_TYPE_CASTER(CColorMap, const_name("matplotlib.colors.Colormap"));

            // Conversion part 1 (Python -> C++)
            bool load(py::handle arObj, bool convert) {
                if (!arObj || !convert) {
                    return false;
                }
                std::string name;
                std::vector<std::array<float, 3>> clm;
                std::string obj_type = GetTypeString(arObj);
                auto colormap = arObj;
                // std::cout<<obj_type<<std::endl;
                if (obj_type == STR) {
                    auto cm = py::module_::import("matplotlib.cm");
                    colormap = cm.attr("get_cmap")(arObj);
                    obj_type = GetTypeString(colormap);
                    // std::cout<<obj_type<<std::endl;
                }
                if (obj_type == MPL_LINEARCOLORMAP) {
                    clm = colormap.attr("colors").cast<std::vector<std::array<float, 3>>>();
                    name = colormap.attr("name").cast<std::string>();
                } else if (obj_type == MPL_SEGMENTEDLINEARCOLORMAP) {
                    name = colormap.attr("name").cast<std::string>();
                    int size = colormap.attr("N").cast<int>();
                    float r, g, b;
                    for (int i = 0; i < size; i++) {
                        auto result = colormap(i);
                        r = result[py::int_(0)].cast<float>();
                        g = result[py::int_(1)].cast<float>();
                        b = result[py::int_(2)].cast<float>();
                        clm.push_back({r, g, b});
                    }
                } else {
                    return false;
                }

                CColorMap cmap (clm, name);

                // This is what's pushed to the function
                value = cmap;

                return true;
            }
            //Conversion part 2 (C++ -> Python)
            static py::handle cast(const CColorMap& src, py::return_value_policy policy, py::handle parent)
            {
				auto LCM = py::module_::import("matplotlib.colors").attr("ListedColormap");
				auto ret = LCM("colors"_a=src.GetNormalizedColorMap(), "name"_a=src.GetName());
                return ret.release();
            }
        };
}} // namespace pybind11::detail

#endif //TYPECASTING_COLORMAP_H