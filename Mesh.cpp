#include "Mesh.h"
#include "GL\glew.h"
#include <iostream>
using std::cout;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material* mat, bool isInstanced)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = mat;
	this->isInstanced = isInstanced;
	this->Initialize();
}

Mesh::Mesh(const Mesh & mesh)
{
	this->vertices = mesh.vertices;
	this->indices = mesh.indices;
	this->material = mesh.material;
	this->Initialize();
}

void Mesh::Initialize()
{
	if (material == NULL)
	{
		cout << "Error in Mesh::Initialize(): Material is NULL.\n";
		return;
	}
	glGenVertexArrays(1, &vertexArrayObjectID);
	glGenBuffers(1, &vertexBufferObjectID);
	glGenBuffers(1, &elementBufferObjectID);

	glBindVertexArray(vertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	GLint attribLocation = 0;
	attribLocation = material->GetParameterLocation("in_position");
	if (attribLocation != -1) {
		glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(attribLocation);
	}
	attribLocation = material->GetParameterLocation("in_normal");
	if (attribLocation != -1) {
		glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(attribLocation);
	}
	attribLocation = material->GetParameterLocation("in_texCoordinates");
	if (attribLocation != -1)
	{
		glVertexAttribPointer(attribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
		glEnableVertexAttribArray(attribLocation);
	}
	attribLocation = material->GetParameterLocation("in_vertexColor");
	if (attribLocation != -1)
	{
		glVertexAttribPointer(attribLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertexColor));
		glEnableVertexAttribArray(attribLocation);
	}

	if (isInstanced)
	{
		glGenBuffers(1, &instanceBufferObjectID);
	}

	glBindVertexArray(0);
	/* Enable these later. */
	/*
	attribLocation = material->GetParameterLocation("in_tangent");
	glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(attribLocation);

	attribLocation = material->GetParameterLocation("in_bitangent");
	glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(attribLocation);
	*/
}

void Mesh::InitializeInstanced(void* data, int maxNumInstances)
{
	if (isInstanced)
	{
		glBindVertexArray(vertexArrayObjectID);
		material->ActivateMaterial();
		glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObjectID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxNumInstances, &((glm::vec3*)data)[0], GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		int attributeLocation = 1;
		glEnableVertexAttribArray(attributeLocation);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBufferObjectID);
		glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(attributeLocation, 1);
		glBindVertexArray(0);
	}
}

void Mesh::Draw(int numInstances)
{
	if (material == NULL)
	{
		cout << "Error in Mesh::Initialize(): Material is NULL.\n";
		return;
	}
	
	material->ActivateMaterial();

	// draw mesh
	glBindVertexArray(vertexArrayObjectID);
	if (isInstanced)
	{
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, numInstances);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	
	// always good practice to set everything back to defaults once configured.
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

array<glm::vec3, 2> Mesh::GetEnclosingBox()
{
	float minX = 9999, minY = 9999, minZ = 9999, maxX = -9999, maxY = -9999, maxZ = -9999;
	for (auto& vertex : vertices) {
		if (vertex.position.x < minX) minX = vertex.position.x;
		if (vertex.position.x > maxX) maxX = vertex.position.x;
		if (vertex.position.y < minY) minY = vertex.position.y;
		if (vertex.position.y > maxY) maxY = vertex.position.y;
		if (vertex.position.z < minZ) minZ = vertex.position.z;
		if (vertex.position.z > maxZ) maxZ = vertex.position.z;
	}
	array<glm::vec3, 2> arr = { glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ) };
	return arr;
}
