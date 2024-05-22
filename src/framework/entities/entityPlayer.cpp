#include "entityPlayer.h"
#include "game/game.h"
#include "framework/input.h"
#include "framework/entities/entity_collider.h"

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

	float sphere_radius = World::get_instance()->sphere_radius;
	float sphere_ground_radius = World::get_instance()->sphere_grow;
	float player_height = World::get_instance()->player_height;

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");
	Matrix44 m = model;

	shader->enable();

	//1st sphere
	{
		m.translate(0.0f, sphere_ground_radius, 0.0f);
		m.scale(sphere_ground_radius, sphere_ground_radius, sphere_ground_radius);

		shader->setUniform("u_color", Vector4(1.0f, 1.0f, 0.0f, 1.0f));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_model", m);

		mesh->render(GL_LINES);
	}

	//2nd sphere
	{
		m = model;
		m.translate(0.0f, player_height, 0.0f);
		m.scale(sphere_radius, sphere_radius, sphere_radius);

		shader->setUniform("u_color", Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_model", m);

		mesh->render(GL_LINES);
	}

	std::string str_stam = "Stamina: " + std::to_string(stamina);
	drawText(20, 20, str_stam, Vector3(1, 1, 1), 2);

	std::string str_lifes = "Lifes: " + std::to_string(lifes);
	drawText(world_width - 100, 20, str_lifes, Vector3(1, 1, 1), 2);

	shader->disable();	
}

void EntityPlayer::update(float seconds_elapsed) {
	float camera_yaw = World::get_instance()->camera_yaw;
	//float camera_yaw = Game::instance->camera_yaw;

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
		// El jugador est� esprintando y tiene resistencia
		is_sprinting = true;
		stamina -= stamina_consumption_rate * seconds_elapsed; // Reduce la resistencia
		stamina = std::max(0.0f, stamina); // Aseg�rate de que la resistencia no sea negativa

		if (stamina > 0.0f) {
			speed_mult *= 1.8f; 
		}
		else {
			is_sprinting = false;
		}
	}
	else {
		// El jugador no est� esprintando, recupera resistencia
		stamina += stamina_recovery_rate * seconds_elapsed; // Recupera resistencia
		stamina = std::min(max_stamina, stamina); // Aseg�rate de que la resistencia no supere el m�ximo
	}

	move_dir.normalize();
	move_dir *= speed_mult;

	velocity += move_dir;

	//CHECK COLLISIONS
	std::vector<sCollisionData> collisions;
	std::vector<sCollisionData> ground_collisions;

	for (auto e : World::get_instance()->root.children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec != nullptr) {
			ec->getCollisions(position + velocity * seconds_elapsed, collisions, ground_collisions, ALL);
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
		velocity.y -= 50.f * seconds_elapsed;

	}
	else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		velocity.y = 20.f;
	}

	//Env collisions
	for (const sCollisionData& collision : collisions) {
		//Move along wall when colliding
		Vector3  newDir = velocity.dot(collision.col_normal) * collision.col_normal;
		velocity.x -= newDir.x;
		velocity.y -= newDir.y;
		velocity.z -= newDir.z;
	}

	//Animation
	if (animation_state == eAnimationState::IDLE && velocity.length() > 1.f) {
		animator.playAnimation("data/final_character/animations/walk.skanim");
		animation_state = eAnimationState::WALKING;
		printf("Walking");
	} 
	
	if (animation_state == eAnimationState::RUNNING && velocity.length() < 1.f) {
			animator.playAnimation("data/final_character/animations/idle.skanim");
			animation_state = eAnimationState::IDLE;
	}
	
	if (animation_state == eAnimationState::IDLE && velocity.length() > 1.f && !is_grounded) {
		animator.playAnimation("data/final_character/animations/jump.skanim");
		animation_state = eAnimationState::JUMP;
	}
	
	if ((animation_state == eAnimationState::IDLE || animation_state == eAnimationState::WALKING) && velocity.length() > 1.f && is_sprinting) {
		animator.playAnimation("data/final_character/animations/run.skanim");
		animation_state = eAnimationState::RUNNING;
	}
	
	// Update players position
	position += velocity * seconds_elapsed;

	//Por si el jugador se cae
	if (position.y < -100 || (position.y < 23.f && is_on_plataform)) {
		lifes -= 1;
		is_on_plataform = false;
	}

	if (lifes == 0) {
		position = World::get_instance()->current_checkpoint; // Enviar a pantalla de inicio
		lifes = 3;
	}

	if (position.y > 24.f) {
		is_on_plataform = true;
	}

	//Update player position
	velocity.x *= 0.5f;
	velocity.z *= 0.5f;

	model.setTranslation(position);
	model.rotate(camera_yaw, Vector3(0, 1, 0));

	EntityMesh::update(seconds_elapsed);
}

