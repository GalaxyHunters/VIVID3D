#pragma once
#include "Mesh.h"

class Shape
{
public:
	Shape(Mesh mesh);
	~Shape();
	const Mesh createSphere();
	const Mesh createElipsoid();
	const Mesh createArrow();
	const Mesh create3DAxes();
private:
	Mesh mesh;
};