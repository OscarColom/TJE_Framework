#include "entity_Flag.h"
#include "game/game.h"
#include "framework/input.h"
#include "framework/entities/entityPlayer.h"

EntityFlag::EntityFlag(Mesh* player_mesh, const Material& player_material, const std::string& name) {
	this->mesh = player_mesh;
	this->material = player_material;
	this->name = name;
}

EntityFlag::~EntityFlag() {

}

void EntityFlag::render(Camera* camera) {

	
	
	EntityMesh::render(camera);	

}

void EntityFlag::update(float seconds_elapsed) {
	//float camera_yaw = GamePlay::get_instance()->camera_yaw;

	//Matrix44 mYaw;
	//mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	//Axes where the char is gonna move
	//Vector3 front = mYaw.frontVector();
	//Vector3 right = mYaw.rightVector();

	//printf("%f", Game::instance->world->current_stage->player->position.length());
	
	if (open) {
		position = model.getTranslation() + Vector3(0.f, 15.f, 0.f);
	}
	else {
		position = model.getTranslation();
	}

	model.setTranslation(position);
	EntityMesh::update(seconds_elapsed);
}

