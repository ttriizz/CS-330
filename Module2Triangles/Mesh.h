#pragma once
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnOpengl/camera.h> 
#include <vector>

struct GLMesh
{
	// Array
	GLuint vao;
	// Buffers
	GLuint vbo;
	GLuint vbos[2];
	// Indices
	GLuint nIndices;
	std::vector<float> indice;

	// Shape Properties = Location, Scale, Rotation, and Color
	std::vector<float> prop;
	float height;
	float length;
	float radius;
	float number_of_sides;

	// Matrix Obj
	glm::mat4 scale;
	glm::mat4 rotateX;
	glm::mat4 rotateY;
	glm::mat4 rotateZ;
	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 model;
	glm::vec2 gTexScale;

	class Mesh
	{

	};
};