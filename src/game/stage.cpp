#pragma once
#include "stage.h"
#include "framework/input.h"
#include "framework/entities/entityPlayer.h"
#include "framework/entities/entityKey.h"
#include "framework/entities/entity_Gate.h"
#include "framework/entities/entity_Flag.h"




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
		GamePlay::get_instance()->in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		GamePlay::get_instance()->player->lifes = 3;
		GamePlay::get_instance()->player->stamina = 50.f;
		world->current_stage = world->game_stage;
		GamePlay::get_instance()->player->model.setTranslation(Vector3(0.f, 2.f, 0.f));
		break;

	case EndButton:
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		exit(0);
		break;

	case PlayAgainButton:
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		world->current_stage = world->game_stage;
		break;

	case MenuButton:
		GamePlay::get_instance()->in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		world->current_stage = world->menu_stage;
		break;

	case OptionsButton:
		//in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		world->current_stage = world->options_stage;
		break;

	case HighButton:
		//in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		Game::instance->mouse_speed += 0.01f;
		break;

	case LowButton:
		//in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		Game::instance->mouse_speed -= 0.01f;
		break;

	case TutorialButton:
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		world->current_stage = world->game_stage;
		GamePlay::get_instance()->player->model.setTranslation(Vector3(380.f,32.f,330.f));
		GamePlay::get_instance()->in_tutorial = true;
		break;

	}


}


/*MENU STAGE
#############################################################################
##############################################################################*/
void Menu::init() {
	//Game::instance->mouse_locked = false;
	isFinished = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

	camera2d = World::get_instance()->camera2D;

	Material background_mat;
	background_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_mat.diffuse = new Texture();
	background_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	background = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_mat);

	Material titulo_mat;
	titulo_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	titulo_mat.diffuse = new Texture();
	titulo_mat.diffuse = Texture::Get("data/ui/titulo.png");
	titulo = new EntityUI(Vector2(world_width * 0.5, 80), Vector2(400, 100), titulo_mat, eButtonId::Undefined);


	Material play_mat;
	play_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	play_mat.diffuse = new Texture();
	play_mat.diffuse = Texture::Get("data/ui/play_button.png");
	play_button = new EntityUI(Vector2(world_width * 0.5, 250), Vector2(240, 60), play_mat, eButtonId::PlayButton);

	Material exit_mat;
	exit_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	exit_mat.diffuse = new Texture();
	exit_mat.diffuse = Texture::Get("data/ui/exit_button.png");
	exit_button = new EntityUI(Vector2(world_width * 0.5, 490), Vector2(240, 60), exit_mat, eButtonId::EndButton);

	Material options_mat;
	options_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	options_mat.diffuse = new Texture();
	options_mat.diffuse = Texture::Get("data/ui/options_button.png");
	options_button = new EntityUI(Vector2(world_width * 0.5, 410), Vector2(240, 60), options_mat, eButtonId::OptionsButton);

	Material tutorial_mat;
	tutorial_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	tutorial_mat.diffuse = new Texture();
	tutorial_mat.diffuse = Texture::Get("data/ui/tutorial_button.png");
	tutorial_button = new EntityUI(Vector2(world_width * 0.5, 330), Vector2(240, 60), tutorial_mat, eButtonId::TutorialButton);





}

void Menu::restart() {

}

void Menu::render() {

	background->render(camera2d);
	play_button->render(camera2d);
	exit_button->render(camera2d);
	options_button->render(camera2d);
	titulo->render(camera2d);
	tutorial_button->render(camera2d);

}

void Menu::update(float seconds_elapsed) {

	background->update(seconds_elapsed);
	play_button->update(seconds_elapsed);
	exit_button->update(seconds_elapsed);
	options_button->update(seconds_elapsed);
	titulo->update(seconds_elapsed);
	tutorial_button->update(seconds_elapsed);
}







