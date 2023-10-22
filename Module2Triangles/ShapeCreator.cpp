// Math for Easier Calculations 
#include <cmath>
// Basic Required Includes
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShapeCreator.h"
#include <vector>

void ShapeCreator::Plane(GLMesh& mesh)
{
	// Vectors
	vector<float> color = { mesh.prop[0], mesh.prop[1], mesh.prop[2], mesh.prop[3] }; // Set Color Properties R,G,B,A

	mesh.indice = 
	{
		// X   // Y  // Z	// Colors								// Texture X, Y
		-1.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 0.0f, 1.0f,
		 0.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,
		-1.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 0.0f, 0.0f,
		-1.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 0.0f, 1.0f,
		 0.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,
		 0.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,
		 0.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,
		 0.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 0.5f, 0.0f,
		 1.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 1.0f, 0.0f,	
		 0.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 0.5f, 1.0f,	
		 1.0f, -0.5f, 1.0f, color[0], color[1], color[2], color[3], 1.0f, 0.0f,	
		 1.0f, -0.5f, -1.0f, color[0], color[1], color[2], color[3], 1.0f, 1.0f,	

	};
	Mesh(mesh);
}

void ShapeCreator::Cylinder(GLMesh& mesh)
{
	// Vectors
	vector<float> indice;
	vector<float> color = {mesh.prop[0], mesh.prop[1], mesh.prop[2], mesh.prop[3]}; // Set Color Properties R,G,B,A
					
	float radius = mesh.radius;
	float length = mesh.length;
	float sides = mesh.number_of_sides;
	float height = mesh.height;
	const float pi = 3.14f;
	const float step = 2.0f * pi / sides;

	// Bottom Triangle Fan 
	for (auto i = 1; i < sides + 1; i++)
	{
		// Insert Each Triangle		// X									// Y								 // Z		// Colors									// Texture X, Y
		indice.insert(indice.end(), { 0.0f,									0.0f,								 0.0f,		color[0], color[1], color[2], color[3],		0.0f, 0.0f});  	
		indice.insert(indice.end(), { 0.0f + radius * cos(i * step),		0.0f + radius * sin(i * step),		 0.0f,		color[0], color[1], color[2], color[3],		0.0f + (radius * cos((i)*step)), 0.0f + (radius * sin((i)*step))});	
		indice.insert(indice.end(), { 0.0f + radius * cos((i + 1) * step),	0.0f + radius * sin((i + 1) * step), 0.0f,		color[0], color[1], color[2], color[3],		0.0f + (radius * cos((i + 1) * step)), 0.0f + (radius * sin((i + 1) * step))}); 
	}

	// Top Triangle Fan
	for (auto i = 1; i < sides + 1; i++)
	{
		// Insert Each Triangle		// X								   // Y									 // Z			// Colors										// Texture X, Y
		indice.insert(indice.end(), { 0.0f,								   0.0f,								 length,		color[0], color[1], color[2], color[3],			0.0f, 0.0f}); 
		indice.insert(indice.end(), { 0.0f + radius * cos(i * step),	   0.0f + radius * sin(i * step),		 length,		color[0], color[1], color[2], color[3],			0.0f + (radius * cos((i)*step)), 0.0f + (radius * sin((i)*step))}); 
		indice.insert(indice.end(), { 0.0f + radius * cos((i + 1) * step), 0.0f + radius * sin((i + 1) * step),  length,		color[0], color[1], color[2], color[3],			0.0f + (radius * cos((i + 1) * step)), 0.0f + (radius * sin((i + 1) * step))}); 
	}

	// Texture Origin
	const float x = 3.0f;
	// Texture Location
	float Tl = 1.0f / (sides / x); 
	// Texture Clamp
	float Tc = 0.0f; 

	// Cylinder Sides
	for (auto i = 1; i < sides + 1; i++)
	{
		// Insert Each Side			 // X									 // Y									  // Z		// Colors									// Texture X, Y
		indice.insert(indice.end(), { 0.0f + radius * cos(i * step),		 0.0f + radius * sin(i * step),		      0.0f,		color[0], color[1], color[2], color[3],		 Tc, 0}); 
		indice.insert(indice.end(), { 0.0f + radius * cos(i * step),		 0.0f + radius * sin(i * step),			  length,   color[0], color[1], color[2], color[3],		 Tc, 1.0f}); 
		indice.insert(indice.end(), { 0.0f + radius * cos((i + 1) * step),	 0.0f + radius * sin((i + 1) * step),	  length,	color[0], color[1], color[2], color[3],		 Tc + Tl, 1.0f}); 
		indice.insert(indice.end(), { 0.0f + radius * cos((i + 1) * step),	 0.0f + radius * sin((i + 1) * step),	  length,	color[0], color[1], color[2], color[3],		 Tc + Tl, 1.0f}); 
		indice.insert(indice.end(), { 0.0f + radius * cos((i + 1) * step),	 0.0f + radius * sin((i + 1) * step),	  0.0f,		color[0], color[1], color[2], color[3],		 Tc + Tl, 0.0f}); 
		indice.insert(indice.end(), { 0.0f + radius * cos(i * step),		 0.0f + radius * sin(i * step),			  0.0f,		color[0], color[1], color[2], color[3],		 Tc, 0.0f}); 
		
		// Increase Texture Clamp with Texture Location
		Tc += Tl;
	}

	mesh.indice = indice;
	indice.clear();
	Mesh(mesh);

}

