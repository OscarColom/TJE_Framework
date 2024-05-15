#pragma once
#include "framework/entities/entityMesh.h"


class EntityPlayer : public EntityMesh {
public:

	float walk_speed = 8.f;
	Vector3 velocity = Vector3(0.0f);
	Vector3 position;

	EntityPlayer() {};
	EntityPlayer(Mesh* player_mesh, const Material& player_material, const std::string& name) ;
	~EntityPlayer();

	void update(float seconds_elapsed);
	void render(Camera* camera);

};
