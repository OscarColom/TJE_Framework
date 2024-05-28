#pragma once

#include "framework/entities/entity_collider.h"


class EntityGate : public EntityCollider {
public:

	Vector3 position;
	bool open = false;

	EntityGate() {};
	EntityGate(Mesh* player_mesh, const Material& player_material, const std::string& name);
	~EntityGate();

	void update(float seconds_elapsed);
	void render(Camera* camera);
};
