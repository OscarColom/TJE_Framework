#pragma once

#include "entity.h"
#include "entityMesh.h"

class EntityCollider : public EntityMesh {
	void getCollisionsWithModel(const Matrix44& m, const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions);
	int layer = eCollisionFilter::ALL;

public:
	bool is_static = true;

	EntityCollider() {};

	EntityCollider(Mesh* mesh, const Material& material, const std::string& name) : 
		EntityMesh(mesh, material, name) {}

	EntityCollider(Mesh* mesh, const Material& material):
		EntityMesh(mesh, material)  {};

	~EntityCollider() {};

	void getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter);

	int getLayer() { return layer; }
	void setLayer(int new_layer) { layer = new_layer; }
};