/*! \file Tessellation3D.hpp
\brief Abstract class for the tessellation in 3D
\author Elad Steinberg
*/

#ifndef TESSELLATION3D_HPP
#define TESSELLATION3D_HPP 1

#include <vector>
#include <boost/container/small_vector.hpp>
#include "Face.hpp"
typedef boost::container::small_vector<size_t, 24> face_vec;
typedef boost::container::small_vector<size_t, 8> point_vec;
typedef boost::container::small_vector<size_t, 40> tetra_vec;

using std::vector;

/*! \brief Abstract class for tessellation in 3D
\author Elad Steinberg
*/
class Tessellation3D
{
public:
#ifdef RICH_MPI
	virtual vector<Vector3D> UpdateMPIPoints(Tessellation3D const& vproc, int rank,
		vector<Vector3D> const& points, vector<std::size_t>& selfindex, vector<int>& sentproc,
		vector<vector<std::size_t> >& sentpoints) = 0;
#endif

	/*! \brief Builds the tessellation
	\param points Initial position of mesh generating points
	*/
	virtual void Build(vector<Vector3D> const& points) = 0;

#ifdef RICH_MPI
	/*! \brief Builds the tessellation
	\param points Initial position of mesh generating points
	\param tproc The tessellation of the domain decomposition
	*/
	virtual void Build(vector<Vector3D> const& points, Tessellation3D const& tproc) = 0;
#endif

	/*! \brief Get Total number of mesh generating points
	\return Number of mesh generating points
	*/
	virtual size_t GetPointNo(void) const = 0;

	virtual size_t& GetPointNo(void) = 0;

	/*! \brief Returns Position of mesh generating point
	\param index Mesh generating point index
	\return Position of mesh generating point
	*/
	virtual Vector3D GetMeshPoint(size_t index) const = 0;

	/*! \brief Returns Area of face
	\param index The index of the face
	\return The area of the face
	*/
	virtual double GetArea(size_t index) const = 0;

	virtual vector<double>& GetAllArea(void) = 0;


	/*! \brief Returns Position of Cell's Center of Mass
	\param index Mesh generating point index (the cell's index)
	\return Position of CM
	*/
	virtual Vector3D const& GetCellCM(size_t index) const = 0;

	/*! \brief Returns the total number of faces
	\return Total number of faces
	*/
	virtual size_t GetTotalFacesNumber(void) const = 0;

	/*! \brief Returns the effective width of a cell
	\param index Cell index
	\return Effective cell width
	*/
	virtual double GetWidth(size_t index) const = 0;

	/*! \brief Returns the volume of a cell
	\param index Cell index
	\return Cell volume
	*/
	virtual double GetVolume(size_t index) const = 0;

	/*! \brief Returns the indeces of a cell's Faces
	\param index Cell index
	\return Cell edges
	*/
	virtual face_vec const& GetCellFaces(size_t index) const = 0;

	virtual vector<face_vec >& GetAllCellFaces(void) = 0;

	/*!
	\brief Returns a reference to the point vector
	\returns The reference
	*/
	virtual vector<Vector3D>& GetMeshPoints(void) = 0;

	/*!
	\brief Returns a reference to the points composing the faces vector
	\returns The reference
	*/
	virtual vector<Vector3D>& GetFacePoints(void) = 0;

	/*!
	\brief Returns a reference to the points composing the faces vector
	\returns The reference
	*/
	virtual vector<Vector3D>const& GetFacePoints(void) const = 0;

	/*!
	\brief Returns a reference to the indeces of the points composing a face. Points are order in a right hand fashion, normal pointing towards the first neighbor
	\param index The index of the face
	\returns The reference
	*/
	virtual point_vec const& GetPointsInFace(size_t index) const = 0;

	virtual vector<point_vec > & GetAllPointsInFace(void) = 0;

	/*!
	\brief Returns a list of the neighbors of a cell
	\param index The cell to check
	\return The neighbors
	*/

	virtual vector<size_t> GetNeighbors(size_t index)const = 0;
	/*!
	\brief Returns a list of the neighbors of a cell
	\param index The cell to check
	\param res The neighbors, returned
	*/

	virtual void GetNeighbors(size_t index,vector<size_t> &res)const = 0;

	/*!
	\brief Cloning function
	*/
	virtual Tessellation3D* clone(void) const = 0;

	//! \brief Virtual destructor
	virtual ~Tessellation3D(void);

