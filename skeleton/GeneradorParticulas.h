#pragma once
#include "Particle.h"
#include <memory>
#include <random>
#include <vector>

/// <summary>
/// Generador abstracto de partículas. Define la interfaz común para todos los
/// generadores concretos y centraliza la gestión de la tasa de emisión, los
/// rangos de dispersión, el estado activo/inactivo y el motor de números
/// aleatorios. Las subclases deben implementar cómo se generan la posición y
/// la velocidad inicial de cada partícula según su distribución.
/// </summary>
class GeneradorParticulas {
#pragma region Metodos virtuales puros
protected:
	/// <summary>
	/// Genera la posición inicial de una nueva partícula.
	/// </summary>
	/// <returns> Vector con la posición generada según la distribución de la subclase </returns>
	virtual Vector3D generarPosicion() = 0;

	/// <summary>
	/// Genera la velocidad inicial de una nueva partícula.
	/// </summary>
	/// <returns> Vector con la velocidad generada según la distribución de la subclase </returns>
	virtual Vector3D generarVelocidad() = 0;
#pragma endregion

#pragma region Atributos
protected:
	/// <summary> Partícula prototipo usada como modelo para las nuevas partículas generadas. Nunca será visualizada (visible_ = false) </summary>
	std::unique_ptr<Particle> modelo;

	/// <summary> Tasa de generación de partículas por segundo. </summary>
	float rate;

	/// <summary> Rango de dispersión aplicado a la posición de las partículas. </summary>
	Vector3D posSpread;

	/// <summary> Rango de dispersión aplicado a la velocidad de las partículas. </summary>
	Vector3D velSpread;

	/// <summary> Motor de números aleatorios (Mersenne Twister) compartido por los métodos auxiliares. </summary>
	std::mt19937 mt;

private:
	/// <summary> Indica si el generador está emitiendo partículas. </summary>
	bool active;

	/// <summary>
	/// Acumula la fracción de partículas pendientes de emitir entre frames,
	/// para que la tasa media de emisión sea consistente aunque dt no sea múltiplo
	/// del periodo entre partículas.
	/// </summary>
	double acumulador;
#pragma endregion

#pragma region Constructores y destructor
public:
	/// <summary>
	/// Construye un generador de partículas a partir de una descripción de
	/// partícula, la tasa de emisión y los rangos de dispersión de posición y velocidad.
	/// </summary>
	/// <param name="d"> Descripción de la partícula usada como modelo </param>
	/// <param name="r"> Tasa de generación de partículas por segundo </param>
	/// <param name="posS"> Rango de dispersión de la posición </param>
	/// <param name="velS"> Rango de dispersión de la velocidad </param>
	GeneradorParticulas(ParticleDesc d, float r, Vector3D posS, Vector3D velS);

	/// <summary>
	/// Destructor virtual para permitir la destrucción correcta de las subclases
	/// a través de un puntero a la clase base.
	/// </summary>
	virtual ~GeneradorParticulas() = default;
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Genera las partículas correspondientes al tiempo transcurrido, según la
	/// tasa configurada. El acumulador interno permite emitir fracciones de
	/// partícula entre frames para que la tasa media sea consistente.
	/// No genera nada si el generador está inactivo.
	/// </summary>
	/// <param name="dt"> Tiempo transcurrido desde la última llamada, en segundos (Definido en la aplicación/main como gFixedTimestep) </param>
	/// <returns> Vector con punteros a las partículas recién creadas (propiedad del llamador) </returns>
	std::vector<Particle*> generarParticulas(double dt);

	/// <summary>
	/// Activa o desactiva el generador.
	/// </summary>
	/// <param name="a"> true para activarlo, false para desactivarlo </param>
	void setActive(bool a) { active = a; }

	/// <summary>
	/// Alterna el estado activo del generador.
	/// </summary>
	void toggleActive() { active = !active; }

	/// <summary>
	/// Comprueba si el generador está activo.
	/// </summary>
	/// <returns> true si el generador está emitiendo partículas, false en caso contrario </returns>
	bool isActive() const { return active; }
#pragma endregion
};

/// <summary>
/// Generador de partículas que utiliza distribuciones normales (gaussianas)
/// para calcular la posición y la velocidad inicial de cada partícula.
/// </summary>
class GeneradorNormal : public GeneradorParticulas {
#pragma region Constructores
public:
	/// <summary>
	/// Construye un generador de partículas con distribución normal.
	/// </summary>
	/// <param name="d"> Descripción de la partícula usada como modelo </param>
	/// <param name="r"> Tasa de generación de partículas por segundo </param>
	/// <param name="posS"> Rango de dispersión de la posición </param>
	/// <param name="velS"> Rango de dispersión de la velocidad </param>
	GeneradorNormal(ParticleDesc d, float r, Vector3D posS, Vector3D velS);
#pragma endregion

#pragma region Metodos
private:
	/// <summary>
	/// Genera un número aleatorio con distribución normal (gaussiana).
	/// Método auxiliar pensado para ser usado por las implementaciones concretas
	/// de generarPosicion() y generarVelocidad().
	/// </summary>
	/// <param name="media"> Media de la distribución </param>
	/// <param name="desviacion"> Desviación estándar de la distribución </param>
	/// <returns> Valor aleatorio con distribución normal </returns>
	float normal(float media, float desviacion);

	/// <summary>
	/// Genera la posición inicial de una partícula usando una distribución normal
	/// centrada en el origen y con dispersión definida por posSpread.
	/// </summary>
	/// <returns> Vector con la posición generada </returns>
	Vector3D generarPosicion() override;

	/// <summary>
	/// Genera la velocidad inicial de una partícula usando una distribución normal
	/// centrada en el origen y con dispersión definida por velSpread.
	/// </summary>
	/// <returns> Vector con la velocidad generada </returns>
	Vector3D generarVelocidad() override;
#pragma endregion
};

/// <summary>
/// Generador de partículas que utiliza distribuciones uniformes para calcular
/// la posición y la velocidad inicial de cada partícula.
/// </summary>
class GeneradorUniforme : public GeneradorParticulas {
#pragma region Constructores
public:
	/// <summary>
	/// Construye un generador de partículas con distribución uniforme.
	/// </summary>
	/// <param name="d"> Descripción de la partícula usada como modelo </param>
	/// <param name="r"> Tasa de generación de partículas por segundo </param>
	/// <param name="posS"> Rango de dispersión de la posición </param>
	/// <param name="velS"> Rango de dispersión de la velocidad </param>
	GeneradorUniforme(ParticleDesc d, float r, Vector3D posS, Vector3D velS);
#pragma endregion

#pragma region Metodos
private:
	/// <summary>
	/// Genera un número aleatorio con distribución uniforme entre min y max.
	/// Método auxiliar pensado para ser usado por las implementaciones concretas
	/// de generarPosicion() y generarVelocidad().
	/// </summary>
	/// <param name="min"> Límite inferior del intervalo </param>
	/// <param name="max"> Límite superior del intervalo </param>
	/// <returns> Valor aleatorio con distribución uniforme en [min, max] </returns>
	float uniforme(float min, float max);

	/// <summary>
	/// Genera la posición inicial de una partícula usando una distribución uniforme
	/// dentro del rango definido por posSpread.
	/// </summary>
	/// <returns> Vector con la posición generada </returns>
	Vector3D generarPosicion() override;

	/// <summary>
	/// Genera la velocidad inicial de una partícula usando una distribución uniforme
	/// dentro del rango definido por velSpread.
	/// </summary>
	/// <returns> Vector con la velocidad generada </returns>
	Vector3D generarVelocidad() override;
#pragma endregion
};