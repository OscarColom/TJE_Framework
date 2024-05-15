#pragma once
#include "includes.h"
#include "framework.h"
#include "entities/Entity.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include <string>

#include "utils.h"
#include "entities/EntityMesh.h"

#include <fstream>
#include <map>
#include <typeinfo>

#include <iostream>
#include <cstdlib> 
#include <ctime>

class World {
public:
	World() ; 			// Constructor
	virtual ~World(); 	// Destructor

	// Some attributes 
	std::string name;
	Matrix44 model;

	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;

	// Methods that should be overwritten
	// by derived classes 
	virtual void render() {};
	virtual void update(float elapsed_time) {};

	// Pointers to children
	std::vector<Entity*> children;

	void addChild(Entity* child);
	void removeChild(Entity* child);

	// Get transform in world coordinates
	Matrix44 getGlobalMatrix();
};