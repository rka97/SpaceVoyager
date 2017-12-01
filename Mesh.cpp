#include "Mesh.h"
#include "GL\glew.h"
#include <iostream>
using std::cout;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material* mat)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = mat;
	this->Initialize();
}

Mesh::Mesh(const Mesh & mesh)
{
	this->vertices = mesh.vertices;
	this->indices = mesh.indices;
	this->material = mesh.material;
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
	if (attribLocation == 0)
	{
		cout << "ERROR: attribute in_position not found.\n";
		return;
	}
	glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(attribLocation);
	attribLocation = material->GetParameterLocation("in_normal");
	if (attribLocation == 0)
	{
		cout << "ERROR: attribute in_normal not found.\n";
		return;
	}
	glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(attribLocation);
	attribLocation = material->GetParameterLocation("in_texCoordinates");
	if (attribLocation == 0)
	{
		cout << "ERROR: attribute in_texCoordinates not found.\n";
		return;
	}
	glVertexAttribPointer(attribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
	glEnableVertexAttribArray(attribLocation);

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

void Mesh::Draw()
{
	if (material == NULL)
	{
		cout << "Error in Mesh::Initialize(): Material is NULL.\n";
		return;
	}
	
	material->BindTextures();

	// draw mesh
	glBindVertexArray(vertexArrayObjectID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	// always good practice to set everything back to defaults once configured.
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
