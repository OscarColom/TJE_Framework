#include "entity_collider.h"
#include "game/world.h"
#include "entityPlayer.h"

void EntityCollider::getCollisionsWithModel(const Matrix44& m, const Vector3& target_position , std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {

	Vector3 collision_point;
	Vector3 collision_normal;

	float sphere_radius = World::get_instance()->sphere_radius;
	float sphere_ground_radius = World::get_instance()->sphere_grow;
	float player_height = World::get_instance()->player_height;

	Vector3 center_char = World::get_instance()->player->position;
	Vector3 center = target_position;

	//Check for collisions
	Vector3 floor_sphere_center = center + Vector3(0.f, sphere_ground_radius, 0.f);


	if (mesh->testSphereCollision(m, floor_sphere_center + 0.1f, sphere_ground_radius - 0.29999f, collision_point, collision_normal)) {
		collisions.push_back({ collision_point, collision_normal.normalize(), floor_sphere_center.distance(collision_point) });
	}

	//Check wall collisions
	Vector3 character_center_target = center + Vector3(0.f, player_height, 0.f);  //tendria que ir sphere_radius
	if (mesh->testSphereCollision(m, character_center_target, sphere_radius - 0.9f, collision_point, collision_normal)) {
		collisions.push_back({ collision_point, collision_normal.normalize(), character_center_target.distance(collision_point) });
	}

	Vector3 character_center = center_char + Vector3(0.f, player_height, 0.f);
	//Check if grounded... SI EL CHAR_CENTER (DESDE DNDE SE TIRA EL RAYO MUY GRANDE SUBE DIRECTAMENTE A OBJETOS TIPO MUROS ESCALERAS)
	if (mesh->testRayCollision(m, character_center_target, Vector3(0, -1, 0), collision_point, collision_normal, player_height + 0.5f )) {
		ground_collisions.push_back({ collision_point, collision_normal.normalize(), character_center_target.distance(collision_point) });
	}

}

void EntityCollider::getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions, eCollisionFilter filter ) {

	if (!layer && filter) {
		return;
	}

	if (!isInstanced) {
		getCollisionsWithModel(model, target_position, collisions, ground_collisions);
	}
	else
	{
		for (int i = 0; i < models.size(); ++i)
		{
			getCollisionsWithModel(models[i], target_position, collisions, ground_collisions);
		}
	}
}