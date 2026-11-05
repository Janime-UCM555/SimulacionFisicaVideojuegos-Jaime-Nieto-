#pragma once
#include "Particle.h"

/// <summary>
/// Proyectil: especialización de Particle orientada a escalar objetos físicos de forma realista.
/// Hereda de Particle las propiedades físicas y de renderizado que se usarán en la simulación, 
/// pero mantiene su propia masa y velocidad reales que se pueden escalar para una mejor visualización
/// sin afectar a los resultados de la simulación de PhysX.
/// </summary>
class Proyectile : public Particle
{
#pragma region Atributos
private:
	/// <summary> Aceleración de la gravedad usada por la simulación de este proyectil (por defecto GRAVITY). </summary>
	Vector3D simGravity = GRAVITY;

	/// <summary> Masa real del proyectil, independiente de la masa simulada heredada de Particle. </summary>
	float realMass;

	/// <summary> Velocidad real del proyectil, independiente de la velocidad simulada (vel) heredada de Particle. </summary>
	Vector3D realVelocity;
#pragma endregion

#pragma region Constructores y destructor
public:
	/// <summary>
	///	Construye un proyectil a partir de una descripción de partícula, que se usará como modelo para la simulación y el renderizado.
	/// Por defecto, la gravedad simulada se ajusta a GRAVITY y la masa y velocidad reales se toman de la descripción.
	/// Así que se asume que no se escala al contruirlo.
	/// </summary>
	/// <param name="desc"> Descripción de la partícula </param>
	Proyectile(ParticleDesc desc);

	/// <summary>
	/// Construye un proyectil con los parámetros físicos iniciales y su
	/// representación visual. Por defecto usa amortiguación 1.0 (sin pérdida de
	/// velocidad), a diferencia de Particle que usa 0.99. Esto es para evitar 
	/// problemas en el reescalado de la velocidad, ya que la amortiguación de
	/// Particle se aplica sobre la velocidad simulada (vel) y no sobre la velocidad real (realVelocity).
	/// </summary>
	/// <param name="pos"> Posición inicial </param>
	/// <param name="vel"> Velocidad inicial </param>
	/// <param name="acc"> Aceleración aplicada (por defecto nula) </param>
	/// <param name="mass"> Masa del proyectil </param>
	/// <param name="damping"> Factor de amortiguación de la velocidad </param>
	/// <param name="color"> Color RGBA </param>
	/// <param name="shape"> Forma geométrica de renderizado </param>
	Proyectile(Vector3D pos, Vector3D vel, Vector3D acc = Vector3D(0, 0, 0), float mass = 1.0f, float damping = 1.0f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), physx::PxSphereGeometry shape = physx::PxSphereGeometry(2.0f));

	/// <summary> Destructor por defecto (no hay recursos propios que liberar). </summary>
	~Proyectile() = default;
#pragma endregion

#pragma region Metodos
public:
	// void ScaleMass(float mass);
	// NOTA: método comentado porque actualmente no se usa. Fue creado por el enunciado anterior. 
	// En caso de necesario se puede volver a activar.
	// Escalaba la masa del proyectil a un valor absoluto validando que fuese positivo, y ajustaba la gravedad simulada en consecuencia.

	/*void ScaleMassByFactor(float factor) {
		if (factor <= 0.0f) {
			throw std::invalid_argument("Factor must be greater than zero.");
		}
		float newMass = mass * factor;
		ScaleMass(newMass);
	}*/
	// NOTA: método comentado porque actualmente no se usa. Fue creado por el enunciado anterior. 
	// En caso de necesario se puede volver a activar.
	// Escalaba la masa del proyectil con un factor sobre la masa simulada, y ajustaba la gravedad simulada en consecuencia.

	/// <summary>
	/// Ajusta la velocidad del proyectil al valor indicado, 
	/// recalculando la masa simulada y la gravedad simulada para mantener la energía cinética constante.
	/// </summary>
	/// <param name="velocity"> Nueva velocidad a asignar </param>
	void ScaleVelocity(Vector3D velocity);

	/// <summary>
	/// Escala la velocidad actual del proyectil multiplicándola por un factor.
	/// Valida que el factor no sea cero y delega el ajuste final
	/// en ScaleVelocity().
	/// </summary>
	/// <param name="factor"> Factor de escala a aplicar sobre la velocidad (debe ser != 0) </param>
	/// <exception cref="std::invalid_argument"> Si factor es igual que cero </exception>
	void ScaleVelocityByFactor(float factor) {
		if (factor == 0.0f) {
			throw std::invalid_argument("Factor must be different from zero.");
		}
		Vector3D newVelocity = vel * factor;
		ScaleVelocity(newVelocity);
	}
#pragma endregion
};