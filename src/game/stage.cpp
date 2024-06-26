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

	camera = World::get_instance()->camera;

}

Stage::~Stage() {

}

void Stage::onResize(int width, int height)
{

	//Camera::current->aspect = width / (float)height;
	World::get_instance()->camera2D->aspect = width / (float)height;

}

void Stage::onButtonPressed(eButtonId buttonid) {
	World* world = World::get_instance();
	switch (buttonid) {

	case PlayButton:
		GamePlay::get_instance()->in_tutorial = false;
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		world->current_stage = world->game_stage;
		GamePlay::get_instance()->new_game = true;
		GamePlay::get_instance()->player->lifes = 3;
		break;

	case EndButton:
		Audio::Play("data/audio/Click_button.wav", 1.5f, BASS_SAMPLE_MONO);
		if (world->current_stage == world->menu_stage) {
			exit(0);
		}
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
	exit_button = new EntityUI(Vector2(world_width * 0.5, 490), Vector2(220, 50), exit_mat, eButtonId::EndButton);

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

void Menu::onResize(int width, int height)
{
	World::get_instance()->camera2D->aspect = width / (float)height;

	world_width = width;
	world_height = height;

	background->position = Vector2(world_width * 0.5, world_height * 0.5);
	background->size = Vector2(world_width, world_height);

	titulo->position = Vector2(world_width * 0.5, 80);

	play_button->position = Vector2(world_width * 0.5, 250);
	exit_button->position = Vector2(world_width * 0.5, 490);
	options_button->position = Vector2(world_width * 0.5, 410);
	tutorial_button->position = Vector2(world_width * 0.5, 330);
}

void Menu::render() {

	if (SDL_WINDOWEVENT_RESIZED) {
		World::get_instance()->current_stage->onResize(World::get_instance()->window_width, World::get_instance()->window_height);
	}

	background->render(camera2d);
	play_button->render(camera2d);
	exit_button->render(camera2d);
	options_button->render(camera2d);
	titulo->render(camera2d);
	tutorial_button->render(camera2d);

}

void Menu::update(float seconds_elapsed) {

	if (SDL_WINDOWEVENT_RESIZED) {
		World::get_instance()->current_stage->onResize(World::get_instance()->window_width, World::get_instance()->window_height);
	}
	
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

	//Stamina
	Material stamina_mat;
	stamina_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/stamina.fs");
	stamina_mat.diffuse = new Texture();
	stamina_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	stamina_bar = new EntityUI(Vector2(window_width *0.2, window_height/ 10), Vector2(300, 20), stamina_mat, eButtonId::Stamina);

	Material fondo_barra_mat;
	fondo_barra_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	fondo_barra_mat.diffuse = new Texture();
	fondo_barra_mat.diffuse = Texture::Get("data/ui/barra_fondo.png");
	fondo_barra = new EntityUI(Vector2(window_width * 0.2, window_height / 10), Vector2(310, 30), fondo_barra_mat, eButtonId::Undefined);

	//Lifes
	Material life_mat;
	life_mat.shader = Shader::Get("data/shaders/stamina_quad.vs", "data/shaders/lifes2.fs");
	life_mat.diffuse = new Texture();
	life_mat.diffuse = Texture::Get("data/ui/fondo_menu.png");
	lifes_bar = new EntityUI(Vector2(window_width - 160, window_height / 10), Vector2(300, 20), life_mat, eButtonId::Life);
	fondo_barra2 = new EntityUI(Vector2(window_width - 160, window_height / 10), Vector2(310, 30), fondo_barra_mat, eButtonId::Undefined);

	//Player
	Material player_material;
	player_material.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	player_material.diffuse = new Texture();
	player_material.diffuse->load("data/final_character/survivorFemaleA.png");
	Mesh* player_mesh = Mesh::Get("data/final_character/animations/character.MESH");

	player = new EntityPlayer(player_mesh, player_material, "player");
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

	fondo_barra->render(camera2d);
	fondo_barra2->render(camera2d);

	stamina_bar->render_stamina(camera2d, GamePlay::get_instance()->player->stamina);
	lifes_bar->render_lifes(camera2d, GamePlay::get_instance()->player->lifes);
	//level->render_lifes(camera2d, num_doors);

	if (in_tutorial == true) {
		drawText(window_width * 0.02, window_height * 0.30, "Press W to move forward.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.35, "Press S to move backward.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.40, "Press A to move left.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.45, "Press D to move right.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.50, "Press Space to jump.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.55, "Press Shift to sprint.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.15, "Left-click mouse to pick up the keys.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.02, window_height * 0.20, "Keys open doors.", Vector3(0, 0, 0), 2.5f);
		drawText(window_width * 0.60, window_height * 0.35, "Reach the flag to win.", Vector3(0, 0, 0), 2.5f);
	}

	std::string str_num_doors = "Level: " + std::to_string(num_doors) + "/3";
	if (num_doors <= 3) {
		drawText(World::get_instance()->window_width / 2.3, World::get_instance()->window_width / 15, str_num_doors, Vector3(1, 1, 1), 2);

	}
	else if (num_doors > 3) {
		std::string str_num_doors = "Level: " + std::to_string(num_doors-1) + "/3";
		drawText(World::get_instance()->window_width / 2.3, World::get_instance()->window_width / 15, str_num_doors, Vector3(1, 1, 1), 2);

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

				if (Input::isKeyPressed(SDL_SCANCODE_T)) {
					player->model.setTranslation(key->model.getTranslation());
				}
			}

			if (gate != nullptr) {
				for (auto e : World::get_instance()->root.children) {
					EntityKey* key_gate = dynamic_cast<EntityKey*>(e);

					if (key_gate != nullptr && key_gate->with_player && gate->distance(player) < 10.f) {
						Audio::Play("data/audio/Open_Door.wav", 0.5f, BASS_SAMPLE_MONO);
						World::get_instance()->root.removeChild(gate);
						World::get_instance()->root.removeChild(key_gate);
						num_doors += 1;
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
					Audio::Play("data/audio/Victory_sound.wav", 1.0f, BASS_SAMPLE_MONO);
					World* world = World::get_instance();
					world->current_stage = world->final_stage;
				}
			}
			if (flag != nullptr && flag->name != "scene/flag-pennant@flag/flag-pennant@flag.obj") {
				if (flag->distance(player) < 10.f) {
					Audio::Play("data/audio/Victory_sound.wav", 0.4f, BASS_SAMPLE_MONO);
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
	play_again_button = new EntityUI(Vector2(world_width * 0.25, 520), Vector2(220, 50), play_again_mat, eButtonId::PlayAgainButton);

	Material menu_mat;
	menu_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	menu_mat.diffuse = new Texture();
	menu_mat.diffuse = Texture::Get("data/ui/menu_button.png");
	menu_button = new EntityUI(Vector2(world_width * 0.75, 520), Vector2(220, 50), menu_mat, eButtonId::MenuButton);

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
	background_death_mat.diffuse = Texture::Get("data/ui/fondo_final.png");
	background_death = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(world_width, world_height), background_death_mat);

	Material menu_mat;
	menu_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	menu_mat.diffuse = new Texture();
	menu_mat.diffuse = Texture::Get("data/ui/menu_button.png");
	menu_button_f = new EntityUI(Vector2(world_width * 0.7, 520), Vector2(240, 60), menu_mat, eButtonId::MenuButton);


	Material bar_tiemp_mat;
	bar_tiemp_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	bar_tiemp_mat.diffuse = new Texture();
	bar_tiemp_mat.diffuse = Texture::Get("data/ui/barra_fondo.png");
	menu_barra_tiempo = new EntityUI(Vector2(world_width * 0.4, world_height * 0.22), Vector2(520, 60), bar_tiemp_mat, eButtonId::Undefined);


	//medallas
	Material bronce_mat;
	bronce_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/medallas.fs");
	bronce_mat.diffuse = new Texture();
	bronce_mat.diffuse = Texture::Get("data/ui/bronce.png");
	bronce = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(200, 200), bronce_mat, eButtonId::Undefined);

	Material plata_mat;
	plata_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/medallas.fs");
	plata_mat.diffuse = new Texture();
	plata_mat.diffuse = Texture::Get("data/ui/plata.png");
	plata = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(200, 200), plata_mat, eButtonId::Undefined);

	Material oro_mat;
	oro_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/medallas.fs");
	oro_mat.diffuse = new Texture();
	oro_mat.diffuse = Texture::Get("data/ui/oro.png");
	oro = new EntityUI(Vector2(world_width * 0.5, world_height * 0.5), Vector2(200, 200), oro_mat, eButtonId::Undefined);

}

void Final::restart() {

}

void Final::render() {

	background_death->render(camera2d);


	menu_barra_tiempo->render(camera2d);
	long elapsed_time_game = World::get_instance()->elapsed_time_game;
	std::string str_total_time = "Time to complete: " + std::to_string(elapsed_time_game) + " seconds";
	drawText(world_width* 0.1 , world_height* 0.2, str_total_time, Vector3(0, 0, 0), 3);

	menu_button_f->render(camera2d);

	if (World::get_instance()->elapsed_time_game < 130.0f) {
		oro->render(camera2d);
	}
	else if (World::get_instance()->elapsed_time_game <= 200.0f) {
		plata->render(camera2d);
	}
	else {
		bronce->render(camera2d);
	}


}

void Final::update(float seconds_elapsed) {
	background_death->update(seconds_elapsed);
	menu_button_f->update(seconds_elapsed);
	menu_barra_tiempo->update(seconds_elapsed);

	bronce->update(seconds_elapsed);
	plata->update(seconds_elapsed);
	oro->update(seconds_elapsed);

}