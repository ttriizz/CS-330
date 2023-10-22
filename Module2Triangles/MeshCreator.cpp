#include "MeshCreator.h"
#include "ShapeCreator.h"
#include "Mesh.h"
#include <vector>
using namespace std;


void MeshCreator::CreateMesh(vector<GLMesh>& CreateMesh)
{
// Table Base
	GLMesh TableBase;
	TableBase.prop = 
	{
		// Colors
		1.0f, 0.0f, 1.0f, 0.0f,	
		// Scale
		8.0f, 4.0f, 8.0f,	
		// X Rotation
		0.0f, 1.0f, 0.0f, 0.0f,	
		// Y Rotation
		45.0f, 0.0f, 1.0f, 0.0f,				
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,				
		// Translation
		1.0f, 1.5f, 0.0f,					
		0.0f, 0.0f
	};
	ShapeCreator::Plane(TableBase);
	CreateMesh.push_back(TableBase);

// Candle
	GLMesh Candle;
	Candle.prop =
	{
		// Colors
		1.42f, 0.42f, 0.42f, 1.0f,
		// Scale
		5.0f, 5.0f, 0.8f,
		// X Rotation
		-90.0f, 1.0f, 0.0f, 0.0f,
		// Y Rotation
		0.0f, 0.0f, 1.0f, 0.0f,
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,
		// Translation
		4.0f, -0.5f, -5.0f,
		0.0f, 0.0f
	};
	Candle.length = 4.0f;
	Candle.radius = 0.5f;
	Candle .number_of_sides = 256.0f;
	ShapeCreator::Cylinder(Candle);
	CreateMesh.push_back(Candle);

// Candle Wax
	GLMesh CandleWax;
	CandleWax.prop =
	{
		// Colors
		10.0f, 10.0f, 10.0f, 1.0f,
		// Scale
		4.8f, 4.8f, 0.82f,
		// X Rotation
		-90.0f, 1.0f, 0.0f, 0.0f,
		// Y Rotation
		0.0f, 0.0f, 1.0f, 0.0f,
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,
		// Translation
		4.0f, -0.5f, -5.0f,
		0.0f, 0.0f
	};
	CandleWax.length = 4.0f;
	CandleWax.radius = 0.5f;
	CandleWax.number_of_sides = 256.0f;
	ShapeCreator::Cylinder(CandleWax);
	CreateMesh.push_back(CandleWax);

// Book 
	// Distort Cube into Rectangle //
	GLMesh Book;
	Book.prop =
	{
		// Colors
		6.0f, 0.0f, 0.0f, 1.0f,
		// Scale
		8.4f, 0.4f, 3.8f,
		// X Rotation
		0.0f, 1.0f, 0.0f, 0.0f,
		// Y Rotation
		33.0f, 0.0f, 1.0f, 0.0f,
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,
		// Translation
		2.8f, -0.5f, 2.0f,
		0.0f, 0.0f
	};
	ShapeCreator::Cube(Book);
	CreateMesh.push_back(Book);

 //Pen Base
	GLMesh PenBase;
	PenBase.prop = 
	{
		// Colors
		1.0f, 1.0f, 1.0f, 1.0f,
		// Scale
		0.5f, 0.5f, 0.8f,
		// X Rotation
		0.0f, 1.0f, 0.0f, 0.0f,
		// Y Rotation
		-38.0f, 0.0f, 1.0f, 0.0f,
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,
		// Translation
		5.0f, 0.15f, 0.5f,
		1.0f, 1.0f
	};
	PenBase.length = 4.0f;
	PenBase.radius = 0.5f;
	PenBase.number_of_sides = 256.0f;
	ShapeCreator::Cylinder(PenBase);
	CreateMesh.push_back(PenBase);

// Pen Tip
	GLMesh PenTip;
	PenTip.prop = 
	{
		// Color
		1.0f, 1.0f, 1.0f, 1.0f,
		// Scale
		0.5f, 0.5f, 0.5f,
		// X Rotation
		-35.0f, 0.0f, 1.0f, 0.0f,
		// Y Rotation
		0.0f, 1.0f, 0.0f, 0.0f,
		// Z Rotation
		0.0f, 0.0f, 0.0f, 1.0f,
		// Translation
		3.04f, 0.15f, 3.f,
		1.0f, 1.0f
	};
	PenTip.length = 1.0f;
	PenTip.radius = 0.5f;
	PenTip.number_of_sides = 256.0f;
	ShapeCreator::Cone(PenTip);
	CreateMesh.push_back(PenTip);
}
