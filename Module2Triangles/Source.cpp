/// Jeremiah Schoenherr $ SNHU CS-330 Project ///

// Includes
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      
#include "ShapeCreator.h"
#include "Mesh.h"
#include "MeshCreator.h"
#include <vector>
ShapeCreator creator;
vector<GLMesh> CreateMesh;

// Shader Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

const char* const WINDOW_TITLE = "Mod 7 Project - Jeremiah Schoenherr";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Camera Setup
Camera gCamera(glm::vec3(-5.0f, 4.5f, -0.3f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f, -30.0f);
float gLastX = WINDOW_WIDTH / 2.0f;
float gLastY = WINDOW_HEIGHT / 2.0f;
float yaw = 0;
float pitch = 0;
bool gFirstMouse = true;
glm::vec3 gCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 gCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool viewType = false;

// Frame Timing
float gDeltaTime = 0.0f; 
float gLastFrame = 0.0f;

using namespace std;

GLFWwindow* gWindow = nullptr;
GLuint gProgramId;
GLuint gPlastic;
GLuint gTexBrick;
GLuint gLampProgramId;

struct GLight {
	GLuint vao;
	GLuint vbo;
	GLuint nVertices;
};

GLight lampLight;

//light color
glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);

// Light position and scale
glm::vec3 gLightPosition(6.0f, 2.0f, 3.0f);
glm::vec3 gLightScale(0.3f);

// Vertex Shader Source Code
const GLchar* vertexShaderSource = GLSL(440,
layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

	vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

	vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	vertexTextureCoordinate = textureCoordinate;
}
);

// Fragment Shader Source Code
const GLchar* fragmentShaderSource = GLSL(440,
in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
uniform vec2 TexScale;

void main()
{
	/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

	//Calculate Ambient lighting*/
	float ambientStrength = 0.1f; // Set ambient or global lighting strength
	vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

	//Calculate Diffuse lighting*/
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse = impact * lightColor; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity = 0.8f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate * TexScale);

	// Calculate phong result + Object Color
	vec3 phong = (ambient + diffuse + specular + objectColor) * textureColor.xyz;

	fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}
);

/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);

/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,

	out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

void main()
{
	fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

void flipImageVert(unsigned char* image, int width, int height, int channels)
{
	for (int z = 0; z < height / 2; ++z)
	{
		int indexA = z * width * channels;
		int indexB = (height - 1 - z) * width * channels;

		for (int x = width * channels; x > 0; --x)
		{
			unsigned char temp = image[indexA];
			image[indexA] = image[indexB];
			image[indexB] = temp;
			++indexA;
			++indexB;
		}
	}
}

bool UCreateTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVert(image, width, height, channels);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); 

		return true;
	}
	return false;
}

void UDestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}

void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process All Input 
void UProcessInput(GLFWwindow* window)
{
	static const float cameraSpeed = 2.5f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraOffset = cameraSpeed * gDeltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCameraPos += cameraOffset * gCameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCameraPos -= cameraOffset * gCameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCameraPos -= glm::normalize(glm::cross(gCameraFront, gCameraUp)) * cameraOffset;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCameraPos += glm::normalize(glm::cross(gCameraFront, gCameraUp)) * cameraOffset;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		gCameraPos -= cameraOffset * gCameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCameraPos += cameraOffset * gCameraUp;

	// Change Views

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		viewType = false;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		viewType = true;

}

// Mouse Aim Function
void UMousePositionFunction(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; 

	gLastX = xpos;
	gLastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	gCameraFront = glm::normalize(front);
}

