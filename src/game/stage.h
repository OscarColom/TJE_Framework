#pragma once
#include <vector>
#include "world.h"
#include "game.h"
#include "framework/entities/entity_Gate.h"
#include "framework/entities/entityUI.h"


enum eStages {
	UNDEFINED,
	MENU,
	OPTIONS,
	GAMEPLAY,
	TUTORIAL,
	DEATH,
	FINAL
};

class Stage {
	eStages type = UNDEFINED;
public:
	Stage();
	virtual ~Stage();

	EntityPlayer* player = nullptr;

	bool canPress;
	bool isFinished;
	int whatStage;
	int nextStage;

	int window_width;
	int window_height;
	Camera* camera = nullptr;
	bool mouse_locked = true;


	// Methods that should be overwritten
	// by derived classes 
	virtual void init() {};
	virtual void restart() {};
	virtual void render() {};
	virtual void update(float elapsed_time) {};
	virtual void MouseButDown(SDL_MouseButtonEvent event) {};
	void onButtonPressed(eButtonId buttonid);
	virtual void onResize(int width, int height);

	// Pointer to parent
	Stage* parent;

	// Pointers to children
	std::vector<Stage*> children;
};


class Menu : public Stage {

	eStages type = MENU;
public:
	Menu() {};

	Texture* fondo;
	int world_width;
	int world_height;

	Camera* camera2d;

	EntityUI* background;
	EntityUI* play_button;
	EntityUI* exit_button;
	EntityUI* options_button;
	EntityUI* titulo;
	EntityUI* tutorial_button;

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void onButtonPressed(eButtonId buttonid) {};
	void onResize(int width, int height);
};




class Options : public Stage {

	eStages type = OPTIONS;
public:
	Options() {};

	Texture* fondo;
	int world_width;
	int world_height;

	Camera* camera2d;

	Entity* background;
	Entity* sensitivity;

	Entity* low_button;
	Entity* hueco;
	Entity* high_button;
	Entity* back_button;
	Entity* continue_button;

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void onButtonPressed(eButtonId buttonid) {};
	void onResize(int width, int height) {};
};





class GamePlay : public Stage {
	static GamePlay* instance;

	eStages type = GAMEPLAY;
public:

	static GamePlay* get_instance() {
		if (instance == nullptr) {
			instance = new GamePlay();
		}
		return instance;
	}

	EntityMesh* skybox = nullptr;

	int window_width;
	int window_height;

	Camera* camera2d;
	EntityUI* stamina_bar;
	EntityUI* lifes_bar;
	EntityUI* fondo_barra;
	EntityUI* fondo_barra2;
	EntityUI* level;
	bool new_game = false;

	int num_doors = 1;
	bool in_tutorial = false;

	float angle = 0;
	bool mouse_locked = true; //tells if the mouse is locked (not seen)

	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.0f;

	bool free_camera = false;

	float  sphere_radius = 1.0f;
	float sphere_grow = .3f;
	float player_height = 3.f;

	Vector3 current_checkpoint = Vector3(0.0f, 5.0f, 0.0f);

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void OnKeyDown(SDL_KeyboardEvent event) {};
	void onResize(int width, int height) {};

};


class Death : public Stage {
	eStages type = DEATH;
public:
	Death() {};


	//Texture* fondo;
	int world_width;
	int world_height;

	Camera* camera2d;

	Entity* background_death;
	Entity* play_again_button;
	Entity* menu_button;


	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void onButtonPressed(eButtonId buttonid) {};
	void onResize(int width, int height) {};
};



class Final : public Stage {
	eStages type = FINAL;
public:
	Final() {};


	//Texture* fondo;
	int world_width;
	int world_height;

	Camera* camera2d;

	EntityUI* background_death;
	EntityUI* play_again_button;
	EntityUI* menu_button_f;
	EntityUI* menu_barra_tiempo;


	EntityUI* bronce;
	EntityUI* plata;
	EntityUI* oro;


	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void onButtonPressed(eButtonId buttonid) {};
	void onResize(int width, int height) {};
};