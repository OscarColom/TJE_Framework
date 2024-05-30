#include "world.h"
#include "game/game.h"
#include "framework/entities/entityMesh.h"
#include "framework/entities/entityPlayer.h"
#include "framework/entities/entity_collider.h"
#include "framework/entities/entityHeart.h"
#include "framework/entities/entityKey.h"
#include "framework/entities/entity_Gate.h"
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
	camera2D->view_matrix.setIdentity();
	camera2D->setOrthographic(0, window_width, window_height, 0, -1.f, 1.f);
	

	menu_stage = new Menu();
	game_stage = new GamePlay();


	menu_stage->init();
	game_stage->init();

	current_stage = game_stage;




	//Player
	//Material player_material;
	//player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//player_material.diffuse = new Texture();
	//player_material.diffuse->load("data/final_character/survivorFemaleA.png");
	//Mesh* player_mesh = Mesh::Get("data/final_character/animations/character.MESH");
	//
	//player = new EntityPlayer(player_mesh, player_material, "player");
	//player->model.translate(0.f, 5.f, 0.f);
	//player->isAnimated = true;

	////Skybox
	//Material sky_cubemap;
	//sky_cubemap.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
	//sky_cubemap.diffuse = new Texture();
	//sky_cubemap.diffuse->loadCubemap("landscape", {
	//	"data/CubeSky/px.png",
	//	"data/CubeSky/nx.png",
	//	"data/CubeSky/ny.png",
	//	"data/CubeSky/py.png",
	//	"data/CubeSky/pz.png",
	//	"data/CubeSky/nz.png"
	//	});
	//Mesh* skybox_mesh = Mesh::Get("data/meshes/cubemap.ASE");
	//skybox = new EntityMesh(skybox_mesh, sky_cubemap, "landscape");
	//skybox->model.scale(70.f, 70.f, 70.f);

	parseScene("data/myscene.scene", &root);

	//cargar l agate
	//parseScene("data/myGate.scene", &Gate);


	// Nombre de la entidad que deseas encontrar
	//	//Crear entity_ui
	//herda de entitymesh
	//2 constuctores
	//, pos , size , material butonide con un enum, 
	//En world cerar camara 2d ortografica
	//como renderizar: en el reder de ui: hacer los set_uniform, no usar depth_buffer,,n no usar cull_face word, i viewproj = camera2s->view:projectionmatrix.
	//al acabar rende hacer , y volver a dejar gl como antes entity::render(camersa2s).
	////
	//audio hacer chanel1  = Audio::Play().. si llop añadir flag de loop
	//parq quitar audio usar el chanel

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


//
void World::update(float seconds_elapsed) {
	current_stage->update(seconds_elapsed);
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

