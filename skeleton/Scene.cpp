#include "Scene.h"

void Scene::cleanup() {
	//if (!m_renderItems.empty()) {
	//	// Liberar recursos asociados a la escena
	//	for (const auto& item : m_renderItems) {
	//		if (item) {
	//			item->release(); // Deregistra y destruye el item
	//		}
	//	}
	//	m_renderItems.clear();
	//}
	if (!v_objects.empty())
	{
		for(auto& particle : v_objects)
		{
			delete particle;
		}
		v_objects.clear();
	}
	
}