#ifndef VIVID_TYPECASTING_H
#define VIVID_TYPECASTING_H

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "Point.h"
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
        /* -------------------------------------------------- CPOINT --------------------------------------------------*/
        template <> struct type_caster<CPoint>
        {
        public:
            PYBIND11_TYPE_CASTER(CPoint, _("Point3D"));

            // Conversion part 1 (Python -> C++)
            bool load(py::handle src, bool convert) {
                if (!src)
                    return false;

                if (!convert and !py::array_t<double>::check_(src))
                    return false;

                //auto buf = py::array_t<double, py::array::c_style | py::array::forcecast>::ensure(src);
                py::buffer_info buffer = py::array_t<double>::ensure(src).request();

                auto dims = buffer.ndim;
                if (dims != 1)
                    return false;

                size_t X = buffer.shape[0];

                if (X != 3)
                    return false;

                CPoint point;

                auto *ptr = (coord_t *) buffer.ptr;

                point = CPoint(ptr[0], ptr[1], ptr[2]);

                // This is what's pushed to the function
                value = point;

                return true;
            }

            //Conversion part 2 (C++ -> Python)
            static py::handle cast(const CPoint& src, py::return_value_policy policy, py::handle parent)
            {
                std::vector<double> shape (3);

                shape[0] = src.X();
                shape[1] = src.Y();
                shape[2] = src.Z();

                py::array ret = py::cast(shape);

                return ret;
            }
        };

        /* ---------------------------------------------- VECTOR<CPOINT> ----------------------------------------------*/
        template <> struct type_caster<std::vector<CPoint>>
        {
        public:

            PYBIND11_TYPE_CASTER(std::vector<CPoint>, _("Point3DList"));

            // Conversion part 1 (Python -> C++)
            bool load(py::handle src, bool convert)
            {
                if ( !convert and !py::array_t<double>::check_(src) )
                    return false;

                if ( !src )
                    return false;

        //            auto buf = py::array_t<double, py::array::c_style | py::array::forcecast>::ensure(src);
                py::buffer_info buffer = py::array_t<double>::ensure(src).request();

                auto dims = buffer.ndim;

                if (dims != 2) {
                    return false;
                }

                size_t X = buffer.shape[0];
                size_t Y = buffer.shape[1];

                std::vector<CPoint> point_vector (X);

                if (Y != 3)
                    return false;

                auto *ptr = (coord_t *) buffer.ptr;

                for (size_t idx = 0; idx < X; idx++) {
                    point_vector[idx] = {ptr[idx*3+0], ptr[idx*3+1], ptr[idx*3+2]};
                }

                value = point_vector;

                return true;
            }

//            //Conversion part 2 (C++ -> Python)
//            static py::handle cast(const std::vector<CPoint>& src, py::return_value_policy policy, py::handle parent)
//            {
//                std::vector<std::vector<double>> shape (src.size());
//
//                for ( int i = 0 ; i < src.size() ; i++ ) {
//                    shape[i] = {src[i].X(), src[i].Y(), src[i].Z() };
//                }
//
//                py::array ret = py::cast(shape);
//
//                return ret;
//
//            }
        };
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

        /* -------------------------------------------- MATRIX<CPOINT> --------------------------------------------*/
        // TODO: FIgure out how to do this, if the np 3d matrix has different lengths then its ndim is 1 and impossible to parse using the current strategy
        // SOLUTION 1: Only accept proper np matrix
        // SOLUTION 2: Just take the whole buffer (flattened) and read 3 values at a time
        //    template <> struct type_caster<std::vector<std::vector<<CPoint>>>
        //    {
        //    public:
        //
        //    PYBIND11_TYPE_CASTER(vector<CPoint>, _("CPointMatrix"));
        //
        //        // Conversion part 1 (Python -> C++)
        //        bool load(py::handle src, bool convert)
        //        {
        //            if ( !convert and !py::array_t<double>::check_(src) )
        //                return false;
        //
        //            if ( !src )
        //                return false;
        //
        ////            auto buf = py::array_t<double, py::array::c_style | py::array::forcecast>::ensure(src);
        //            py::buffer_info buffer = py::array_t<double>::ensure(src).request();
        //
        //            auto dims = buffer.ndim;
        //
        //
        //            if ( dims != 3) {
        //                return false;
        //            }
        //
        //            size_t X = buffer.shape[0];
        //            size_t Y = buffer.shape[1];
        //            size_t Z = buffer.shape[2];
        //
        //            if (Z != 3) {
        //                return false;
        //            }
        //
        //            std::vector<std::vector<CPoint>> point_vector (X);
        //
        //            auto *ptr = (coord_t *) buffer.ptr;
        //
        //            for (size_t idx = 0; idx < X; idx++) {
        //                point_vector[idx] = {ptr[idx*3+0], ptr[idx*3+1], ptr[idx*3+2]};
        //            }
        //
        //            value = point_vector;
        //
        //            return true;
        //        }
        //
        //        //Conversion part 2 (C++ -> Python)
        //        static py::handle cast(const std::vector<CPoint>& src, py::return_value_policy policy, py::handle parent)
        //        {
        //            std::vector<std::vector<double>> shape (src.size());
        //
        //            for ( int i = 0 ; i < src.size() ; i++ ) {
        //                shape[i] = {src[i].X(), src[i].Y(), src[i].Z() };
        //            }
        //
        //            py::array ret = py::cast(shape);
        //
        //            return ret;
        //
        //        }
        //    };
}} // namespace pybind11::detail

#endif //VIVID_TYPECASTING_H