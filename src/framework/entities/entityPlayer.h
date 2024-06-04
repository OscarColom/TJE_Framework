#pragma once
#include "framework/entities/entityMesh.h"

enum eAnimationState {
	IDLE,
	WALKING,
	RUNNING,
	JUMP
};

class EntityPlayer : public EntityMesh {

	eAnimationState animation_state = eAnimationState::IDLE;

public:

	float walk_speed = 8.f;
	Vector3 velocity = Vector3(0.0f);
	Vector3 position;

	int lifes = 3;
	float fall = 0.f;
	bool is_on_plataform = false;

	float stamina = 50.0f; // Resistencia inicial
	float max_stamina = 50.0f; // Máxima resistencia
	float stamina_consumption_rate = 20.0f; // Tasa de consumo por segundo
	float stamina_recovery_rate = 10.0f; // Tasa de recuperación por segundo
	bool is_sprinting = false; // Indica si el jugador está esprintando

	EntityPlayer() {};
	EntityPlayer(Mesh* player_mesh, const Material& player_material, const std::string& name) ;
	~EntityPlayer();

	void update(float seconds_elapsed);
	void render(Camera* camera);

	int Getlifes() {
		return lifes;
	};

};