// Function to Render Frame
void URender(vector<GLMesh> CreateMesh)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = glm::lookAt(gCameraPos, gCameraPos + gCameraFront, gCameraUp);	
	
	// Perspective & Ortho
	glm::mat4 projection;
	if (viewType == false)
	{
		projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
	else
	{
		float scale = 60;
		projection = glm::ortho(-(800.0f / scale), 800.0f / scale, -(600.0f / scale), (600.0f / scale), -4.5f, 6.5f);
	}

	// Create Mesh
	for (auto i = 0; i < CreateMesh.size(); ++i)
	{
		auto Mesh = CreateMesh[i];

		glBindVertexArray(Mesh.vao);

		glUseProgram(gProgramId);

		GLint modelLoc = glGetUniformLocation(gProgramId, "model");
		GLint viewLoc = glGetUniformLocation(gProgramId, "view");
		GLint projLoc = glGetUniformLocation(gProgramId, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Mesh.model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
		GLint lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
		GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

		glUniform3f(objectColorLoc, Mesh.prop[0], Mesh.prop[1], Mesh.prop[2]);
		glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
		glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

		const glm::vec3 cameraPosition = gCamera.Position;
		glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		GLint UVScaleLoc = glGetUniformLocation(gProgramId, "TexScale");
		glUniform2fv(UVScaleLoc, 1, glm::value_ptr(Mesh.gTexScale));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPlastic);

		// Draw Triangles
		glDrawArrays(GL_TRIANGLES, 0, Mesh.nIndices);

	}

	// Light Setup
	glUseProgram(gLampProgramId);
	glBindVertexArray(lampLight.vao);

	glm::mat4 model = glm::translate(gLightPosition) * glm::scale(gLightScale);

	GLint modelLocation = glGetUniformLocation(gLampProgramId, "model");
	GLint viewLocation = glGetUniformLocation(gLampProgramId, "view");
	GLint projLocation = glGetUniformLocation(gLampProgramId, "projection");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, lampLight.nVertices);

	glBindVertexArray(0);
	glUseProgram(0);

	// GLFW Swap Buffers and Poll IO Events
	glfwSwapBuffers(gWindow);
}

// Create Shader Function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// Shader Program Obj
	programId = glCreateProgram();

	// Vertex / Fragment Shader Objs
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Shader Source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile Shaders
	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);

	// Attach Shaders 
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Link Shader
	glLinkProgram(programId);

	// Use Shader
	glUseProgram(programId);

	return true;
}

void UDestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}

void UDestroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}

// Initialize Function
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW Initialize & Config
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW Window Creation
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionFunction);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW Initialize & Config
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	return true;
}

// Implements the UCreateMesh function
void UCreateLightMesh(GLight& mesh)
{
	// Position and Color data
	GLfloat verts[] = {
		//Positions          //Normals
		// ------------------------------------------------------
		//Back Face          //Negative Z Normal  Texture Coords.
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   //Front Face         //Positive Z Normal
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

	  //Left Face          //Negative X Normal
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Right Face         //Positive X Normal
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Bottom Face        //Negative Y Normal
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	//Top Face           //Positive Y Normal
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

// Main Loop 
int main(int argc, char* argv[])
{
	// Initiate 
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// Create Mesh
	MeshCreator::CreateMesh(CreateMesh);

	// Load textures
	const char* texFilename = "greenplastic.png";
	if (!UCreateTexture(texFilename, gPlastic))
		{
			return EXIT_FAILURE;
		}

	if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource,
			gProgramId)) 
			return EXIT_FAILURE;
	
	if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
		return EXIT_FAILURE;

	UCreateLightMesh(lampLight);
	glUseProgram(gProgramId);

	glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

	// Background Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Loop
	while (!glfwWindowShouldClose(gWindow))
	{
		// Frame Timing
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// Read Input
		UProcessInput(gWindow);

		// Render Frame
		URender(CreateMesh);

		glfwPollEvents();
	}

	// Destroy Mesh Function
	for (auto& mesh : CreateMesh)
	{
		UDestroyMesh(mesh);
	}
	CreateMesh.clear();

	UDestroyTexture(gPlastic);

	// Destroy Shader Function
	UDestroyShaderProgram(gProgramId);
	UDestroyShaderProgram(gLampProgramId);

	exit(EXIT_SUCCESS);
}