#include "Model.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

Model::Model(const string& name, const string & path, Material* mat, bool gamma)
{
	this->name = name;
	this->path = path;
	this->material = mat;
	this->gammaCorrection = gamma;
	LoadModel();
}

void Model::Draw()
{
	for (auto& mesh : meshes)
		mesh.Draw();
}

void Model::Initialize()
{

}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(this->path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
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
		tempVec.x = mesh->mNormals[i].x;
		tempVec.y = mesh->mNormals[i].y;
		tempVec.z = mesh->mNormals[i].z;
		vertex.normal = tempVec;
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
	
	Material* childMaterial = new Material(*material);

	childMaterial->SetTextures(textures);

	return Mesh(vertices, indices, childMaterial);
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

