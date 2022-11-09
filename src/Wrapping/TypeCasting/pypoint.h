#ifndef TYPECASTING_POINT_H
#define TYPECASTING_POINT_H

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "Point.h"

using namespace vivid;
namespace py = pybind11;

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

                if (!convert and !py::array_t<coord_t>::check_(src))
                    return false;

                //auto buf = py::array_t<double, py::array::c_style | py::array::forcecast>::ensure(src);
                py::buffer_info buffer = py::array_t<coord_t>::ensure(src).request();

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
                std::vector<coord_t> shape (3);

                shape[0] = src.X();
                shape[1] = src.Y();
                shape[2] = src.Z();

                py::array ret = py::cast(shape);

                //ret.inc_ref();

                return ret.release();
            }
        };
}} // namespace pybind11::detail

#endif //TYPECASTING_POINT_H