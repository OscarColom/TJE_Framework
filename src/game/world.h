#pragma once
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/entities/entity.h"
#include "framework/entities/entityKey.h"
#include "framework/entities/entityHeart.h"
#include "graphics/material.h"

class Camera;
class EntityPlayer;
class EntityMesh;
class Stage;

class World {

	static World* instance;

public:
	
	Stage* current_stage = nullptr;
	std::map<int, Stage*> stages;
	Stage* menu_stage = nullptr;
	Stage* game_stage = nullptr;


	static World* get_instance() {
		if (instance != nullptr) return instance;
		instance = new World();
		return instance;
	}

	World(); 			// Constructor
	virtual ~World(); 	// Destructor

	Entity root; 

	//Entity Gate;

	EntityPlayer* player = nullptr;

	EntityMesh* skybox = nullptr;

	int window_width;
	int window_height;

	Camera* camera = nullptr;
	Camera* camera2D = nullptr;

	float angle = 0;
	bool mouse_locked = true; //tells if the mouse is locked (not seen)

	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.0f;
	float mouse_speed = 10.f;

	bool free_camera = false;

	float  sphere_radius = 1.0f;
	float sphere_grow = .3f;
	float player_height = 3.f;

	Vector3 current_checkpoint = Vector3(0.0f, 5.0f, 0.0f);

	void render();
	void update(float seconds_elapsed);

	//Scene
	std::map<std::string, sRenderData> meshes_to_load;
	std::vector<Entity*> entities_to_destroy;

	sCollisionData  ray_cast(const Vector3& origin, const Vector3& direction, int layer, float max_ray_dist);
	bool parseScene(const char* filename, Entity* root);
	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);
	void OnKeyDown(SDL_KeyboardEvent event);
};