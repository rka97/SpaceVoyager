#include "BulletModel.h"
#include <iostream>
const static int NUM_OF_VERTICES = 40;

BulletModel::BulletModel(const string& name, Material* mat) : Model(name, mat)
{
	UpdateMesh();
}

void BulletModel::UpdateMesh() {
	float r1, r2;
	bool clockDirection;
	vector<Vertex> vertices1;
	vector<unsigned int> indices1;
	r2 = 1;
	r1 = 0;
	clockDirection = true;
	for (int i = 0; i < 2 * NUM_OF_VERTICES; i++) {
		if (i < NUM_OF_VERTICES) {
			Vertex u, w;
			u.position = glm::vec3(r1 * cos(i *  (2 * glm::radians(180.0f) / NUM_OF_VERTICES)),
				r1 * sin(i *  (2 * glm::radians(180.0f) / NUM_OF_VERTICES)), 0);
			w.position = glm::vec3(r2 * cos(i *  (2 * glm::radians(180.0f) / NUM_OF_VERTICES)),
				r2 * sin(i *  (2 * glm::radians(180.0f) / NUM_OF_VERTICES)), 0);
			vertices1.push_back(u);
			vertices1.push_back(w);
		}
		if (clockDirection) {
			indices1.push_back(i);
			indices1.push_back((i + 1) % (2 * NUM_OF_VERTICES));
			indices1.push_back((i + 2) % (2 * NUM_OF_VERTICES));
		}
		else {
			indices1.push_back((i + 2) % (2 * NUM_OF_VERTICES));
			indices1.push_back((i + 1) % (2 * NUM_OF_VERTICES));
			indices1.push_back(i);
		}
		clockDirection = !clockDirection;
	}
	Mesh mesh1(vertices1, indices1, material, true);
	meshes.push_back(mesh1);
}

BulletModel::~BulletModel()
{

}
