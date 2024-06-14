#include "game.h"
#include "framework/utils.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/fbo.h"
#include "graphics/shader.h"
#include "framework/input.h"
#include "framework/entities/entityPlayer.h"
#include <cmath>

#include "stage.h"

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float mouse_speed = 100.0f;

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//World
	world = new World();

	Camera::current = World::get_instance()->camera2D;
	
	//Audio
	audio->Init();
	channel = audio->Play("data/audio/temple-sound.wav", 0.1f, BASS_SAMPLE_LOOP);
}

//what to do when the image has to be draw
void Game::render(void)
{
	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
	// Create model matrix for cube
	world->render();

	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	// Swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	world->update(seconds_elapsed);
	int lifes = GamePlay::get_instance()->player->lifes;
	if (lifes <= 0) {
		Audio::Play("data/audio/Death_sound.wav", 1.5f, BASS_SAMPLE_MONO);
		GamePlay::get_instance()->player->lifes = 3;
		world = new World();
		world->current_stage = world->death_stage;
	}

	bool is_new_game = GamePlay::get_instance()->new_game;
	if (is_new_game == true) {
		GamePlay::get_instance()->player->lifes = 3;
		world = new World();
		world->current_stage = world->game_stage;
	}
}

	


//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	Stage* stage = World::get_instance()->current_stage;
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: 
			if (stage == World::get_instance()->game_stage) {
				World* world = World::get_instance();
				world->current_stage = world->options_stage;
			}
			else {
				must_exit = true; 
				break; //ESC key, kill the app
			}
		case SDLK_F1: Shader::ReloadAll(); break; 
	}

	world->OnKeyDown(event);
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		//mouse_locked = !mouse_locked;
		//SDL_ShowCursor(!mouse_locked);
		//SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{

}

//bool free_cam = World::get_instance()->free_camera;


void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1f : 0.9f;
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	//camera = Camera::current;//No estaab, al pasar camera a world poner cual se usta usando 
	
	//  Camera::current = World::get_instance()->camera;
	//if (World::get_instance()->current_stage != world->game_stage) {
	//	Camera::current = World::get_instance()->camera2D;
	//}
	//else {
	//	//Camera::current = World::get_instance()->camera;
	//}
	Camera::current->aspect =  width / (float)height;


	window_width = width;
	window_height = height;
}