void ShapeCreator::Cone(GLMesh& mesh)
{
	// Vectors
	vector<float> indice;
	vector<float> color = {mesh.prop[0], mesh.prop[1], mesh.prop[2], mesh.prop[3]}; // Set Color Properties R,G,B,A

	float radius = mesh.radius;
	float length = mesh.length;
	float sides = mesh.number_of_sides;
	constexpr float pi = 3.14f;
	const float step = 2.0f * pi / sides;

	// Texture Step + Texture X
	const float Tstep = 1.0f / sides;
	float Tx = 0.0f;

	for (auto i = 1; i < sides + 1; i++) 
	{
		// Triangle Fan - Cone Base
		// Insert Each Triangle		// X						// Y						// Z	// Colors									// Texture X, Y
		indice.insert(indice.end(), {0.0f,						0.0f,						0.0f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f});
		indice.insert(indice.end(), {radius * cos(i * step),	radius * sin(i * step),		0.0f,	color[0], color[1], color[2], color[3],		0.5f + (radius * cos((i)*step)), 0.5f + (radius * sin((i)*step))});									
		
		// Cone Sides
		// Insert Each Side			// X								//Y								// Z		// Colors									// Texture X, Y
		indice.insert(indice.end(), {radius * cos((i + 1) * step),		radius * sin((i + 1) * step),	0.0f,		color[0], color[1], color[2], color[3],		0.5f + (radius * cos((i + 1) * step)), 0.5f + (radius * sin((i + 1) * step))});								
		indice.insert(indice.end(), {radius * cos(i * step),			radius * sin(i * step),			0.0f,		color[0], color[1], color[2], color[3],		Tx, 0.0f});
		indice.insert(indice.end(), {radius * cos((i + 1) * step),		radius * sin((i + 1) * step),	0.0f,		color[0], color[1], color[2], color[3],		Tx + Tstep, 0.0f});
		
		// Cone Point
		indice.insert(indice.end(), {0.0f, 0.0f, length, color[0], color[1], color[2], color[3], Tx + (Tstep / 2), 1.0f});		

		// Increase Texture X with Texture Step
		Tx += Tstep;

	}

	mesh.indice = indice;
	indice.clear();	
	Mesh(mesh);
}

void ShapeCreator::Pyramid(GLMesh& mesh)
{
	vector<float> color = { mesh.prop[0], mesh.prop[1], mesh.prop[2], mesh.prop[3] }; // Set Color Properties R,G,B,A
	float height = mesh.height;

	mesh.indice = {
		// X    Y       Z       // Colors								 // Texture X & Y
		 0.0f,	height,	0.0f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,

		 0.0f,  height,	0.0f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,

		 0.0f,  height,	0.0f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,

		 0.0f,  height,	0.0f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,

		-0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,

		 0.5f, -0.0f, -0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		-0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f,
		 0.5f, -0.0f,  0.5f,	color[0], color[1], color[2], color[3],		0.5f, 0.5f
	};
	Mesh(mesh);
}

void ShapeCreator::Cube(GLMesh& mesh)
{
	vector<float> color = { mesh.prop[0], mesh.prop[1], mesh.prop[2], mesh.prop[3] };

	mesh.indice = {

	  // X	   // Y	   // Z			// Colors									// Texture X & Y
		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5F,	0.5f,
		0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	1.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.0f,	0.5f,
		0.5f,	1.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		-0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

		-0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	0.0f,	0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,
		0.5f,	0.0f,	-0.5f,		color[0], color[1], color[2], color[3],		0.5f,	0.5f,

	};
	Mesh(mesh);
}

// UCreateMesh Function
void ShapeCreator::Mesh(GLMesh& mesh)
{
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 4;
	const GLuint floatsPerUV = 2;
	mesh.nIndices = mesh.indice.size() / (floatsPerVertex + floatsPerUV + floatsPerColor);

	// Create VAO
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, &mesh.vbo);
	// Activate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); 

	// Buffer Vertex
	glBufferData(GL_ARRAY_BUFFER, mesh.indice.size() * sizeof(float), &mesh.indice.front(), GL_STATIC_DRAW); 

	// Vertex Strides
	const GLint stride = (sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerColor));

	// Create Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)*floatsPerVertex));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerColor)));
	glEnableVertexAttribArray(2);

	// Scale Obj
	mesh.scale = glm::scale(glm::vec3(mesh.prop[4], mesh.prop[5], mesh.prop[6]));
	const glm::mat4 rotate = glm::mat4(1.0f);

	// Rotate Obj
	mesh.rotateX = glm::rotate(rotate, glm::radians(mesh.prop[7]), glm::vec3(mesh.prop[8], mesh.prop[9], mesh.prop[10]));
	mesh.rotateY = glm::rotate(rotate, glm::radians(mesh.prop[11]), glm::vec3(mesh.prop[12], mesh.prop[13], mesh.prop[14]));
	mesh.rotateZ = glm::rotate(rotate, glm::radians(mesh.prop[15]), glm::vec3(mesh.prop[16], mesh.prop[17], mesh.prop[18]));

	// Move Obj X , Y , Z
	mesh.translation = glm::translate(glm::vec3(mesh.prop[19], mesh.prop[20], mesh.prop[21]));
	mesh.model = mesh.translation * mesh.rotateX * mesh.rotateY * mesh.rotateZ * mesh.scale;

	// Scale Texture to Obj
	mesh.gTexScale = glm::vec2(mesh.prop[22], mesh.prop[23]);

}