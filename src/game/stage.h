#pragma once
#include <vector>
#include "world.h"
#include "game.h"
#include "framework/entities/entity_Gate.h"

enum eStages {
	UNDEFINED,
	MENU,
	GAMEPLAY,
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


	// Methods that should be overwritten
	// by derived classes 
	virtual void init() {};
	virtual void restart() {};
	virtual void render() {};
	virtual void update(float elapsed_time) {};
	virtual void MouseButDown(SDL_MouseButtonEvent event) {};


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

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
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
	EntityKey* key = nullptr;
	EntityHeart* heart = nullptr;
	//EntityGate* gate = nullptr;

	int window_width;
	int window_height;

	//Camera* camera = nullptr;
	float angle = 0;
	bool mouse_locked = true; //tells if the mouse is locked (not seen)

	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.0f;
	float mouse_speed = 10.f;

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


class Death : public Stage {
public:
	Death() {};


	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
};