#include "Model.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>
#include <utility>
using namespace std;

Model::Model(const string& name, const string & path, Material* mat, bool isInstanced, bool gamma)
{
	this->name = name;
	this->path = path;
	this->material = mat;
	this->gammaCorrection = gamma;
	this->isInstanced = isInstanced;
}

Model::Model(const string& name, Material* mat) {
	this->name = name;
	this->material = mat;
}

Model::~Model()
{
	// Loops over all the children materials and kills them.
	for (vector<Material*>::iterator it = childMaterials.begin(); it != childMaterials.end(); it++)
	{
		if ((*it) != nullptr)
		{
			delete (*it);
		}
	}
}

void Model::Draw(int numInstances)
{
	for (auto& mesh : meshes)
		mesh.Draw(numInstances);
}

void Model::Initialize()
{
	LoadModel();
}

bool Model::SetParameterValue(int id, void * parameterValue)
{
	for (vector<Material*>::iterator it = childMaterials.begin(); it != childMaterials.end(); it++)
	{
		(*it)->SetParameterValue(id, parameterValue);
	}
	return material->SetParameterValue(id, parameterValue);
}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(this->path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "Error in Model::LoadModel(): Assimp:: " << importer.GetErrorString() << "\n";
		return;
	}
	// retrieve the directory path of the file path
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}


Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	

	/* Get vertices data. */
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 tempVec;
		/* Get vertex position. */
		tempVec.x = mesh->mVertices[i].x;
		tempVec.y = mesh->mVertices[i].y;
		tempVec.z = mesh->mVertices[i].z;
		vertex.position = tempVec;
		
		/* Get vertex normals. */
		if (mesh->mNormals != NULL)
		{
			tempVec.x = mesh->mNormals[i].x;
			tempVec.y = mesh->mNormals[i].y;
			tempVec.z = mesh->mNormals[i].z;
			vertex.normal = tempVec;
		}
		/* We assume there is only ONE texture coordinate per vertex. */
		if (mesh->mTextureCoords[0] != NULL)
		{
			glm::vec2 tempVec2;
			tempVec2.x = mesh->mTextureCoords[0][i].x;
			tempVec2.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoordinates = tempVec2;
		}
		else
		{
			vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
		}
		if (mesh->mTangents != NULL)
		{
			/* Tangents. */
			tempVec.x = mesh->mTangents[i].x;
			tempVec.y = mesh->mTangents[i].y;
			tempVec.z = mesh->mTangents[i].z;
			vertex.tangent = tempVec;
		}
		if (mesh->mBitangents != NULL)
		{
			/* Bitangents. */
			tempVec.x = mesh->mBitangents[i].x;
			tempVec.y = mesh->mBitangents[i].y;
			tempVec.z = mesh->mBitangents[i].z;
			vertex.bitangent = tempVec;

		}
		vertices.push_back(vertex);
	}
	cout << "Loading Model [" << this->name << "]: number of vertices = " << vertices.size() << "\n";
	/* Indices. */
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	/* Process Materials. */
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN
	vector<Texture> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	vector<Texture> normalMaps = LoadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	vector<Texture> heightMaps = LoadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	
	// there is a memory leak here-- be careful!!

	Material* childMaterial = new Material(*material);
	//childMaterial->Initialize();
	childMaterial->SetTextures(textures);

	childMaterials.push_back(childMaterial);

	return Mesh(vertices, indices, childMaterial, isInstanced);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		const string texturePath = str.C_Str();
		if (loadedTextures.find(texturePath) != loadedTextures.end())
		{
			textures.push_back(loadedTextures[texturePath]);
		}
		else
		{
			Texture tex;
			tex.id = TextureFromFile(str.C_Str(), this->directory);
			tex.type = typeName;
			tex.path = str.C_Str();
			textures.push_back(tex);
			loadedTextures[texturePath] = tex;
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}


