#include "Delaunay3D.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "Voronoi3D.hpp"
#include <iostream>
#include <string>
#include <fstream>


namespace
{
	vector<Vector3D> ReadPoints(std::string const& filename)
	{
		std::ifstream fh(filename.c_str(), std::ifstream::binary);
		int npoints;
		fh.read(reinterpret_cast<char*>(&npoints), sizeof(int));
		vector<Vector3D> res(static_cast<size_t>(npoints));
		double temp;
		for (size_t i = 0; i<static_cast<size_t>(npoints); ++i)
		{
			fh.read(reinterpret_cast<char*>(&temp), sizeof(double));
			res[i].x = temp;
			fh.read(reinterpret_cast<char*>(&temp), sizeof(double));
			res[i].y = temp;
			fh.read(reinterpret_cast<char*>(&temp), sizeof(double));
			res[i].z = temp;
		}
		return res;
	}

	bool myfunction(Vector3D i, Vector3D j)
	{
		return i.x < j.x;
	}

	vector<Vector3D> cartesian_mesh(std::size_t nx, std::size_t ny, std::size_t nz, Vector3D const& lower_left, Vector3D const& upper_right)
	{
		assert(upper_right.x > lower_left.x);
		assert(upper_right.y > lower_left.y);
		assert(upper_right.z > lower_left.z);

		vector<Vector3D> res;
		const double dx = (upper_right.x - lower_left.x) /
			static_cast<double>(nx);
		const double dy = (upper_right.y - lower_left.y) /
			static_cast<double>(ny);
		const double dz = (upper_right.z - lower_left.z) /
			static_cast<double>(nz);
		for (double x = lower_left.x + 0.5*dx; x < upper_right.x; x += dx)
		{
			for (double y = lower_left.y + 0.5*dy; y < upper_right.y; y += dy)
				for (double z = lower_left.z + 0.5*dz; z < upper_right.z; z += dz)
					res.push_back(Vector3D(x, y, z));
		}
		return res;
	}


	vector<Vector3D> RandSquare(std::size_t PointNum, Vector3D const& ll, Vector3D const& ur)
	{
		typedef boost::mt19937_64 base_generator_type;
		double ran[3];
		Vector3D diff = ur - ll;
		vector<Vector3D> res;
		Vector3D point;
		base_generator_type generator;
		boost::random::uniform_real_distribution<> dist;
		for (std::size_t i = 0; i < PointNum; ++i)
		{
			ran[0] = dist(generator);
			ran[1] = dist(generator);
			ran[2] = dist(generator);
			point.x = ran[0] * diff.x + ll.x;
			point.y = ran[1] * diff.y + ll.y;
			point.z = ran[2] * diff.z + ll.z;
			res.push_back(point);
		}
		return res;
	}
}

int main(void)
{
	std::size_t np = 10;
	Vector3D ll(-10, -10, -10), ur(10, 10, 10);
	//vector<Vector3D> points = RandSquare(np, Vector3D(-1, -1, -1), Vector3D(1, 1, 1));
	//vector<Vector3D> points = cartesian_mesh(np,np,np, Vector3D(-1, -1, -1), Vector3D(1, 1, 1));

	vector<Vector3D> points;
	double meshPar[18][3] = { 0 };
	int counter(0);
	for (int i = 2; i > -4; i -= 2) {
		for (int j = 2; j > -4; j -= 2) {
			for (int z = 2; z > -2; z -= 2) {

				points.push_back(*(new Vector3D(i, j, z)));
				counter++;
			}
		}
	}




	Delaunay3D tri;
	//tri.Build(points);
	//tri.CheckCorrect();
	//	tri.output("c:/del3.bin");
	Voronoi3D tess(Vector3D(-10, -10, -10), Vector3D(10, 10, 10));
	tess.Build(points);
	points = tess.GetMeshPoints();
	

	const double small = 1e-10;
	// Check outer box
	size_t Nface = tess.GetTotalFacesNumber();
	for (size_t i = 0; i < Nface; ++i)
		if (tess.BoundaryFace(i))
		{
			vector<size_t> const& fpoints = tess.GetPointsInFace(i);
			for (size_t j = 0; j < fpoints.size(); ++j)
			{
				Vector3D p = tess.GetFacePoints()[fpoints[j]];
				if (p.x<(ll.x - small) || p.y<(ll.y - small) || p.z<(ll.z - small) || p.x>(ur.x + small) || p.y>(ur.y + small) || p.z>(ur.z + small))
				{
					std::cout << "Incomplete outer box" << std::endl;
					assert(false);
				}
			}
		}
	// Check total volume
	double v = 0;
	//for (size_t i = 0; i < points.size(); ++i)
	//	v += tess.GetVolume(i);
	//if (v<(8 - small) || v>(8 + small))
	//{
	//	std::cout << "wrong volume " << v<<std::endl;
	//	assert(false);
	//}
	return 0;
}