/*OPTIONS STAGE
#############################################################################
##############################################################################*/
void Options::init() {
	//Game::instance->mouse_locked = false;
	isFinished = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

	camera2d = World::get_instance()->camera2D;

	Material background_mat;
	background_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_mat.diffuse = new Texture();
	background_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	background = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_mat);


	Material sensitivity_mat;
	sensitivity_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	sensitivity_mat.diffuse = new Texture();
	sensitivity_mat.diffuse = Texture::Get("data/ui/Sensivility_button.png");
	sensitivity = new EntityUI(Vector2(world_width * 0.4, world_height * 0.5), Vector2(270, 60), sensitivity_mat, eButtonId::Undefined);


	Material low_mat;
	low_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	low_mat.diffuse = new Texture();
	low_mat.diffuse = Texture::Get("data/ui/low_button.png");
	low_button = new EntityUI(Vector2(world_width * 0.65, (world_height * 0.5) + 100), Vector2(60, 60), low_mat, eButtonId::LowButton);

	Material hueco_mat;
	hueco_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	hueco_mat.diffuse = new Texture();
	hueco_mat.diffuse = Texture::Get("data/ui/hueco.png");
	hueco = new EntityUI(Vector2(world_width * 0.65, world_height * 0.5), Vector2(60, 60), hueco_mat, eButtonId::Undefined);

	Material high_mat;
	high_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	high_mat.diffuse = new Texture();
	high_mat.diffuse = Texture::Get("data/ui/high_button.png");
	high_button = new EntityUI(Vector2(world_width * 0.65, (world_height * 0.5) - 100), Vector2(60, 60), high_mat, eButtonId::HighButton);

	Material goback_mat;
	goback_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	goback_mat.diffuse = new Texture();
	goback_mat.diffuse = Texture::Get("data/ui/atras_button.png");
	back_button = new EntityUI(Vector2(world_width * 0.1, world_height * 0.1), Vector2(60, 60), goback_mat, eButtonId::MenuButton);

	Material continue_mat;
	continue_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	continue_mat.diffuse = new Texture();
	continue_mat.diffuse = Texture::Get("data/ui/continue_button.png");
	continue_button = new EntityUI(Vector2(world_width * 0.4, world_height * 0.7), Vector2(60, 60), continue_mat, eButtonId::PlayAgainButton);



}

void Options::restart() {

}

void Options::render() {

	background->render(camera2d);
	sensitivity->render(camera2d);
	low_button->render(camera2d);
	hueco->render(camera2d);
	high_button->render(camera2d);
	back_button->render(camera2d);
	continue_button->render(camera2d);

	long sensibilidad = Game::instance->mouse_speed;
	std::string str_sensibilidad = std::to_string(sensibilidad);
	drawText(world_width * 0.63, world_height * 0.48, str_sensibilidad, Vector3(1, 1, 1), 3);



}

void Options::update(float seconds_elapsed) {

	background->update(seconds_elapsed);
	sensitivity->update(seconds_elapsed);
	low_button->update(seconds_elapsed);
	hueco->update(seconds_elapsed);
	high_button->update(seconds_elapsed);
	back_button->update(seconds_elapsed);
	continue_button->update(seconds_elapsed);
}

				/*GAME STAGE
#############################################################################
##############################################################################*/

