#include "framework/entities/entityUI.h"
#include "framework/input.h"
#include "game/world.h"
#include "game/game.h"
#include "game/stage.h"

#include <iostream>
#include <fstream>


EntityUI::EntityUI(Vector2 size, const Material& material) {
	this->size = size;
	this->material = material;

}

EntityUI::EntityUI(Vector2 position, Vector2 size, const Material& material, eButtonId buttonId) {
	this->position = position;
	this->size = size;
	this->material = material;
	this->buttonId = buttonId;

	quad.createQuad(position.x, position.y, size.x, size.y, false);

}
void EntityUI::render(Camera* camera2d) {

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (material.shader) {
		printf("g");
	}
	material.shader->enable();

	World* world = World::get_instance();
	//Matrix44 viewProj = camera2d->viewprojection_matrix;

	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_viewprojection", camera2d->viewprojection_matrix);
	material.shader->setUniform("u_model", model); //sino getGlobalMatrix()

	material.shader->disable();

	//quad.render(GL_TRIANGLES);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	Entity::render(camera2d);


}

void EntityUI::update(float seconds_elapsed) {
	Vector2 mouse_pos = Input::mouse_position;

	if (buttonId != Undefined &&
		mouse_pos.x > (position.x - size.x * 0.5f) &&
		mouse_pos.x < (position.x - size.x * 0.5f) &&
		mouse_pos.y >(position.y - size.y * 0.5f) &&
		mouse_pos.y < (position.y - size.y * 0.5f)) {

		material.color = Vector4::RED;

		if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			World* world = World::get_instance();
			world->current_stage->onButtonPressed(buttonId); //ONbUTTONPRESSED
		}
	}
	else {
		material.color = Vector4::RED;

	}
	Entity::update(seconds_elapsed);

}