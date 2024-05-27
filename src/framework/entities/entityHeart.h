#pragma once
#include "framework/entities/entityMesh.h"

class EntityHeart : public EntityMesh {
public:

	Vector3 position;
	bool life_added = false;

	EntityHeart() {};
	EntityHeart(Mesh* player_mesh, const Material& player_material, const std::string& name);
	~EntityHeart();

	void update(float seconds_elapsed);
	void render(Camera* camera);
};
