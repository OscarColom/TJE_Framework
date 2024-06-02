#pragma once

#include "framework/entities/entityMesh.h"

enum eButtonId{
	Undefined,
	PlayButton,
	EndButton,
	Stamina
};

class EntityUI : public EntityMesh {
public:
	EntityUI(Vector2 size, const Material& material);
	EntityUI(Vector2 position, Vector2 size, const Material& material, eButtonId buttonId = Undefined );
	~EntityUI() {};

	Vector2 position;
	Vector2 size;

	bool visible = true; 

	Camera* camera2d;

	Mesh quad;

	eButtonId buttonId;
	Material material;

	void render(Camera* camera2d);
	void render_stamina(Camera* camera2d, float current_stamina);
	void update(float seconds_elapsed);

};