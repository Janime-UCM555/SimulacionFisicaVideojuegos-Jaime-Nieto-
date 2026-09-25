#include "Scene.h"

void Scene::cleanup() {
	// Liberar recursos asociados a la escena
	for (const auto& item : m_renderItems) {
		if (item) {
			item->release(); // Deregistra y destruye el item
		}
	}
	m_renderItems.clear();
}