#include "entityPlayer.h"
#include "game/game.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"
#include "game/stage.h"

EntityPlayer::EntityPlayer(Mesh* player_mesh, const Material& player_material, const std::string& name) {
	this->mesh = player_mesh;
	this->material = player_material;
	this->name = name;

	animator.playAnimation("data/final_character/animations/idle.skanim");
}

EntityPlayer::~EntityPlayer() {

}

void EntityPlayer::render(Camera* camera) {

	EntityMesh::render(camera);
	int world_width = World::get_instance()->window_width;

	std::string str_stam = "Stamina: " + std::to_string(stamina);
	drawText(20, 20, str_stam, Vector3(1, 1, 1), 2);

	std::string str_lifes = "Lifes: " + std::to_string(lifes);
	drawText(world_width - 100, 20, str_lifes, Vector3(1, 1, 1), 2);	
}

void EntityPlayer::update(float seconds_elapsed) {
	float camera_yaw = GamePlay::get_instance()->camera_yaw;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	//Axes where the char is gonna move
	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();

	//Where the character is
	position = model.getTranslation();

	Vector3 move_dir;

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir += front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir -= front;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir += right;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir -= right;

	float speed_mult = walk_speed;

	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) && stamina > 0.0f) {
		// El jugador está esprintando y tiene resistencia
		is_sprinting = true;
		stamina -= stamina_consumption_rate * seconds_elapsed; // Reduce la resistencia
		stamina = std::max(0.0f, stamina); // Asegúrate de que la resistencia no sea negativa

		if (stamina > 0.0f) {
			speed_mult *= 1.8f; 
		}
		else {
			is_sprinting = false;
		}
	}
	else {
		// El jugador no está esprintando, recupera resistencia
		stamina += stamina_recovery_rate * seconds_elapsed; // Recupera resistencia
		stamina = std::min(max_stamina, stamina); // Asegúrate de que la resistencia no supere el máximo
		is_sprinting = false;
	}

	move_dir.normalize();
	move_dir *= speed_mult;

	velocity += move_dir;

	//CHECK COLLISIONS
	std::vector<sCollisionData> collisions;
	std::vector<sCollisionData> ground_collisions;

	for (auto e : World::get_instance()->root.children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		EntityGate* gate = dynamic_cast<EntityGate*>(e);
		if (ec != nullptr) {
			ec->getCollisions(position + velocity * seconds_elapsed, collisions, ground_collisions, ALL);
		}
		if (gate != nullptr) {
			EntityGate* gt = dynamic_cast<EntityGate*>(gate);
			gt->getCollisions(position + velocity * seconds_elapsed, collisions, ground_collisions, ALL);
		}
	}

	//Ground collsisons
	bool is_grounded = false;

	for (const sCollisionData& collision : ground_collisions) {
		//If normal pointig upward,it means is floor collisons
		float up_factor = fabs(collision.col_normal.dot(Vector3::UP));
		if (up_factor > 0.5) {
			is_grounded = true;
		}

		if (collision.col_point.y  > (position.y + velocity.y * seconds_elapsed)) {
			position.y = collision.col_point.y;
		}
	}

	if (!is_grounded) {
		velocity.y -= 25.f * seconds_elapsed;

	}
	else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		velocity.y = 15.f;
		animator.playAnimation("data/final_character/animations/jump_prueba.skanim", false, 0.6f);
		animation_state = eAnimationState::JUMP;
		Audio::Play("jump.wav", 1, true);
	}

	//Env collisions
	for (const sCollisionData& collision : collisions) {
		//Move along wall when colliding
		Vector3  newDir = velocity.dot(collision.col_normal) * collision.col_normal;
		velocity.x -= newDir.x;
		velocity.y -= newDir.y;
		velocity.z -= newDir.z;
	}

	if (velocity.length() > 0.f && is_grounded) {
		if (is_sprinting) {
			Audio::Play("run.wav", 1, BASS_SAMPLE_LOOP);
		}
		else {
			Audio::Play("footstep.wav", 1, BASS_SAMPLE_LOOP);
		}
	}
	
	//Animation
	if ( (animation_state == eAnimationState::IDLE || animation_state == eAnimationState::RUNNING) && velocity.length() > 1.f && !is_sprinting) {
		animator.playAnimation("data/final_character/animations/walk.skanim", true, 0.6f);
		animation_state = eAnimationState::WALKING;
		//printf("Walking");
	} 
	
	if (velocity.length() < 1.f && is_grounded) {
		animator.playAnimation("data/final_character/animations/idle.skanim", true, 0.6f);
		animation_state = eAnimationState::IDLE;
		//printf("idle");
		//printf("%f", velocity.length());
	}

	if (animation_state == eAnimationState::WALKING && velocity.length() > 2.f && is_sprinting && is_grounded) {
		animator.playAnimation("data/final_character/animations/run.skanim", true, 0.6f);
		animation_state = eAnimationState::RUNNING;
	}
	
	// Update players position
	position += velocity * seconds_elapsed;

	//Por si el jugador se cae
	//if (position.y < -100 || (position.y < 34.f && is_on_plataform)) {
	//	lifes -= 1;
	//	is_on_plataform = false;
	//}

	static float fall_start_height = 0.0f;
	static bool is_falling = false;

	if (is_grounded) {
		if (is_falling) {
			// Calcula la distancia de caída
			float fall_distance = fall_start_height - position.y;
			if (fall_distance > 10.0f && fall_distance < 20.f) { 
				lifes -= 1; 
			}
			else if (fall_distance > 20.0f && fall_distance < 30.f) {   
				lifes -= 2; 
			}
			else if (fall_distance > 30.0f) { 
				lifes -= 3; 
			}
			is_falling = false; // Resetea el estado de caída
		}
		// Actualiza la altura de inicio de la caída
		fall_start_height = position.y;
	}
	else {
		if (!is_falling) {
			is_falling = true;
			// Marca la altura de inicio cuando comienza a caer
			fall_start_height = position.y;
		}
	}

	if (lifes <= 0) {
		//World::get_instance()->root.;
		//Entity root = World::get_instance()->root;
		//Entity newroot;
		//World::get_instance()->parseScene("data/myscene.scene", &newroot);
		//world->restart();
		//World world.delete;
		//world = new World();
		//GamePlay* game = GamePlay::get_instance();
		//game->~Stage();
		//game = new GamePlay();
		//GamePlay* game = GamePlay::get_instance();
		//game->restart();///////////etso no esta bien, no funciona.
		position = World::get_instance()->current_checkpoint; // Enviar a pantalla de inicio
		//lifes = 3;
	}

	//if (position.y > 34.f) {
	//	is_on_plataform = true;
	//}

	//Update player position
	velocity.x *= 0.5f;
	velocity.z *= 0.5f;

	model.setTranslation(position);
	model.rotate(camera_yaw, Vector3(0, 1, 0));

	if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
		printf("x = %f, y = %f, z = %f", position.x, position.y, position.z);
	}
	//printf("x = %f, y = %f, z = %f \n", position.x, position.y, position.z);

	EntityMesh::update(seconds_elapsed);
}