struct HalfEdge
{
	unsigned int vertex; // vertex index at the end of this half edge.
	HalfEdge* twin; // oppositely oriented adjacent half-edge.
	HalfEdge* next; // next half-edge around the face.
};
void Model::FindAdjacencies(aiMesh * mesh, vector<unsigned int>& indices)
{
	int numEdges = mesh->mNumFaces * 3;
	HalfEdge* halfEdges = new HalfEdge[numEdges];
	map< pair<unsigned int, unsigned int>, HalfEdge* > edgeDirectionMap;

	// Create all the appropriate half-Edges for the faces.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		int currEdgeIndex = i * 3;
		pair<unsigned int, unsigned int> edgePair;
		cout << "Face [" << i << "]: ";

		// half-edge from vertexA to vertexB
		halfEdges[currEdgeIndex].vertex = face.mIndices[0];
		halfEdges[currEdgeIndex].next = &halfEdges[currEdgeIndex + 1];
		edgePair = { face.mIndices[0], face.mIndices[1] };
		cout << "(" << edgePair.first << " , " << edgePair.second << "), \t";
		edgeDirectionMap[edgePair] = &halfEdges[currEdgeIndex];

		// half-edge from vertexB to vertexC
		halfEdges[currEdgeIndex + 1].vertex = face.mIndices[1];
		halfEdges[currEdgeIndex + 1].next = &halfEdges[currEdgeIndex + 2];
		edgePair = { face.mIndices[1], face.mIndices[2] };
		cout << "(" << edgePair.first << " , " << edgePair.second << "), \t";
		edgeDirectionMap[edgePair] = &halfEdges[currEdgeIndex + 1];

		// half-edge from vertexC to vertexA
		halfEdges[currEdgeIndex + 2].vertex = face.mIndices[2];
		halfEdges[currEdgeIndex + 2].next = &halfEdges[currEdgeIndex];
		edgePair = { face.mIndices[2], face.mIndices[0] };
		cout << "(" << edgePair.first << " , " << edgePair.second << "). \n";
		edgeDirectionMap[edgePair] = &halfEdges[currEdgeIndex + 2];
	}

	int boundaryCount = 0;
	// Populate the twin pointers.
	for (map< pair<unsigned int, unsigned int>, HalfEdge* >::iterator it = edgeDirectionMap.begin(); it != edgeDirectionMap.end(); it++)
	{
		pair<unsigned int, unsigned int> edgePair = it->first;
		unsigned int temp = edgePair.first;
		edgePair.first = edgePair.second;
		edgePair.second = temp;
		if (edgeDirectionMap.find(edgePair) != edgeDirectionMap.end())
		{
			it->second->twin = edgeDirectionMap[edgePair];
		}
		else
		{
			boundaryCount++;
		}
	}

	/* Now create the adjacent indices. */
	vector<unsigned int> adjacentTriIndices;
	if (boundaryCount > 0)
	{
		cout << "Mesh in Model " << this->name << " is not watertight, it contains " << boundaryCount << " boundary edges. \n";
		for (int i = 0; i < numEdges; i++)
		{
			adjacentTriIndices.push_back(halfEdges[i].vertex);
			adjacentTriIndices.push_back((halfEdges[i].twin != NULL) ? halfEdges[i].twin->vertex : halfEdges[i].vertex);
		}
	}
	else
	{
		for (int i = 0; i < numEdges; i++)
		{
			adjacentTriIndices.push_back(halfEdges[i].vertex);
			adjacentTriIndices.push_back(halfEdges[i].twin->vertex);
		}
	}

	indices.insert(indices.end(), adjacentTriIndices.begin(), adjacentTriIndices.end());
	delete[] halfEdges;
}

int Model::GetParameterId(string parameterName) {
	return material->GetParameterID(parameterName);
}

Material * Model::GetMaterial()
{
	return material;
}

void Model::InitializeInstanced(void * data, int numInstances)
{
	for (auto& mesh : meshes)
	{
		mesh.InitializeInstanced(data, numInstances);
	}
}
