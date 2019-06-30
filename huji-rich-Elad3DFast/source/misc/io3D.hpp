/*! \file io3D.hpp
\brief A collection of simple input / output methods for Vector3D
\author Elad Steinberg
*/

#ifndef IO3D_HPP
#define IO3D_HPP 1

#include "simple_io.hpp"
#include "../3D/GeometryCommon/Vector3D.hpp"

/*! \brief Writes a binary vector of Vector3D to a file
\param vec The vector to write
\param fname Name of the file
*/
void write_vec3d(std::vector<Vector3D> const&vec, std::string const& fname);

/*! \brief Reads a binary vector of Vector3D from a file
\param fname Name of the file
\return The vector
*/
std::vector<Vector3D> read_vec3d(std::string fname);

/*! \brief Writes a binary vector of size_t to a file
\param vec The vector to write
\param fname Name of the file
*/
void write_vecst(std::vector<size_t> const&vec, std::string const& fname);

/*! \brief Writes a vector of size_t to a file
\param vec The vector to write
\param fname Name of the file
*/
void write_vecint(std::vector<int> const&vec, std::string const& fname);

/*! \brief Reads a  binary vector of size_t from a file
\param fname Name of the file
\return The vector
*/
std::vector<size_t> read_vecst(std::string fname);

/*! \brief Reads a binary vector of int from a file
\param fname Name of the file
\return The vector
*/
std::vector<int> read_vecint(std::string fname);

#endif // IO3D_HPP
