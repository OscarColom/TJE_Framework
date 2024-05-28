#include "stage.h"
#include "framework/input.h"
#include "framework/entities/entityPlayer.h"
#include "framework/entities/entityKey.h"
#include "framework/entities/entity_Gate.h"



//STAGE BASE CLASS
GamePlay* GamePlay::instance = nullptr;

Stage::Stage() {

	window_width = World::get_instance()->window_width;
	window_height = World::get_instance()->window_height;

	camera = camera = World::get_instance()->camera;

}

Stage::~Stage() {

}




//MENU STAGE
void Menu::init() {
	Game::instance->mouse_locked = false;
	isFinished = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

}

void Menu::restart() {

}

void Menu::render() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawText(world_width/2, world_height /2, "Clica para entarr al juego", Vector3(1, 1, 1), 2);
	drawText(world_width / 2, (world_height / 2  )+20, "Apreta G para cojer la llave cuando estes cerca de ella", Vector3(1, 1, 1), 2);

	//SDL_GL_SwapWindow(this->window);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Menu::update(float seconds_elapsed) {
	if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
		World* world = World::get_instance();
		world->current_stage = world->game_stage;
	}
}


//GAME STAGE
void GamePlay::init() {

	window_width = World::get_instance()->window_width;
	window_height = World::get_instance()->window_height;

	instance = this;

	//Key Level 1
	Material key_material;
	key_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	key_material.diffuse = new Texture();
	key_material.diffuse->load("data/key/Key_Material.png");
	Mesh* key_mesh = Mesh::Get("data/key/key.obj");
	
	key = new EntityKey(key_mesh, key_material, "key"); 
	key->model.translate(6.55f, 55.f, -94.64f);
	
	//Gate Level 1
	Material gate_material;
	gate_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	gate_material.diffuse = new Texture();
	gate_material.diffuse->load("data/wall-gate/colormap.png");
	Mesh* gate_mesh = Mesh::Get("data/wall-gate/wall-gate.obj");

	//gate = new EntityGate(gate_mesh, gate_material, "gate"); 
	//gate->model.translate(0.0f, 15.85f, -189.79f);

	//Heart Level 1
	Material heart_material;
	heart_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	heart_material.diffuse = new Texture();
	heart_material.diffuse->load("data/heart_2/red_texture.png");
	Mesh* heart_mesh = Mesh::Get("data/heart_2/heart.obj");

	heart = new EntityHeart(heart_mesh, heart_material, "heart");
	heart->model.translate(5.6f, 60.f, -162.27f);
	
	//Player
	Material player_material;
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	player_material.diffuse = new Texture();
	player_material.diffuse->load("data/final_character/survivorFemaleA.png");
	Mesh* player_mesh = Mesh::Get("data/final_character/animations/character.MESH");

	player = new EntityPlayer(player_mesh, player_material, "player");
	player->model.translate(0.f, 5.f, 0.f);
	player->isAnimated = true;

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
	skybox->model.scale(70.f, 70.f, 70.f);

	World::get_instance()->root.addChild(heart);
	World::get_instance()->root.addChild(key);
	//World::get_instance()->root.addChild(gate);
}

void GamePlay::restart() {

}

void GamePlay::render() {
	camera->enable();

	glDisable(GL_DEPTH_TEST);
	skybox->model.setTranslation(camera->center);
	skybox->model.scale(70.f, 70.f, 70.f);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);

	//Transformations
	for (auto e : World::get_instance()->root.children) {
		EntityGate* gate = dynamic_cast<EntityGate*>(e);
		if (gate != nullptr) {
			gate->model.rotate( -(PI / 2), Vector3(1, 0, 0));
			gate->model.scale(25.f, 25.f, 25.f);
		}
	}

	//gate->model.rotate(PI, Vector3(0, 1, 0));
	//gate->model.rotate(PI/2, Vector3(0, 0, 1));
	//gate->model.scale(25.f, 25.f, 25.f);
	key->model.scale(4.f, 4.f, 4.f);
	heart->model.scale(0.5f, 0.5f, 0.5f);

	World::get_instance()->root.render(camera);
	player->render(camera);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

}

void GamePlay::update(float seconds_elapsed) {
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	free_camera = World::get_instance()->free_camera;
	mouse_locked = World::get_instance()->mouse_locked;
	if (free_camera) {
		if (Input::isMousePressed(SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		// Async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 20; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {

		player->update(seconds_elapsed);

		Vector3 eye;
		Vector3 center;

		SDL_ShowCursor(!mouse_locked);
		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));

		camera_yaw -= Input::mouse_delta.x * Game::instance->elapsed_time * 2.f;
		camera_pitch -= Input::mouse_delta.y * Game::instance->elapsed_time * 2.f;

		camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

		Matrix44 myaw;
		myaw.setRotation(camera_yaw, Vector3(0, 1, 0));

		Matrix44 mpitch;
		mpitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

		Vector3 front = (mpitch * myaw).frontVector().normalize();

		float orbit_dist = 6.f;
		Vector3 corrector = Vector3(0.f, 4.1f, 0.f);
		eye = (player->model.getTranslation() - front * orbit_dist) + corrector;
		center = player->model.getTranslation() + Vector3(0.f, 4.1f, 0.f); //The last vector is bc we cound be pointing at the char feet otherwise

		Vector3 dir = eye - center;
		sCollisionData data = World::get_instance()->ray_cast(center, dir.normalize(), eCollisionFilter::ALL, dir.length());///////////////PELIOGROO

		if (data.collided) {
			eye = data.col_point;
		}

		//Key grab
		Vector3 key_distance = player->position.distance(key->position);
		//printf("%f \n", key_distance.length());
		if (key_distance.length() < 7.f && Input::isKeyPressed(SDL_SCANCODE_G)) {
			key->with_player = true;
		}
		
		//Por ahora solo gate
		for (auto e : World::get_instance()->root.children) {
			EntityGate* gate = dynamic_cast<EntityGate*>(e);
			if (gate != nullptr) {
				printf("%f \n", gate->distance(player));
				if (key->with_player && gate->distance(player) < 40.f) {
					World::get_instance()->root.removeChild(gate);
					World::get_instance()->root.removeChild(key);
				}
			}
		}

		//printf("%f \n", heart->distance(player));
		if (heart->distance(player) < 5.f && !heart->life_added) {
			heart->~EntityHeart();
			player->lifes = player->lifes + 1;
			heart->life_added = true;
		}

		//Cheat mode: k vagi a la pos de la clau
		if (Input::isKeyPressed(SDL_SCANCODE_T)) {
			player->model.setTranslation(key->model.getTranslation());
		}

		camera->lookAt(eye, center, Vector3(0, 1, 0));
	}

	World::get_instance()->root.update(seconds_elapsed);
	skybox->update(seconds_elapsed);
}



void Death::init() {
	
}

void Death::restart() {

}

void Death::render() {

}

void Death::update(float seconds_elapsed) {

}
