#pragma once

#include "framework/entities/entity_collider.h"


class EntityFlag : public EntityCollider {
public:

	Vector3 position;
	bool open = false;

	EntityFlag() {};
	EntityFlag(Mesh* player_mesh, const Material& player_material, const std::string& name);
	~EntityFlag();

	void update(float seconds_elapsed);
	void render(Camera* camera);
};
