#pragma once
#include <string>
#include <vector>
#include "Shader.h"
#include "Material.h"
#include "glm\glm.hpp"
using namespace std;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	GLuint vertexArrayObjectID;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
	void Draw(Shader& vertexShader);	
};