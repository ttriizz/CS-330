#pragma once
#include "Mesh.h"
#include <vector>
using namespace std;

class ShapeCreator
{
	public:
		static void Plane(GLMesh& mesh);
		static void Cylinder(GLMesh& mesh);
		static void Cone(GLMesh& mesh);
		static void Pyramid(GLMesh& mesh);
		static void Cube(GLMesh& mesh);
		static void Mesh(GLMesh& mesh);
};