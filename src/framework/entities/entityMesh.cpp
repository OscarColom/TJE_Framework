#include "EntityMesh.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include <algorithm>


void EntityMesh::render(Camera* camera) {

	if (!mesh) return;

	std::vector<Matrix44> models_instanced;
	std::vector<Matrix44>* final_models = &models;

	if (isInstanced) {
		for (int i = 0; i < models.size(); i++) {
			Vector3 center_world = models[i] * mesh->box.center;
			float aabb_radius = mesh->box.halfsize.length();
			if (camera->testSphereInFrustum(center_world, aabb_radius)) {
				models_instanced.push_back(models[i]);
			}
		}
	}
	else
	{
		Vector3 center_world = model * mesh->box.center;
		float aabb_radius = mesh->box.halfsize.length();

		if (!camera->testSphereInFrustum(center_world, aabb_radius+2.f)) { //preguntar, prueba suelo
			return;
		} 
	}

	if (!material.shader) {
		if (isInstanced) {
			material.shader = Shader::Get("data/shaders/instanced.vs", "data/shaders/phong.fs");
		}
		else {
			material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
		}
		
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Enable shader and pass uniforms 
	material.shader->enable();

	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

	if (material.diffuse) {
		material.shader->setUniform("u_texture", material.diffuse, 0);
	}
	
	if (!isInstanced) {
		material.shader->setUniform("u_model", getGlobalMatrix());

	}
	material.shader->setUniform("u_camera_position", camera->eye);
	material.shader->setUniform("u_light_pos", Vector3(0, 500, 0));
	material.shader->setUniform("u_time", time);

	Mesh* final_mesh = mesh;

	//The LOD (if it's far, a less quality image)
	float distance = camera->eye.distance(model.getTranslation());
	for (int i = 0; i < mesh_lods.size(); ++i) {
		sMeshLOD mesh_lod = mesh_lods[i];
		if (distance > mesh_lod.distance) {
			final_mesh = mesh_lod.mesh_lod;
		}
	}

	if (isInstanced) {
		final_mesh->renderInstanced(GL_TRIANGLES, final_models->data(), models.size());
	}
	else {
		if (isAnimated) {
			final_mesh->renderAnimated(GL_TRIANGLES, &animator.getCurrentSkeleton());
		}
		else {
			final_mesh->render(GL_TRIANGLES);
		}
	}

	// Disable shader after finishing rendering
	material.shader->disable();



	for (int i = 0; i < children.size(); i++) {
		children[i]->render(camera);
	}

	Entity::render(camera);
};


void EntityMesh::render_player(Camera* camera) {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!material.shader) {
		material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	}

	material.shader->enable();

	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	material.shader->setUniform("u_texture", material.diffuse, 0);
	material.shader->setUniform("u_model", getGlobalMatrix());
	material.shader->setUniform("u_time", time);
	material.shader->setUniform("u_camera_position", camera->eye);

	mesh->render(GL_TRIANGLES);
	material.shader->disable();
};


void EntityMesh::update(float delta_time) {

	if (isAnimated) {
		animator.update(delta_time);
	}

	for (int i = 0; i < children.size(); ++i) {
		children[i]->update(delta_time);
	}

	Entity::update(delta_time);
}

void EntityMesh::addInstance(const Matrix44& model) {
	models.push_back(model);
}

void EntityMesh::addLOD(sMeshLOD mesh_lod) {
	mesh_lods.push_back(mesh_lod);

	std::sort(mesh_lods.begin(), mesh_lods.end(), [](sMeshLOD a, sMeshLOD b) {
		return a.distance < b.distance;
	});
}