#ifndef TYPECASTING_POINTARRAY_H
#define TYPECASTING_POINTARRAY_H

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "Point.h"

namespace pybind11 {
    namespace detail {
        /* ---------------------------------------------- VECTOR<CPOINT> ----------------------------------------------*/
        template <> struct type_caster<std::vector<CPoint>>
        {
        public:

            PYBIND11_TYPE_CASTER(std::vector<CPoint>, _("Point3DList"));

            // Conversion part 1 (Python -> C++)
            bool load(py::handle src, bool convert)
            {
                if ( !convert and !py::array_t<coord_t>::check_(src) )
                    return false;

                if ( !src )
                    return false;

        //            auto buf = py::array_t<double, py::array::c_style | py::array::forcecast>::ensure(src);
                py::buffer_info buffer = py::array_t<coord_t>::ensure(src).request();

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

            //Conversion part 2 (C++ -> Python)
            static py::handle cast(const std::vector<CPoint>& src, py::return_value_policy policy, py::handle parent)
            {
                std::vector<std::vector<coord_t>> shape (src.size());

                for ( int i = 0 ; i < src.size() ; i++ ) {
                    shape[i] = {src[i].X(), src[i].Y(), src[i].Z() };
                }

                py::array ret = py::cast(shape);

                ret.inc_ref();
                return ret.release();

            }
        };
}} // namespace pybind11::detail

#endif //VIVID_TYPECASTING_H