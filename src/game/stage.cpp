#pragma once
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

void Stage::onButtonPressed(eButtonId buttonid) {
	World* world = World::get_instance();

	switch (buttonid) {

	case PlayButton:
		world->current_stage = world->game_stage;
		break;

	case EndButton:
		exit(0);
		break;
	case PlayAgainButton:
		world->current_stage = world->game_stage;
		break;
	case MenuButton:
		//world->menu_stage->init();
		world->current_stage = world->menu_stage;
		break;
	}
}


//MENU STAGE
void Menu::init() {
	Game::instance->mouse_locked = false;
	isFinished = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

	camera2d = World::get_instance()->camera2D;

	Material background_mat;
	background_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_mat.diffuse = new Texture();
	background_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	background = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_mat);


	Material play_mat;
	play_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	play_mat.diffuse = new Texture();
	play_mat.diffuse = Texture::Get("data/ui/play_button.png");
	play_button = new EntityUI(Vector2(world_width * 0.5, 400), Vector2(240, 60), play_mat, eButtonId::PlayButton);

	Material exit_mat;
	exit_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	exit_mat.diffuse = new Texture();
	exit_mat.diffuse = Texture::Get("data/ui/exit_button.png");
	exit_button = new EntityUI(Vector2(world_width * 0.5, 500), Vector2(240, 60), exit_mat, eButtonId::EndButton);

	//background->addChild(play_button);
	//background->addChild(exit_button);


}

void Menu::restart() {

}

void Menu::render() {
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//drawText(world_width/2, world_height /2, "Clica para entarr al juego", Vector3(1, 1, 1), 2);
	//drawText(world_width / 2, (world_height / 2  )+20, "Apreta G para cojer la llave cuando estes cerca de ella", Vector3(1, 1, 1), 2);

	////SDL_GL_SwapWindow(this->window);
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	background->render(camera2d);
	play_button->render(camera2d);
	exit_button->render(camera2d);
}

void Menu::update(float seconds_elapsed) {
	//if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
	//	World* world = World::get_instance();
	//	world->current_stage = world->game_stage;
	//}
	background->update(seconds_elapsed);
	play_button->update(seconds_elapsed);
	exit_button->update(seconds_elapsed);
}

//void Menu::onButtonPressed(eButtonId buttonid) {
//	World* world = World::get_instance();
//
//	switch (buttonid) {
//	
//	case PlayButton:
//		world->current_stage = world->game_stage;
//		break;
//	
//	case EndButton:
//		exit(0);
//		break;
//	}
//}


//GAME STAGE
void GamePlay::init() {

	window_width = World::get_instance()->window_width;
	window_height = World::get_instance()->window_height;

	instance = this;
	
	camera2d = World::get_instance()->camera2D;

	//stamina
	Material stamina_mat;
	stamina_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/stamina.fs");
	stamina_mat.diffuse = new Texture();
	stamina_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	stamina_bar = new EntityUI(Vector2(window_width *0.2, window_height/ 10), Vector2(300, 20), stamina_mat, eButtonId::Stamina);

	//lifes
	Material life_mat;
	life_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/lifes.fs");
	life_mat.diffuse = new Texture();
	life_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	lifes_bar = new EntityUI(Vector2(window_width - 160, window_height / 10), Vector2(300, 20), life_mat, eButtonId::Life);




	//Player
	Material player_material;
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	player_material.diffuse = new Texture();
	player_material.diffuse->load("data/final_character/survivorFemaleA.png");
	Mesh* player_mesh = Mesh::Get("data/final_character/animations/character.MESH");

	player = new EntityPlayer(player_mesh, player_material, "player");
	player->model.translate(0.f, 6.f, 0.f);
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
}


void GamePlay::render() {
	camera->enable();

	glDisable(GL_DEPTH_TEST);
	skybox->model.setTranslation(camera->center);
	skybox->model.scale(70.f, 70.f, 70.f);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);

	World::get_instance()->root.render(camera);
	player->render(camera);
	stamina_bar->render_stamina(camera2d, GamePlay::get_instance()->player->stamina);
	lifes_bar->render_lifes(camera2d, GamePlay::get_instance()->player->lifes);

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
		
		//Interactions
		for (auto e : World::get_instance()->root.children) {
			EntityKey* key = dynamic_cast<EntityKey*>(e);
			EntityGate* gate = dynamic_cast<EntityGate*>(e);
			EntityHeart* heart = dynamic_cast<EntityHeart*>(e);
			
			if (key != nullptr) {
				Vector3 key_distance = player->position.distance(key->position);
				if (key_distance.length() < 7.f && Input::isKeyPressed(SDL_SCANCODE_G)) {
					key->with_player = true;
				}

				//Cheat mode: k vagi a la pos de la clau
				if (Input::isKeyPressed(SDL_SCANCODE_T)) {
					player->model.setTranslation(key->model.getTranslation());
				}
			}

			if (gate != nullptr) {
				for (auto e : World::get_instance()->root.children) {
					EntityKey* key_gate = dynamic_cast<EntityKey*>(e);

					if (key_gate != nullptr && key_gate->with_player && gate->distance(player) < 10.f) {
						World::get_instance()->root.removeChild(gate);
						World::get_instance()->root.removeChild(key_gate);
					}
				}				
			}
			
			if (heart != nullptr) {
				if (heart->distance(player) < 5.f) {
					World::get_instance()->root.removeChild(heart);
					player->lifes = player->lifes + 1;
				}
			}
		}		

		camera->lookAt(eye, center, Vector3(0, 1, 0));
	}

	World::get_instance()->root.update(seconds_elapsed);
	skybox->update(seconds_elapsed);
	stamina_bar->update(seconds_elapsed);
	lifes_bar->update(seconds_elapsed);

}

void GamePlay::restart() {
	Entity root = World::get_instance()->root;
	World::get_instance()->parseScene("data/myscene.scene", &root);
	root.render(camera);

}

void Death::init() {
	Game::instance->mouse_locked = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

	camera2d = World::get_instance()->camera2D;

	Material background_death_mat;
	background_death_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_death_mat.diffuse = new Texture();
	background_death_mat.diffuse = Texture::Get("data/ui/fondo_muerte.png");
	background_death = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_death_mat);


	Material play_again_mat;
	play_again_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	play_again_mat.diffuse = new Texture();
	play_again_mat.diffuse = Texture::Get("data/ui/play_again_button.png");
	play_again_button = new EntityUI(Vector2(world_width * 0.3, 520), Vector2(240, 60), play_again_mat, eButtonId::PlayAgainButton);

	Material menu_mat;
	menu_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	menu_mat.diffuse = new Texture();
	menu_mat.diffuse = Texture::Get("data/ui/menu_button.png");
	menu_button = new EntityUI(Vector2(world_width * 0.7, 520), Vector2(240, 60), menu_mat, eButtonId::MenuButton);

}

void Death::restart() {

}

void Death::render() {

	background_death->render(camera2d);
	play_again_button->render(camera2d);
	menu_button->render(camera2d);

}

void Death::update(float seconds_elapsed) {
	background_death->update(seconds_elapsed);
	play_again_button->update(seconds_elapsed);
	menu_button->update(seconds_elapsed);
}
