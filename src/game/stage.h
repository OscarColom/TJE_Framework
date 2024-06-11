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
	void onButtonPressed(eButtonId buttonid) ;


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

	Entity* background;
	Entity* play_button;
	Entity* exit_button;
	Entity* options_button;
	Entity* titulo;
	Entity* tutorial_button;




	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void onButtonPressed(eButtonId buttonid) {};
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

	//Entity root;

	EntityMesh* skybox = nullptr;
	//EntityKey* key = nullptr;
	//EntityHeart* heart = nullptr;
	//EntityGate* gate = nullptr;

	int window_width;
	int window_height;

	Camera* camera2d;
	EntityUI* stamina_bar;
	EntityUI* lifes_bar;
	EntityUI* fondo_barra;
	EntityUI* fondo_barra2;

	//Audio* audio = new Audio();
	//HCHANNEL channel_vidas;


	bool in_tutorial = false;


	//Camera* camera = nullptr;
	float angle = 0;
	bool mouse_locked = true; //tells if the mouse is locked (not seen)

	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.0f;
	//float mouse_speed = 10.f;

	bool free_camera = false;

	float  sphere_radius = 1.0f;
	float sphere_grow = .3f;
	float player_height = 3.f;

	Vector3 current_checkpoint = Vector3(0.0f, 5.0f, 0.0f);

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void OnKeyDown(SDL_KeyboardEvent event);

};




//class Tutorial : public Stage {
//	//static Tutorial* instance;
//
//	eStages type = TUTORIAL;
//public:
//
//	//static Tutorial* get_instance() {
//	//	if (instance == nullptr) {
//	//		instance = new Tutorial();
//	//	}
//	//	return instance;
//	//}
//
//
//	EntityMesh* skybox = nullptr;
//
//
//	int window_width;
//	int window_height;
//
//	Camera* camera2d;
//	EntityUI* stamina_bar;
//	EntityUI* lifes_bar;
//
//
//	float angle = 0;
//	bool mouse_locked = true; //tells if the mouse is locked (not seen)
//
//	float camera_yaw = 0.f;
//	float camera_pitch = 0.f;
//	float camera_speed = 2.0f;
//
//	bool free_camera = false;
//
//	float  sphere_radius = 1.0f;
//	float sphere_grow = .3f;
//	float player_height = 3.f;
//
//	Vector3 current_checkpoint = Vector3(0.0f, 5.0f, 0.0f);
//
//	void init();
//	void render();
//	void update(float seconds_elapsed);
//	void OnKeyDown(SDL_KeyboardEvent event);
//
//};



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
};



class Final : public Stage {
	eStages type = FINAL;
public:
	Final() {};


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
};