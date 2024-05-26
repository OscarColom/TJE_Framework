#pragma once
#include "framework/entities/entityMesh.h"

class EntityKey : public EntityMesh {
public:

	Vector3 position;
	bool with_player = false;

	EntityKey() {};
	EntityKey(Mesh* player_mesh, const Material& player_material, const std::string& name);
	~EntityKey();

	void update(float seconds_elapsed);
	void render(Camera* camera);
};