void GamePlay::init() {

	window_width = World::get_instance()->window_width;
	window_height = World::get_instance()->window_height;
	//World::get_instance()->mouse_locked = true;

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
	life_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/lifes2.fs");
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
	//player->model.translate(0.f, 6.f, 0.f);
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

	if (in_tutorial == true) {
		drawText(window_width * 0.02, window_height * 0.30, "Press W to move forward.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.35, "Press S to move backward.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.40, "Press A to move left.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.45, "Press D to move right.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.50, "Press Space to jump.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.55, "Press Shift to sprint.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.15, "Click with left to pick up the keys.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.20, "With the keys, you can open the doors.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.60, window_height * 0.35, "Reach the flag to win.", Vector3(0, 0, 0), 2.5f);


	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


}

void GamePlay::update(float seconds_elapsed) {
	float mouse_speed = Game::instance->mouse_speed;
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

		camera_yaw -= Input::mouse_delta.x * Game::instance->elapsed_time * 2.f * (0.1 * mouse_speed);
		camera_pitch -= Input::mouse_delta.y * Game::instance->elapsed_time * 2.f * (0.1 * mouse_speed);

		camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

		Matrix44 myaw;
		myaw.setRotation(camera_yaw , Vector3(0, 1, 0));  //sensibilidad

		Matrix44 mpitch;
		mpitch.setRotation(camera_pitch , Vector3(-1, 0, 0));

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
			EntityFlag* flag = dynamic_cast<EntityFlag*>(e);


			
			if (key != nullptr) {
				Vector3 key_distance = player->position.distance(key->position);
				if (key_distance.length() < 7.f && Input::isMousePressed(SDL_BUTTON_RIGHT)/*Input::isKeyPressed(SDL_SCANCODE_G)*/) {
					Audio::Play("data/audio/Get_key.wav", 1.5f, BASS_SAMPLE_MONO);
					key->with_player = true;
				}

				//if (player->lifes <= 0){/////////////////////7
				//	if (key->with_player = true) {
				//		World::get_instance()->root.removeChild(key);
				//	}
				//}
				// 
				//Cheat mode: k vagi a la pos de la clau
				if (Input::isKeyPressed(SDL_SCANCODE_T)) {
					player->model.setTranslation(key->model.getTranslation());
				}
			}

			if (gate != nullptr) {
				for (auto e : World::get_instance()->root.children) {
					EntityKey* key_gate = dynamic_cast<EntityKey*>(e);

					if (key_gate != nullptr && key_gate->with_player && gate->distance(player) < 10.f) {
						Audio::Play("data/audio/Open_Door.wav", 1.f, BASS_SAMPLE_MONO);
						World::get_instance()->root.removeChild(gate);
						World::get_instance()->root.removeChild(key_gate);
					}
				}				
			}
			
			if (heart != nullptr) {
				if (heart->distance(player) < 5.f) {
					Audio::Play("data/audio/Get_Life.wav", 1.5f, BASS_SAMPLE_MONO);
					World::get_instance()->root.removeChild(heart);
					player->lifes = player->lifes + 1;
				}
			}

			if (flag != nullptr && flag->name == "scene/flag-pennant@flag/flag-pennant@flag.obj") {
				if (flag->distance(player) < 10.f) {
					player->lifes = player->lifes + 1;/////////////////////
					Audio::Play("data/audio/Victory_sound.wav", 1.5f, BASS_SAMPLE_MONO);
					World* world = World::get_instance();
					world->current_stage = world->final_stage;
				}
			}
			if (flag != nullptr && flag->name != "scene/flag-pennant@flag/flag-pennant@flag.obj") {
				if (flag->distance(player) < 10.f) {
					Audio::Play("data/audio/Victory_sound.wav", 0.5f, BASS_SAMPLE_MONO);
					World* world = World::get_instance();
					world->current_stage = world->menu_stage;
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




/*GAME STAGE
#############################################################################
##############################################################################*/

//void Tutorial::init() {
//
//	window_width = World::get_instance()->window_width;
//	window_height = World::get_instance()->window_height;
//	//World::get_instance()->mouse_locked = true;
//
//	//instance = this;
//
//	camera2d = World::get_instance()->camera2D;
//
//	//stamina
//	Material stamina_mat;
//	stamina_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/stamina.fs");
//	stamina_mat.diffuse = new Texture();
//	stamina_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
//	stamina_bar = new EntityUI(Vector2(window_width * 0.2, window_height / 10), Vector2(300, 20), stamina_mat, eButtonId::Stamina);
//
//	//lifes
//	Material life_mat;
//	life_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/lifes.fs");
//	life_mat.diffuse = new Texture();
//	life_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
//	lifes_bar = new EntityUI(Vector2(window_width - 160, window_height / 10), Vector2(300, 20), life_mat, eButtonId::Life);
//
//
//
//
//	//Player
//	Material player_material;
//	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
//	player_material.diffuse = new Texture();
//	player_material.diffuse->load("data/final_character/survivorFemaleA.png");
//	Mesh* player_mesh = Mesh::Get("data/final_character/animations/character.MESH");
//
//	player = new EntityPlayer(player_mesh, player_material, "player");
//	player->model.translate(0.f, -40.f, 0.f);
//	player->isAnimated = true;
//
//	//Skybox
//	Material sky_cubemap;
//	sky_cubemap.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
//	sky_cubemap.diffuse = new Texture();
//	sky_cubemap.diffuse->loadCubemap("landscape", {
//		"data/CubeSky/px.png",
//		"data/CubeSky/nx.png",
//		"data/CubeSky/ny.png",
//		"data/CubeSky/py.png",
//		"data/CubeSky/pz.png",
//		"data/CubeSky/nz.png"
//		});
//	Mesh* skybox_mesh = Mesh::Get("data/meshes/cubemap.ASE");
//	skybox = new EntityMesh(skybox_mesh, sky_cubemap, "landscape");
//	skybox->model.scale(70.f, 70.f, 70.f);
//}
//
//
//void Tutorial::render() {
//	camera->enable();
//
//	glDisable(GL_DEPTH_TEST);
//	skybox->model.setTranslation(camera->center);
//	skybox->model.scale(70.f, 70.f, 70.f);
//	skybox->render(camera);
//	glEnable(GL_DEPTH_TEST);
//
//	World::get_instance()->root.render(camera);
//	player->render(camera);
//	stamina_bar->render_stamina(camera2d, GamePlay::get_instance()->player->stamina);
//	lifes_bar->render_lifes(camera2d, GamePlay::get_instance()->player->lifes);
//
//	glDisable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//
//}
//
//void Tutorial::update(float seconds_elapsed) {
//	float mouse_speed = Game::instance->mouse_speed;
//	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
//	free_camera = World::get_instance()->free_camera;
//	mouse_locked = World::get_instance()->mouse_locked;
//	if (free_camera) {
//		if (Input::isMousePressed(SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
//		{
//			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
//			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
//		}
//
//		// Async input to move the camera around
//		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 20; //move faster with left shift
//		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
//		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
//		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
//		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
//	}
//	else {
//
//		player->update(seconds_elapsed);
//
//		Vector3 eye;
//		Vector3 center;
//
//		SDL_ShowCursor(!mouse_locked);
//		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
//
//		camera_yaw -= Input::mouse_delta.x * Game::instance->elapsed_time * 2.f * (0.1 * mouse_speed);
//		camera_pitch -= Input::mouse_delta.y * Game::instance->elapsed_time * 2.f * (0.1 * mouse_speed);
//
//		camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);
//
//		Matrix44 myaw;
//		myaw.setRotation(camera_yaw, Vector3(0, 1, 0));  //sensibilidad
//
//		Matrix44 mpitch;
//		mpitch.setRotation(camera_pitch, Vector3(-1, 0, 0));
//
//		Vector3 front = (mpitch * myaw).frontVector().normalize();
//
//		float orbit_dist = 6.f;
//		Vector3 corrector = Vector3(0.f, 4.1f, 0.f);
//		eye = (player->model.getTranslation() - front * orbit_dist) + corrector;
//		center = player->model.getTranslation() + Vector3(0.f, 4.1f, 0.f); //The last vector is bc we cound be pointing at the char feet otherwise
//
//		Vector3 dir = eye - center;
//		sCollisionData data = World::get_instance()->ray_cast(center, dir.normalize(), eCollisionFilter::ALL, dir.length());///////////////PELIOGROO
//
//		if (data.collided) {
//			eye = data.col_point;
//		}
//
//		//Interactions
//		for (auto e : World::get_instance()->root.children) {
//			EntityKey* key = dynamic_cast<EntityKey*>(e);
//			EntityGate* gate = dynamic_cast<EntityGate*>(e);
//			EntityHeart* heart = dynamic_cast<EntityHeart*>(e);
//			EntityFlag* flag = dynamic_cast<EntityFlag*>(e);
//
//
//			if (key != nullptr) {
//				Vector3 key_distance = player->position.distance(key->position);
//				if (key_distance.length() < 7.f && Input::isKeyPressed(SDL_SCANCODE_G)) {
//					Audio::Play("data/audio/Get_key.wav", 1.5f, BASS_SAMPLE_MONO);
//					key->with_player = true;
//				}
//
//				//Cheat mode: k vagi a la pos de la clau
//				if (Input::isKeyPressed(SDL_SCANCODE_T)) {
//					player->model.setTranslation(key->model.getTranslation());
//				}
//			}
//
//			if (gate != nullptr) {
//				for (auto e : World::get_instance()->root.children) {
//					EntityKey* key_gate = dynamic_cast<EntityKey*>(e);
//
//					if (key_gate != nullptr && key_gate->with_player && gate->distance(player) < 10.f) {
//						Audio::Play("data/audio/Open_Door.wav", 1.f, BASS_SAMPLE_MONO);
//						World::get_instance()->root.removeChild(gate);
//						World::get_instance()->root.removeChild(key_gate);
//					}
//				}
//			}
//
//			if (heart != nullptr) {
//				if (heart->distance(player) < 5.f) {
//					Audio::Play("data/audio/Get_Life.wav", 1.5f, BASS_SAMPLE_MONO);
//					World::get_instance()->root.removeChild(heart);
//					player->lifes = player->lifes + 1;
//				}
//			}
//
//			if (flag != nullptr) {
//				if (flag->distance(player) < 10.f) {
//					player->lifes = player->lifes + 1;/////////////////////
//					Audio::Play("data/audio/Victory_sound.wav", 1.5f, BASS_SAMPLE_MONO);
//					World* world = World::get_instance();
//					world->current_stage = world->final_stage;
//				}
//			}
//		}
//
//		camera->lookAt(eye, center, Vector3(0, 1, 0));
//	}
//
//	World::get_instance()->root.update(seconds_elapsed);
//	skybox->update(seconds_elapsed);
//	stamina_bar->update(seconds_elapsed);
//	lifes_bar->update(seconds_elapsed);
//
//}



/*DEATH STAGE
#############################################################################
##############################################################################*/
void Death::init() {
	//World::get_instance()->mouse_locked  = false;
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


				/*FINAL STAGE
#############################################################################
##############################################################################*/

void Final::init() {
	//Game::instance->mouse_locked = false;
	world_width = World::get_instance()->window_width;
	world_height = World::get_instance()->window_height;

	camera2d = World::get_instance()->camera2D;

	Material background_death_mat;
	background_death_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_death_mat.diffuse = new Texture();
	background_death_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	background_death = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_death_mat);


	/*Material play_again_mat;
	play_again_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	play_again_mat.diffuse = new Texture();
	play_again_mat.diffuse = Texture::Get("data/ui/play_again_button.png");
	play_again_button = new EntityUI(Vector2(world_width * 0.3, 520), Vector2(240, 60), play_again_mat, eButtonId::PlayAgainButton);

	Material menu_mat;
	menu_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	menu_mat.diffuse = new Texture();
	menu_mat.diffuse = Texture::Get("data/ui/menu_button.png");
	menu_button = new EntityUI(Vector2(world_width * 0.7, 520), Vector2(240, 60), menu_mat, eButtonId::MenuButton);*/

}

void Final::restart() {

}

void Final::render() {

	background_death->render(camera2d);
	//play_again_button->render(camera2d);
	//menu_button->render(camera2d);
	long elapsed_time_game = World::get_instance()->elapsed_time_game;
	std::string str_total_time = "Time to complete the game: " + std::to_string(elapsed_time_game) + " seconds";
	drawText(world_width* 0.2 , world_height* 0.5, str_total_time, Vector3(0, 0, 0), 3);

}

void Final::update(float seconds_elapsed) {
	background_death->update(seconds_elapsed);
	//play_again_button->update(seconds_elapsed);
	//menu_button->update(seconds_elapsed);
}