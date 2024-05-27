#include "entityHeart.h"
#include "game/game.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"
#include "game/stage.h"
#include "framework/entities/entityPlayer.h"

EntityHeart::EntityHeart(Mesh* player_mesh, const Material& player_material, const std::string& name) {
	this->mesh = player_mesh;
	this->material = player_material;
	this->name = name;
}

EntityHeart::~EntityHeart() {

}

void EntityHeart::render(Camera* camera) {

	EntityMesh::render(camera);

}

void EntityHeart::update(float seconds_elapsed) {
	float camera_yaw = GamePlay::get_instance()->camera_yaw;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	//Axes where the char is gonna move
	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();
	
	position = model.getTranslation();
	model.setTranslation(position);

	EntityMesh::update(seconds_elapsed);
}

