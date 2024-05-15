#include "world.h"
#include "game/game.h"
#include "framework/entities/entityMesh.h"
#include "framework/entities/entityPlayer.h"
#include "framework/entities/entity_collider.h"

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


	//Player
	Material player_material;
	player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shader/texture.fs");
	player_material.diffuse = new Texture();
	player_material.diffuse->load("data/final_character/survivorFemaleA.png");
	Mesh* player_mesh = Mesh::Get("data/final_character/character.obj");
	player = new EntityPlayer(player_mesh, player_material, "player");
	player->model.translate(0.f, 70.f, 0.f);




	//Skybox
	Material sky_cubemap;
	sky_cubemap.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
	sky_cubemap.diffuse = new Texture();
	sky_cubemap.diffuse->loadCubemap("landscape", {
		"data/CubeSky/px.png",
		"data/CubeSky/nx.png",
		"data/CubeSky/ny.png",
		"data/CubeSky/py.png",
		"data/CubeSky/pz.png",
		"data/CubeSky/nz.png"
		});
	Mesh* skybox_mesh = Mesh::Get("data/meshes/cubemap.ASE");
	skybox = new EntityMesh(skybox_mesh, sky_cubemap, "landscape");


	//Add meshes to root.
	//root.addChild(isla);
	parseScene("data/myscene.scene", &root);

	//escenarios models resources
	//Nueva clase entity_Collider
}


World::~World() {

}

void World::OnKeyDown(SDL_KeyboardEvent event) {
	switch (event.keysym.sym)
	{
	case SDLK_TAB:
		free_camera = !free_camera;
		break;
	}

}

void World::render() {

	camera->enable();

	glDisable(GL_DEPTH_TEST);
	skybox->model.setTranslation(camera->center);
	skybox->model.scale(70.f, 70.f, 70.f);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);


	//player->render_player(camera);
	player->render(camera);

	root.render(camera);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}


//
void World::update(float seconds_elapsed) {

	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	// Example

	//skybox->model.setTranslation(camera->eye);
	
	// Mouse input to rotate the cam


	//Update the player
	//player->update(seconds_elapsed);


	if (free_camera) {
		if (Input::isMousePressed(SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		// Async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 50; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {

		player->update(seconds_elapsed);

		Vector3 eye;
		Vector3 center;

		camera_yaw -= Input::mouse_delta.x * Game::instance->elapsed_time * 2.f ;
		camera_pitch -= Input::mouse_delta.y * Game::instance->elapsed_time * 2.f ;


		camera_pitch = clamp(camera_pitch, -PI * 0.3f, PI * 0.3f);



		Matrix44 myaw;
		myaw.setRotation(camera_yaw, Vector3(0.f, 1.f, 0.f));

		Matrix44 mpitch;
		mpitch.setRotation(camera_pitch, Vector3(-1.f, 0.f, 0.f));


		Vector3 front = (mpitch * myaw).frontVector().normalize();

		float orbit_dist = 4.f;
		//eye = (player->model.getTranslation() - front * orbit_dist) + Vector3(0.f , 25.5f , -15.f );
		//center = player->getGlobalMatrix() * Vector3(0.f , 25.5f , -15.f );;
		Vector3 corrector = Vector3(0.f, 4.1f, 0.f);
		eye = (player->model.getTranslation() - front * orbit_dist) + corrector;
		center = player->model.getTranslation()  + Vector3(0.f, 4.1f, 0.f); //The last vector is bc we cound be pointing at the char feet otherwise
		 //tirar rayos desde jugador a la camara


		Vector3 dir = eye - center;
		//Vector3 dir = center - eye;
		//sCollisionData data = World::get_instance()->ray_cast(center, dir.normalize(), ALL, dir.length());
		sCollisionData data = World::get_instance()->ray_cast(center, dir.normalize(), eCollisionFilter::SCENARIO, dir.length());

		if (data.collided) {
			eye = data.col_point;
		}


		camera->lookAt(eye, center, Vector3(0, 1, 0));

		//Game::instance->mouse_locked = true;


	}

	//skybox->model.setTranslation(camera->getRayDirection(Input::mouse_delta.x, Input::mouse_delta.y, window_width, window_height));

	root.update(seconds_elapsed);
	skybox->update(seconds_elapsed);
}


sCollisionData  World::ray_cast(const Vector3& origin, const Vector3& direction, int layer, float max_ray_dist) {

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

		data.collided = true;

		if (col_point.y > 0) {
			data.collided = true;
			data.col_point = col_point;
			data.col_normal = col_normal;
			data.distance = new_distance;
			data.collider = ec;
		}

		//float new_distance = (col_point - origin).length();
		//if (new_distance < data.distance) {
		//	data.collided = true;
		//	data.col_point = col_point;
		//	data.col_normal = col_normal;
		//	data.distance = new_distance;
		//	//data.collider = ec;
		//}


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

		size_t tag = data.first.find("@tag");

		if (tag != std::string::npos) {
			Mesh* mesh = Mesh::Get("...");
			// Create a different type of entity
			// new_entity = new ...
		}
		else {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			//mat.diffuse = new Texture();
			//mat.diffuse->load("data/textures/fat_tree.png");
			// 
			//new_entity = new EntityMesh(mesh, mat);
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

