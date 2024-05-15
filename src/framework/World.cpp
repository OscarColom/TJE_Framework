#include "world.h"
#include "entities/EntityMesh.h"

World::World() {

}

World::~World() {

}

//void World::render() {
//	Camera* camera = Camera::current;
//
//	for (int i = 0; i < children.size(); i++) {
//		Entity* child = children[i];
//		if (child != nullptr) {
//			if (child->Draw_requied)
//				child->render();
//			else {
//				EntityMesh* obj = (EntityMesh*)child;
//				Vector3 sphere_center = obj->model * obj->mesh->box.center;
//				float sphere_radius = obj->mesh->radius;
//
//				// Discard objects whose bounding sphere 
//				// is not inside the camera frustum
//				if (camera->testSphereInFrustum(sphere_center,
//					sphere_radius) == false && camera->eye.distance(obj->getGlobalMatrix().getTranslation()) > Draw_Distance)
//					continue;
//
//				// Render object if inside frustum
//				obj->render();
//			}
//		}
//	}
//}
//
//void World::update(float elapsed_time) {
//	for (int i = 0; i < children.size(); i++) {
//		Entity* child = children[i];
//		if (child != nullptr) {
//			child->update(elapsed_time);
//		}
//
//	}
//}