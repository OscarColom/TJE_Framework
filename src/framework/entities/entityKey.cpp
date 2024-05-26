#include "entityKey.h"
#include "game/game.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"
#include "game/stage.h"
#include "framework/entities/entityPlayer.h"

EntityKey::EntityKey(Mesh* player_mesh, const Material& player_material, const std::string& name) {
	this->mesh = player_mesh;
	this->material = player_material;
	this->name = name;
}

EntityKey::~EntityKey() {

}

void EntityKey::render(Camera* camera) {

	
	
	EntityMesh::render(camera);	

}

void EntityKey::update(float seconds_elapsed) {
	float camera_yaw = GamePlay::get_instance()->camera_yaw;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	//Axes where the char is gonna move
	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();

	//printf("%f", Game::instance->world->current_stage->player->position.length());
	
	if (with_player) {
		position = Game::instance->world->current_stage->player->model.getTranslation() + Vector3(0.5f);
	}
	else {
		position = model.getTranslation();
	}

	model.setTranslation(position);
	model.rotate(camera_yaw, Vector3(0, 1, 0));

	EntityMesh::update(seconds_elapsed);
}

