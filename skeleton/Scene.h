#pragma once

#include "Particle.h"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include <string>
#include <vector>

/// <summary>
/// Clase base abstracta para las distintas escenas de la aplicación.
/// Provee la interfaz mínima que debe implementar cualquier escena:
/// inicialización, limpieza, actualización por frame y manejo de teclado.
/// Cada escena concreta define su propio contenido (actores físicos,
/// partículas, etc.) y su propia lógica de simulación.
/// </summary>
class Scene {
#pragma region Constructores y destructor
public:
	/// <summary>
	/// Construye la escena con un nombre identificador.
	/// </summary>
	/// <param name="name"> Nombre identificador de la escena (se mueve al miembro m_name) </param>
	explicit Scene(std::string name) : m_name(std::move(name)) {}

	/// <summary>
	/// Destructor virtual para permitir la destrucción correcta de las
	/// subclases.
	/// </summary>
	virtual ~Scene() = default;
#pragma endregion

#pragma region Metodos virtuales
public:
	/// <summary>
	/// Inicializa los objetos de la escena (Partículas, sistemas de partículas, etc.).
	/// Método virtual puro: cada escena concreta debe implementarlo.
	/// </summary>
	virtual void init() = 0;

	/// <summary>
	/// Libera los recursos asignados en init().
	/// La implementación por defecto borra todos
	/// los objetos UpdatableObj almacenados en 
	/// v_objects y limpia el vector.
	/// </summary>
	virtual void cleanup();

	/// <summary>
	/// Actualiza la lógica de la escena para el paso de tiempo indicado.
	/// Método virtual puro: cada escena concreta debe implementar su propia
	/// simulación.
	/// </summary>
	/// <param name="dt"> Tiempo en segundos transcurrido desde la última actualización (Definido en la aplicación/main como gFixedTimestep) </param>
	virtual void update(double dt) = 0;

	/// <summary>
	/// Manejo de la pulsación de una tecla.
	/// Recibe la tecla pulsada y el transform de la cámara para permitir
	/// manipular la cámara o realizar transformaciones en función de su posición y orientación.
	/// Método opcional: Por defecto, no hace nada, solo las
	/// escenas que lo necesiten deben sobrescribirlo.
	/// </summary>
	/// <param name="key"> Código de la tecla pulsada </param>
	/// <param name="cameraTransform"> Transform (posición y orientación) actual de la cámara </param>
	virtual void keyPress(unsigned char key, const physx::PxTransform& cameraTransform) {}
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Devuelve el nombre identificador de la escena.
	/// </summary>
	/// <returns> Referencia constante al nombre de la escena </returns>
	[[nodiscard]] const std::string& getName() const { return m_name; }
#pragma endregion

#pragma region Atributos
protected:
	/// <summary> Nombre de la escena. </summary>
	std::string m_name;

	/// <summary>
	/// Lista de objetos actualizables asociados a la escena.
	/// La escena es responsable de gestionar su ciclo de vida (crearlos en
	/// init(), actualizarlos en update() y liberarlos en cleanup()).
	/// </summary>
	std::vector<UpdatableObj*> v_objects;
#pragma endregion
};