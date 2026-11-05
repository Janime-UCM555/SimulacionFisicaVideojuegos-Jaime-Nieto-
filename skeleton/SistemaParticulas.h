#pragma once
#include "GeneradorParticulas.h"
#include "GeneradorFuerzas.h"

/// <summary>
/// Sistema que agrupa y coordina varios generadores de partículas dentro de un
/// volumen de simulación delimitado por un cubo (definido por dos esquinas).
/// Hereda de UpdatableObj para integrarse en el bucle de actualización global.
/// Es responsable de:
///  - Mantener la colección de generadores (con propiedad exclusiva sobre ellos).
///  - Mantener la colección de partículas vivas emitidas por esos generadores.
///  - Actualizar la emisión y descartar partículas que caducan o salen del volumen (El volumen es un Cubo).
/// </summary>
class SistemaParticulas : public UpdatableObj
{
#pragma region Metodos auxiliares
private:
	/// <summary>
	/// Comprueba si una posición cae fuera del volumen de simulación definido
	/// por las esquinas eII (inferior izquierda) y eSD (superior derecha).
	/// Se usa para descartar partículas que abandonan el área de colisión.
	/// </summary>
	/// <param name="pos"> Posición a comprobar </param>
	/// <returns> true si la posición está fuera del cubo, false si está dentro </returns>
	bool fueraDeRango(const Vector3D& pos) const {
		return (pos.x < eII.x || pos.x > eSD.x ||
			pos.y < eII.y || pos.y > eSD.y ||
			pos.z < eII.z || pos.z > eSD.z);
	}
#pragma endregion

#pragma region Atributos
private:
	/// <summary>
	/// Colección de generadores de partículas asociados al sistema.
	/// Se almacenan como unique_ptr: el sistema tiene la propiedad exclusiva
	/// de los generadores y los destruye al ser destruido.
	/// </summary>
	std::vector<std::unique_ptr<GeneradorParticulas>> generadores;

	/// <summary>
	/// Partículas actualmente vivas emitidas por los generadores.
	/// Se almacenan como punteros crudos; el sistema es responsable de
	/// liberarlas cuando caducan, salen del volumen o el propio sistema se destruye.
	/// </summary>
	std::vector<Particle*> particulas;

	/// <summary>
	/// Esquina inferior izquierda del cubo que define el área de colisión/simulación.
	/// Junto con eSD delimita el volumen dentro del cual se mantienen las partículas.
	/// </summary>
	Vector3D eII;

	/// <summary>
	/// Esquina superior derecha del cubo que define el área de colisión/simulación.
	/// Junto con eII delimita el volumen dentro del cual se mantienen las partículas.
	/// </summary>
	Vector3D eSD;

	/// <summary>
	/// Vector de punteros a objetos GeneradorFuerzas, inicialmente vacío.
	/// Contiene las fuerzasPropias que se aplican a todas las partículas del sistema.
	/// </summary>
	std::vector<GeneradorFuerzas*> fuerzas;
#pragma endregion

#pragma region Constructores y destructor
public:
	/// <summary>
	/// Construye un sistema de partículas con un volumen de simulación delimitado
	/// por las esquinas indicadas. Por defecto, el volumen es el cubo centrado en
	/// el origen de lado 200 ([-100, 100] en cada eje).
	/// </summary>
	/// <param name="espacioInferiorIzquierdo"> Esquina inferior izquierda del volumen (por defecto (-100,-100,-100)) </param>
	/// <param name="espacioSuperiorDerecho"> Esquina superior derecha del volumen (por defecto (100,100,100)) </param>
	SistemaParticulas(Vector3D espacioInferiorIzquierdo = Vector3D(-100, -100, -100), Vector3D espacioSuperiorDerecho = Vector3D(100, 100, 100))
		: eII(espacioInferiorIzquierdo), eSD(espacioSuperiorDerecho) {}

	/// <summary>
	/// Destructor. Libera los generadores (vía unique_ptr) y las partículas
	/// almacenadas en el sistema.
	/// </summary>
	~SistemaParticulas();
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Añade un generador al sistema, tomando su propiedad.
	/// </summary>
	/// <param name="g"> Generador a añadir (se mueve al sistema) </param>
	void addGenerador(std::unique_ptr<GeneradorParticulas> g);

	/// <summary>
	/// Añade una partícula al sistema, tomando su propiedad.
	/// </summary>
	/// <param name="p"> Puntero a la partícula a añadir </param>
	void addParticula(Particle* p);

	/// <summary>
	/// Activa o desactiva un generador concreto por su índice.
	/// </summary>
	/// <param name="index"> Índice del generador en la colección interna </param>
	/// <param name="activo"> true para activarlo, false para desactivarlo </param>
	void setGeneradorActivo(size_t index, bool activo);

	/// <summary>
	/// Alterna el estado activo/inactivo de un generador concreto por su índice.
	/// </summary>
	/// <param name="index"> Índice del generador en la colección interna </param>
	void toggleGeneradorActivo(size_t index);

	// NOTA: Tal vez se podría añadir alguna esctructura de datos para acceder a los generadores por nombre o ID, en lugar de solo por índice, tal como las escenas.

	/// <summary>
	/// Activa o desactiva todos los generadores del sistema.
	/// </summary>
	/// <param name="activo"> true para activarlos todos, false para desactivarlos todos </param>
	void setGeneradorActivoTodos(bool activo);

	/// <summary>
	/// Alterna el estado activo/inactivo de todos los generadores del sistema.
	/// </summary>
	void toggleGeneradorActivoTodos();

	/// <summary>
	/// Actualiza el sistema para el paso de tiempo indicado: actualiza cada
	/// generador, integra las partículas vivas, elimina las caducadas o fuera
	/// del volumen de simulación, y añade las nuevas partículas emitidas.
	/// Implementación del método virtual heredado de UpdatableObj.
	/// </summary>
	/// <param name="dt"> Tiempo transcurrido desde la última actualización, en segundos </param>
	void update(double dt) override;

	/// <summary>
	/// Añade un generador de fuerzasPropias a la colección de fuerzas.
	/// </summary>
	/// <param name="f"> Puntero al generador de fuerzasPropias que se va a añadir. </param>
	void addForce(GeneradorFuerzas* f) { fuerzas.push_back(f); }

	/// <summary>
	/// Elimina de la colección todas las apariciones de una fuerza.
	/// </summary>
	/// <param name="f"> Puntero a la fuerza que se va a eliminar. </param>
	void removeForce(GeneradorFuerzas* f) {
		auto it = std::remove(fuerzas.begin(), fuerzas.end(), f);
		fuerzas.erase(it, fuerzas.end());
	}
#pragma endregion
};