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
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec4 vertexColor;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material* mat, bool isInstanced = false);
	Mesh(const Mesh& mesh);
	void Initialize();
	void InitializeInstanced(void* data, int maxNumInstances);
	virtual void Draw(int numInstances = 1);
private:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int vertexArrayObjectID;
	unsigned int vertexBufferObjectID;
	unsigned int elementBufferObjectID;
	unsigned int instanceBufferObjectID;
	Material* material;
	bool isInstanced;
};