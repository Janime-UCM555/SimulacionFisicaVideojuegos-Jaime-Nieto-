#pragma once
#include "RenderUtils.hpp"
#include "UpdatableObj.h"
#include "Vector3D.h"

/// <summary>
/// Vector de gravedad estándar usado por las partículas (apunta hacia -Y).
/// Se define como constante global para evitar recalcularlo en cada integración.
/// </summary>
static constexpr Vector3D GRAVITY{ 0.0f, -9.8f, 0.0f };

/// <summary>
/// Estructura de descripción de una partícula. Agrupa todos los parámetros
/// iniciales (posición, velocidad, aceleración, masa, amortiguación, tiempo de
/// vida, visibilidad, color y forma) para facilitar la creación de partículas y permitir
/// configurarlos encadenando llamadas.
/// </summary>
struct ParticleDesc {
#pragma region Atributos
	Vector3D pos_{ 0, 0, 0 };               ///< Posición inicial de la partícula.
	Vector3D vel_{ 0, 0, 0 };               ///< Velocidad inicial de la partícula.
	Vector3D acc_{ 0, 0, 0 };               ///< Aceleración aplicada a la partícula (normalmente gravedad u otra fuerza externa).
	float    mass_ = 1.0f;                  ///< Masa de la partícula.
	float    damping_ = 0.99f;              ///< Factor de amortiguación aplicado a la velocidad en cada integración.
	double   lifeTime_ = -1.0;              ///< Tiempo de vida en segundos; valor negativo indica que no caduca.
	bool     visible_ = true;               ///< Indica si la partícula debe renderizarse. En caso contrario, se simula pero no crea un RenderItem.
	Vector4  color_{ 1.0f, 1.0f, 1.0f, 1.0f }; ///< Color RGBA de la partícula.
	physx::PxGeometryHolder shape_ = physx::PxSphereGeometry(2.0f); ///< Forma geométrica usada para renderizar la partícula.
#pragma endregion

#pragma region Metodos (API fluida)
	/// <summary> Establece la posición inicial de la partícula. </summary>
	/// <param name="v"> Nueva posición inicial </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& position(const Vector3D& v) { pos_ = v; return *this; }

	/// <summary> Establece la velocidad inicial de la partícula. </summary>
	/// <param name="v"> Nueva velocidad inicial </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& velocity(const Vector3D& v) { vel_ = v; return *this; }

	/// <summary> Establece la aceleración aplicada a la partícula. </summary>
	/// <param name="v"> Nueva aceleración </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& acceleration(const Vector3D& v) { acc_ = v; return *this; }

	/// <summary> Establece la masa de la partícula. </summary>
	/// <param name="m"> Nueva masa </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& mass(float m) { mass_ = m; return *this; }

	/// <summary> Establece el factor de amortiguación de la velocidad. </summary>
	/// <param name="d"> Nuevo factor de amortiguación </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& damping(float d) { damping_ = d; return *this; }

	/// <summary> Establece el tiempo de vida de la partícula. </summary>
	/// <param name="s"> Tiempo de vida en segundos (negativo = sin caducidad) </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& lifeTime(double s) { lifeTime_ = s; return *this; }

	/// <summary> Establece si la partícula debe renderizarse. </summary>
	/// <param name="v"> true para hacerla visible, false para ocultarla </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& visible(bool v) { visible_ = v; return *this; }

	/// <summary> Establece el color RGBA de la partícula. </summary>
	/// <param name="c"> Nuevo color </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	ParticleDesc& color(const Vector4& c) { color_ = c; return *this; }

	/// <summary>
	/// Establece la forma geométrica de la partícula a partir de cualquier tipo
	/// de geometría de PhysX compatible con PxGeometryHolder (esfera, caja, cápsula, etc.).
	/// </summary>
	/// <typeparam name="Geometry"> Tipo de geometría de PhysX </typeparam>
	/// <param name="g"> Geometría a usar para renderizar la partícula </param>
	/// <returns> Referencia a esta descripción para encadenar llamadas </returns>
	template <typename Geometry>
	ParticleDesc& shape(const Geometry& g) { shape_ = physx::PxGeometryHolder(g); return *this; }
#pragma endregion
};

/// <summary>
/// Representa una partícula. Hereda de UpdatableObj para actualizarse 
/// de forma independiente en el bucle de actualización global.
/// Durante el bucle actualiza su tiempo de vida y recalcula su estado físico
/// (posición, velocidad, aceleración) en funcion del tiempo transcurrido y 
/// de los parámetros de simulación mediante un integrador con la fórmula de Euler semi-implícita.
/// Además de encapsular sus parámetros físicos tambien sus parámetros de simulación (masa, damping,
/// tiempo de vida) y su representación visual mediante un RenderItem.
/// La clase no tiene constructores de copia ni operadores de asignación, 
/// para evitar comportamientos no deseados como la copia de la referencia a un RenderItem.
/// </summary>
class Particle : public UpdatableObj
{
#pragma region Constructores y destructor
public:
	/// <summary>
	/// Construye una partícula a partir de una descripción (ParticleDesc),
	/// copiando todos sus parámetros iniciales.
	/// </summary>
	/// <param name="des"> Descripción con los parámetros iniciales de la partícula </param>
	explicit Particle(const ParticleDesc& des);

