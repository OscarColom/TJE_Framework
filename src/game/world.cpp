#include "world.h"
#include "game/game.h"
#include "framework/entities/entityMesh.h"
#include "framework/entities/entityPlayer.h"
#include "framework/entities/entity_collider.h"
#include "framework/entities/entityHeart.h"
#include "framework/entities/entityKey.h"
#include "framework/entities/entity_Gate.h"
#include "framework/entities/entity_Flag.h"

#include "stage.h"

#include "framework/input.h"

#include <iostream>
#include <fstream>


World* World::instance = nullptr; // Inicialización de la variable estática


World::World() {

	window_width = Game::instance->window_width;
	window_height = Game::instance->window_height;
	instance = this;

	camera = new Camera();
	camera->lookAt(Vector3(0.f, 1.f, 1.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f);

	//craete 2d camera
	camera2D = new Camera();
	camera2D->setOrthographic(0, window_width, window_height, 0, -1.f, 1.f);

	menu_stage = new Menu();
	game_stage = new GamePlay();
	death_stage = new Death();
	final_stage = new Final();


	menu_stage->init();
	game_stage->init();
	death_stage->init();
	final_stage->init();

	current_stage = menu_stage;

	start_time_recorded = false;
	end_time_recorded = false;

	parseScene("data/myscene.scene", &root);
}


World::~World() {

}

void World::OnKeyDown(SDL_KeyboardEvent event) {
	switch (event.keysym.sym)
	{
	case SDLK_TAB:
		free_camera = !free_camera;
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
		break;
	}

}

void World::render() {
	current_stage->render();
}


void World::update(float seconds_elapsed) {

	current_stage->update(seconds_elapsed);

	if (current_stage != game_stage) {
		mouse_locked = false; 
	}
	else {
		mouse_locked = true; 
	}

	SDL_ShowCursor(mouse_locked ? SDL_DISABLE : SDL_ENABLE);
	SDL_SetRelativeMouseMode(mouse_locked ? SDL_TRUE : SDL_FALSE);

	if (current_stage == game_stage && !start_time_recorded) {
		start_game_time = SDL_GetTicks();
		start_time_recorded = true; // Marcar que el tiempo de inicio ha sido registrado
		end_time_recorded = false;  // Reiniciar el marcador de tiempo de fin
	}

	if (current_stage == final_stage && !end_time_recorded) {
		end_game_time = SDL_GetTicks();
		elapsed_time_game = (end_game_time - start_game_time) / 1000.0f; // Calcular el tiempo transcurrido en segundos
		end_time_recorded = true; // Marcar que el tiempo de fin ha sido registrado

	}
}


sCollisionData World::ray_cast(const Vector3& origin, const Vector3& direction, int layer, float max_ray_dist) {

	sCollisionData data;
	data.collided = false;
	for (auto e : root.children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec == nullptr ) {
			continue;
		}

		Vector3 col_point;
		Vector3 col_normal; 

		if (ec->isInstanced) {

			for (int i = 0; i < ec->models.size(); ++i) {

				if (!ec->mesh->testRayCollision(ec->models[i], origin, direction, col_point, col_normal, max_ray_dist)) {
					continue;
				}
			}
		}

		else
		{
			if (!ec->mesh->testRayCollision(ec->model, origin, direction, col_point, col_normal, max_ray_dist)) {
				continue;
			}
		}


		if (col_point.y > 0) {
			data.collided = true;
			data.col_point = col_point;
		}

	}

	return data;
}

bool World::parseScene(const char* filename, Entity* root)
{
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}


	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		if (mesh_name[0] == '#')
			continue;

		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, nothing to do here
		if (render_data.models.empty())
			continue;


		Material mat = render_data.material;
		EntityMesh* new_entity = nullptr;

		size_t gate_tag = data.first.find("@gate");
		size_t heart_tag = data.first.find("@heart");
		size_t key_tag = data.first.find("@key");
		size_t flag_tag = data.first.find("@flag");

		size_t ground_tag = data.first.find("@ground");
		
		if (gate_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/scene/wall-gate@gate/wall-gate@gate.obj");
			new_entity = new EntityGate(mesh, mat, "gate");
		}
		else if (heart_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/scene/heart@heart/heart@heart.obj");
			new_entity = new EntityHeart(mesh, mat, "heart");
		}
		else if (key_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/scene/key@key/key@key.obj");
			new_entity = new EntityKey(mesh, mat, "key");
		}
		else if (flag_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/scene/flag-pennant@flag/flag-pennant@flag.obj");
			new_entity = new EntityFlag(mesh, mat, "flag");
		}



		else if (ground_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("data/scene/ground.002@ground/ground.002@ground.obj");
			new_entity = new EntityMesh(mesh, mat, "ground");
		}
		else{

			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			new_entity = new EntityCollider(mesh, mat);
		}

		if (!new_entity) {
			continue;
		}

		new_entity->name = data.first;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			new_entity->isInstanced = true;
			new_entity->models = render_data.models; // Add all instances
		}
		// Create normal entity
		else {
			new_entity->model = render_data.models[0];
		}

		// Add entity to scene root
		root->addChild(new_entity);
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}