	/*! 
	\brief Returns if the cell is adjacent to a boundary
	\param index The cell to check
	\return If near boundary
	*/
	virtual bool NearBoundary(size_t index) const = 0;

	/*! 
	\brief Returns if the face is a boundary one
	\param index The face to check
	\return True if boundary false otherwise
	*/
	virtual bool BoundaryFace(size_t index) const = 0;

	/*!
	\brief Returns the indeces of the points that were sent to other processors as ghost points 
	\return The sent points, outer vector is the index of the cpu and inner vector are the points sent through the face
	*/
	virtual vector<vector<size_t> >& GetDuplicatedPoints(void) = 0;
	/*!
	\brief Returns the indeces of the points that were sent to other processors as ghost points
	\return The sent points, outer vector is the index of the cpu and inner vector are the points sent through the face
	*/
	virtual vector<vector<size_t> >const& GetDuplicatedPoints(void)const = 0;

	virtual vector<int> GetDuplicatedProcs(void)const = 0;

	virtual vector<int> GetSentProcs(void)const = 0;

	virtual vector<vector<size_t> > const& GetSentPoints(void)const = 0;

	virtual vector<size_t> const& GetSelfIndex(void) const = 0;

	virtual vector<int>& GetSentProcs(void) = 0;

	virtual vector<vector<size_t> > & GetSentPoints(void) = 0;

	virtual vector<size_t> & GetSelfIndex(void) = 0;

	/*!
	\brief Returns the total number of points (including ghost)
	\return The total number of points
	*/
	virtual size_t GetTotalPointNumber(void)const = 0;

	/*!
	\brief Returns the center of masses of the cells
	\return The CM's
	*/
	virtual vector<Vector3D>& GetAllCM(void) = 0;

	/*!
	\brief Returns the center of masses of the cells
	\return The CM's
	*/
	virtual vector<Vector3D> GetAllCM(void) const = 0;

	/*!
	\brief Returns the volumes of the cells
	\return The volumes
	*/
	virtual vector<double>& GetAllVolumes(void) = 0;

	/*!
	\brief Returns the volumes of the cells
	\return The volumes
	*/
	virtual vector<double> GetAllVolumes(void)const = 0;

	/*!
	\brief Returns the neighbors and neighbors of the neighbors of a cell
	\param point The index of the cell to calculate for
	\param result The neighbors and their neighbors indeces
	*/
	virtual void GetNeighborNeighbors(vector<size_t> &result,size_t point)const = 0;


	virtual std::pair<size_t,size_t> GetFaceNeighbors(size_t face_index)const = 0;

	virtual std::vector<std::pair<size_t, size_t> >& GetAllFaceNeighbors(void) = 0;

	/*!
	\brief Returns a vector normal to the face whose magnitude is the seperation between the neighboring points
	\param faceindex The index of the face
	\return The vector normal to the face whose magnitude is the seperation between the neighboring points pointing from the first neighbor to the second
	*/
	virtual Vector3D Normal(size_t faceindex)const=0;

	/*!
	\brief Checks if a point is a ghost point or not
	\param index Point index
	\return True if is a ghost point, false otherwise
	*/
	virtual bool IsGhostPoint(size_t index)const=0;

	/*!
	\brief Calculates the velocity of a face
	\param index The face index
	\param v0 The velocity of the first neighbor
	\param v1 The velocity of the second neighbor
	\return The velocity of the face
	*/
	virtual Vector3D CalcFaceVelocity(size_t index,Vector3D const& v0,Vector3D const& v1)const=0;

	virtual vector<Vector3D>& GetAllFaceCM(void) = 0;

	virtual Vector3D FaceCM(size_t index)const=0;

	virtual vector<vector<size_t> > const& GetGhostIndeces(void) const = 0;

	virtual vector<vector<size_t> > & GetGhostIndeces(void) = 0;

	virtual std::pair<Vector3D, Vector3D> GetBoxCoordinates(void)const = 0;

	virtual void BuildNoBox(vector<Vector3D> const& points, vector<vector<Vector3D> > const& ghosts, vector<size_t> toduplicate) = 0;

	virtual bool IsPointOutsideBox(size_t index)const = 0;

	virtual void output(std::string const& filename)const=0;

	virtual void SetBox(Vector3D const& ll, Vector3D const& ur) = 0;
};

point_vec_v VectorValues(std::vector<Vector3D> const&v, point_vec const &index);
#endif // TESSELLATION3D_HPP