	/// <summary>
	/// Construye una partícula indicando explícitamente sus parámetros físicos
	/// y su representación visual. Se mantiene por si en algún momento es necesario crear una partícula sin usar una descripción. (No sé qué caso sería más cómodo con la descripción)
	/// </summary>
	/// <param name="pos"> Posición inicial </param>
	/// <param name="vel"> Velocidad inicial </param>
	/// <param name="acc"> Aceleración aplicada (por defecto nula) </param>
	/// <param name="mass"> Masa de la partícula (por defecto 1.0 KG) </param>
	/// <param name="damping"> Factor de amortiguación de la velocidad (por defecto 0.99) </param>
	/// <param name="color"> Color RGBA (por defecto blanco opaco) </param>
	/// <param name="shape"> Forma geométrica de renderizado (por defecto una esfera de radio 2.0) </param>
	Particle(Vector3D pos, Vector3D vel, Vector3D acc = Vector3D(0, 0, 0), float mass = 1.0f, float damping = 0.99f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), physx::PxSphereGeometry shape = physx::PxSphereGeometry(2.0f));

	/// <summary>
	/// Destructor. Libera los recursos asociados a la partícula (RenderItem, etc.).
	/// </summary>
	virtual ~Particle();

	/// <summary> Copia deshabilitada: las partículas no se pueden copiar directamente. </summary>
	Particle(const Particle&) = delete;

	/// <summary> Asignación por copia deshabilitada: las partículas no se pueden copiar directamente. </summary>
	Particle& operator=(const Particle&) = delete;
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Crea una copia independiente de esta partícula, con su propio estado y
	/// recursos asociados. Es el mecanismo explícito de duplicación.
	/// </summary>
	/// <returns> Puntero a la nueva partícula clonada </returns>
	Particle* clone() const;

	/// <summary> Obtiene una referencia constante a la posición actual de la partícula como Vector3D. </summary>
	/// <returns> Referencia a la posición actual </returns>
	const Vector3D& getPosition() const { return Vector3D(transform.p); }

	/// <summary> Obtiene una referencia constante a la velocidad actual de la partícula. </summary>
	/// <returns> Referencia a la velocidad actual </returns>
	const Vector3D& getVelocity() const { return vel; }

	/// <summary> Establece la posición de la partícula. </summary>
	/// <param name="p"> Nueva posición </param>
	void setPosition(const Vector3D& p) { transform.p = physx::PxVec3(p.x, p.y, p.z); }

	/// <summary> Establece la velocidad de la partícula. </summary>
	/// <param name="v"> Nueva velocidad </param>
	void setVelocity(const Vector3D& v) { vel = v; }

	/// <summary> Establece el tiempo de vida de la partícula. </summary>
	/// <param name="time"> Nuevo tiempo de vida en segundos (negativo = sin caducidad) </param>
	void setLifeTime(double time) { lifeTime = time; }

	/// <summary> Reinicia la edad de la partícula a cero (útil al reciclarla. 
	/// En principio para una posible implementación en los sistema de gestión de partículas). </summary>
	void resetAge() { age = 0.0; }

	/// <summary>
	/// Actualiza el estado de la partícula para el paso de tiempo dado.
	/// Implementación del método virtual heredado de UpdatableObj: incrementa
	/// la edad e integra la física.
	/// </summary>
	/// <param name="dt"> Tiempo transcurrido desde la última actualización, 
	/// en segundos (Definido en la aplicación/main como gFixedTimestep) el paso debe ser constante </param>
	void update(double dt) override;

	/// <summary>
	/// Integra el estado físico de la partícula mediante la fórmula de Euler semi-implícita
	/// durante el tiempo indicado (aplica aceleración, amortiguación y actualiza posición/velocidad).
	/// </summary>
	/// <param name="t">Intervalo de tiempo a integrar, en segundos</param>
	void integrate(double t);

	/// <summary>
	/// Comprueba si la partícula ha superado su tiempo de vida.
	/// Una partícula con lifeTime <= 0 nunca caduca.
	/// </summary>
	/// <returns> true si la partícula ha expirado, false en caso contrario </returns>
	bool isExpired() const { return lifeTime > 0.0 && age >= lifeTime; }
#pragma endregion

#pragma region Atributos
protected:
	/// <summary> Masa de la partícula. </summary>
	float mass;

	/// <summary> Velocidad actual de la partícula. </summary>
	Vector3D vel;

	/// <summary> Aceleración aplicada a la partícula. </summary>
	Vector3D acc;

	/// <summary> Transform (posición y orientación) usada por PhysX y el render. </summary>
	physx::PxTransform transform;

private:
	/// <summary> Descripción original con la que se creó la partícula (usado en clone()). </summary>
	ParticleDesc desc;

	/// <summary> Tiempo de vida total en segundos (negativo = sin caducidad). </summary>
	double lifeTime;

	/// <summary> Edad actual de la partícula en segundos. </summary>
	double age;

	/// <summary> Factor de amortiguación aplicado a la velocidad en cada integración. </summary>
	float damping;

	/// <summary> Elemento de render asociado a la partícula (propiedad de la partícula). </summary>
	RenderItem* renderItem;
#pragma endregion
};