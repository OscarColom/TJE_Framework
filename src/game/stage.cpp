#include "stage.h"
#include "framework/input.h"
#include "framework/entities/entityPlayer.h"


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
	//camera = World::get_instance()->camera;

	//Key
	Material key_material;
	key_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	key_material.diffuse = new Texture();
	key_material.diffuse->load("data/key/Key_Material.png");
	Mesh* key_mesh = Mesh::Get("data/key/key.obj");
	
	key = new EntityMesh(key_mesh, key_material, "key");
	//key->model.scale(20.f, 50.f, 10.f);
	//key->model.translate(0.f, 5.f, 0.f);
	//key->model.translate(9.120f, 95.33f, 53.f);
	
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

}

//void GamePlay::OnKeyDown(SDL_KeyboardEvent event) {
//	switch (event.keysym.sym)
//	{
//	case SDLK_TAB:
//		free_camera = !free_camera;
//		mouse_locked = !mouse_locked;
//		SDL_ShowCursor(!mouse_locked);
//		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
//		break;
//	}
//
//}

void GamePlay::restart() {


}

void GamePlay::render() {
	camera->enable();

	glDisable(GL_DEPTH_TEST);
	skybox->model.setTranslation(camera->center);
	skybox->model.scale(70.f, 70.f, 70.f);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);

	root = World::get_instance()->root;
	root.render(camera);
	key->render(camera);
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

		camera->lookAt(eye, center, Vector3(0, 1, 0));
	}
	root.update(seconds_elapsed);
	key->update(seconds_elapsed);
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
