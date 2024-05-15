#pragma once
#include "framework/includes.h"
#include "framework/entities/entity.h"
#include "graphics/material.h"
#include "graphics/texture.h"

class Texture;
class Mesh;
class Shader;

struct sMeshLOD {
	Mesh* mesh_lod = nullptr;
	float distance = 0.0f;
};

class EntityMesh : public Entity {

	std::vector<sMeshLOD> mesh_lods;

public:

	EntityMesh() {};
	EntityMesh(Mesh* mesh, const Material& material, const std::string& name) : mesh(mesh), material(material), name(name) {};
	EntityMesh(Mesh* mesh, const Material& material) : mesh(mesh), material(material) {};
	~EntityMesh() {};


	// Attributes of the derived class  
	Mesh* mesh = nullptr;
	Material material;
	std::string name;
	//Texture* texture = nullptr;
	//Shader* shader = nullptr;
	//Vector4 color;

	//Instancing
	bool isInstanced = false;
	std::vector<Matrix44> models;

	// Methods overwritten from base class
	void render(Camera* camera);
	void render_player(Camera* camera);
	void update(float elapsed_time);

	void addInstance(const Matrix44& model);
	void addLOD(sMeshLOD mesh_lod);
};

