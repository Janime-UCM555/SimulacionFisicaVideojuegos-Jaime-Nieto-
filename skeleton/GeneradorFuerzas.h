#pragma once
#include "Particle.h"
#include "UpdatableObj.h"
#include <algorithm>
#include <cmath>

/// <summary>
/// Clase base abstracta para generadores de fuerzas aplicables a partículas.
/// Hereda de UpdatableObj para poder mantener un tiempo interno (elapsedTime)
/// que los generadores concretos pueden usar para fuerzas dependientes del
/// tiempo (por ejemplo, explosiones que decaen). Cada subclase implementa
/// getForce() devolviendo el vector de fuerza que aplica a una partícula
/// concreta; la partícula se encarga de acumular esas fuerzas y de integrarlas.
/// El estado activo/inactivo permite habilitar o deshabilitar la fuerza sin
/// destruir el generador.
/// </summary>
class GeneradorFuerzas : public UpdatableObj
{
#pragma region Atributos
protected:
	/// <summary> Indica si el generador está aplicando fuerza actualmente. </summary>
	bool active = true;

	/// <summary>
	/// Tiempo acumulado desde que el generador se activó (o desde el último reset).
	/// Lo usan las fuerzas dependientes del tiempo, como las explosiones.
	/// </summary>
	float elapsedTime = 0.0f;
#pragma endregion

#pragma region Constructores y destructor
public:
	/// <summary> Destructor virtual para permitir la destrucción correcta de las subclases. </summary>
	virtual ~GeneradorFuerzas() = default;
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Reinicia el tiempo acumulado del generador a cero.
	/// Útil para reiniciar efectos temporales (explosiones, pulsos, etc.).
	/// </summary>
	void reset() { elapsedTime = 0.0f; }

	/// <summary>
	/// Activa o desactiva el generador. Al activarlo desde un estado inactivo,
	/// reinicia el tiempo acumulado para que los efectos temporales empiecen
	/// desde cero.
	/// </summary>
	/// <param name="a"> true para activarlo, false para desactivarlo </param>
	void setActive(bool a) { if (a && !active) { reset(); } active = a; }

	/// <summary> Alterna el estado activo/inactivo del generador. </summary>
	void toggleActive() { setActive(!active); }

	/// <summary> Comprueba si el generador está aplicando fuerza actualmente. </summary>
	/// <returns> true si está activo, false en caso contrario </returns>
	bool isActive() const { return active; }

	/// <summary>
	/// Calcula la fuerza que este generador aplica sobre una partícula concreta.
	/// Método virtual puro: cada tipo de fuerza define su propia fórmula.
	/// </summary>
	/// <param name="p"> Partícula sobre la que calcular la fuerza </param>
	/// <returns> Vector de fuerza a aplicar a la partícula </returns>
	virtual Vector3D getForce(const Particle& p) const = 0;

	/// <summary>
	/// Actualiza el tiempo acumulado del generador si está activo.
	/// Implementación del método virtual heredado de UpdatableObj.
	/// </summary>
	/// <param name="dt"> Tiempo transcurrido desde la última actualización, en segundos </param>
	void update(double dt) override { if (active) { elapsedTime += dt; } }
#pragma endregion
};

/// <summary>
/// Fuerza de gravedad constante. Aplica a cada partícula una fuerza igual a
/// su masa multiplicada por el vector de gravedad (F = m·g), de modo que la
/// aceleración resultante es independiente de la masa, como en la gravedad real.
/// </summary>
class GravityForce : public GeneradorFuerzas
{
#pragma region Atributos
private:
	/// <summary> Vector de gravedad aplicado a las partículas. </summary>
	Vector3D gravity;
#pragma endregion

#pragma region Constructores
public:
	/// <summary>
	/// Construye una fuerza de gravedad con el vector indicado.
	/// </summary>
	/// <param name="g"> Vector de gravedad (por defecto GRAVITY)  </param>
	explicit GravityForce(const Vector3D& g = GRAVITY) : gravity(g) {}
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Devuelve la fuerza de gravedad aplicada a la partícula: gravity * masa.
	/// </summary>
	/// <param name="p"> Partícula sobre la que calcular la fuerza </param>
	/// <returns> Vector de fuerza gravitatoria </returns>
	Vector3D getForce(const Particle& p) const override { return gravity * p.getMass(); }
#pragma endregion
};

/// <summary>
/// Fuerza de viento aplicada dentro de una zona prismática (caja) definida por
/// un centro y una altura (semiejes). La fuerza depende de la diferencia entre
/// la velocidad del viento en ese punto y la velocidad de la partícula, con un
/// término lineal (k1) y un término cuadrático (k2) para simular arrastre
/// aerodinámico.
/// </summary>
class WindForce : public GeneradorFuerzas
{
#pragma region Metodos auxiliares
protected:
	/// <summary>
	/// Devuelve la velocidad del viento en la posición indicada.
	/// La implementación base devuelve una velocidad de viento constante;
	/// las subclases pueden sobrescribirla para vientos variables en el espacio.
	/// </summary>
	/// <param name="p"> Posición donde calcular la velocidad del viento </param>
	/// <returns> Vector de velocidad del viento </returns>
	virtual Vector3D getWindVelocity(const Vector3D& p) const { return velocidadViento; }

	/// <summary>
	/// Comprueba si una posición cae dentro de la zona de influencia del viento.
	/// La zona es una caja centrada horizontalmente en `centro`, que se extiende
	/// desde el suelo (y=0) hasta `altura.y`.
	/// </summary>
	/// <param name="p"> Posición a comprobar </param>
	/// <returns> true si la posición está dentro de la zona de viento </returns>
	bool isWithinWindZone(const Vector3D& p) const {
		Vector3D pos = p - centro;
		return pos.x >= -altura.x && pos.x <= altura.x &&
			pos.y >= 0 && pos.y <= altura.y &&
			pos.z >= -altura.z && pos.z <= altura.z;
	}
#pragma endregion

#pragma region Atributos
protected:
	/// <summary> Velocidad base del viento (usada por la implementación por defecto de getWindVelocity). </summary>
	Vector3D velocidadViento;

	/// <summary> Centro de la zona de influencia del viento. </summary>
	Vector3D centro;

	/// <summary> Semiejes de la zona de viento (extensión en X, Y, Z desde el centro). </summary>
	Vector3D altura;

	/// <summary> Coeficiente del término lineal de arrastre. </summary>
	float k1;

	/// <summary> Coeficiente del término cuadrático de arrastre. </summary>
	float k2;
#pragma endregion

#pragma region Constructores y destructor
public:
	/// <summary>
	/// Construye una fuerza de viento con velocidad, zona y coeficientes de arrastre.
	/// </summary>
	/// <param name="v"> Velocidad base del viento </param>
	/// <param name="c"> Centro de la zona de influencia </param>
	/// <param name="h"> Semiejes de la zona de influencia </param>
	/// <param name="k1"> Coeficiente del término lineal de arrastre </param>
	/// <param name="k2"> Coeficiente del término cuadrático de arrastre </param>
	WindForce(const Vector3D& v, const Vector3D& c, const Vector3D& h, float k1 = 0.5f, float k2 = 0.0f)
		: velocidadViento(v), centro(c), altura(h), k1(k1), k2(k2) {}

	/// <summary> Destructor virtual (permite herencia, como WhirlwindForce). </summary>
	virtual ~WindForce() = default;
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Calcula la fuerza de viento sobre una partícula. Si la partícula está
	/// fuera de la zona de viento, devuelve fuerza nula. En caso contrario,
	/// aplica arrastre lineal y cuadrático en función de la velocidad relativa
	/// entre viento y partícula.
	/// </summary>
	/// <param name="p"> Partícula sobre la que calcular la fuerza </param>
	/// <returns> Vector de fuerza de viento </returns>
	Vector3D getForce(const Particle& p) const override
	{
		const Vector3D& pos = p.getPosition();
		if (!isWithinWindZone(pos))
		{
			return Vector3D(0, 0, 0);
		}
		const Vector3D& windVelocity = getWindVelocity(pos) - p.getVelocity();
		return windVelocity * k1 + windVelocity * (windVelocity.magnitude() * k2);
	}
#pragma endregion
};

/// <summary>
/// Fuerza de viento en forma de remolino. Hereda de WindForce reutilizando la
/// zona de influencia y los coeficientes de arrastre, pero sobrescribe la
/// velocidad del viento para que describa un movimiento circular alrededor del
/// centro (con una componente vertical que empuja hacia arriba desde el suelo).
/// </summary>
class WhirlwindForce : public WindForce
{
#pragma region Metodos auxiliares
private:
	/// <summary>
	/// Calcula la velocidad del viento en remolino para una posición dada:
	/// componente tangencial en el plano XZ y componente vertical ascendente
	/// que disminuye con la altura, escaladas por el factor k.
	/// </summary>
	/// <param name="p"> Posición donde calcular la velocidad del viento </param>
	/// <returns> Vector de velocidad del viento en remolino </returns>
	Vector3D getWindVelocity(const Vector3D& p) const override
	{
		const Vector3D r = p - centro;
		return Vector3D(-r.z, 50.0f - r.y, r.x) * k;
	}
#pragma endregion

#pragma region Atributos
private:
	/// <summary> Factor de intensidad del remolino. </summary>
	float k;
#pragma endregion

#pragma region Constructores
public:
	/// <summary>
	/// Construye una fuerza de remolino centrada en `c` con la zona definida por `h`.
	/// La velocidad base del viento se pasa como (0,0,0) porque getWindVelocity
	/// se sobrescribe para calcularla dinámicamente según la posición.
	/// </summary>
	/// <param name="c"> Centro del remolino </param>
	/// <param name="h"> Semiejes de la zona de influencia </param>
	/// <param name="k"> Factor de intensidad del remolino </param>
	/// <param name="k1"> Coeficiente del término lineal de arrastre </param>
	/// <param name="k2"> Coeficiente del término cuadrático de arrastre </param>
	WhirlwindForce(const Vector3D& c, const Vector3D& h, float k = 0.3f, float k1 = 1.0f, float k2 = 0.0f)
		: WindForce(Vector3D(0, 0, 0), c, h, k1, k2), k(k) {}
#pragma endregion
};

/// <summary>
/// Fuerza de explosión radial. Al detonarse, aplica una fuerza que empuja las
/// partículas alejándose del centro, con un radio de acción que puede crecer
/// con el tiempo (si ve > 0) y una intensidad que decae exponencialmente con
/// constante de tiempo tau. Pasado un tiempo (≈5·tau) la explosión se desactiva
/// automáticamente.
/// </summary>
class ExplosionForce : public GeneradorFuerzas
{
#pragma region Metodos auxiliares
private:
	/// <summary>
	/// Calcula el radio actual de la explosión. Si ve &lt;= 0, el radio es constante (R);
	/// en caso contrario, crece linealmente con el tiempo hasta un máximo de R.
	/// </summary>
	/// <returns> Radio actual de la explosión </returns>
	float radioActual() const {
		if (ve <= 0.0f) return R;
		return (std::min)(R, ve * static_cast<float>(elapsedTime));
	}
#pragma endregion

#pragma region Atributos
private:
	/// <summary> Centro de la explosión. </summary>
	Vector3D c;

	/// <summary> Intensidad base de la fuerza de la explosión. </summary>
	float K;

	/// <summary> Radio máximo de influencia de la explosión. </summary>
	float R;

	/// <summary> Constante de tiempo del decaimiento exponencial de la fuerza. </summary>
	float tau;

	/// <summary> Velocidad de expansión del radio (0 = radio constante). </summary>
	float ve;
#pragma endregion

#pragma region Constructores
public:
	/// <summary>
	/// Construye una explosión centrada en `center`. Por defecto se crea inactiva:
	/// hay que llamar a detonate() para que empiece a aplicar fuerza.
	/// </summary>
	/// <param name="center"> Centro de la explosión </param>
	/// <param name="K"> Intensidad base </param>
	/// <param name="R"> Radio máximo de influencia </param>
	/// <param name="tau"> Constante de tiempo del decaimiento </param>
	/// <param name="ve"> Velocidad de expansión del radio (0 = constante) </param>
	ExplosionForce(Vector3D center, float K, float R, float tau, float ve = 0.0f)
		: c(center), K(K), R(R), tau(tau), ve(ve) {
		active = false;
	}
#pragma endregion

#pragma region Metodos
public:
	/// <summary>
	/// Detona la explosión: reinicia el tiempo acumulado y la activa.
	/// </summary>
	void detonate() { elapsedTime = 0.0; active = true; }

	/// <summary>
	/// Calcula la fuerza de la explosión sobre una partícula. Devuelve fuerza nula
	/// si la explosión está inactiva o si la partícula está fuera del radio actual.
	/// En caso contrario, aplica una fuerza radial alejándose del centro, con
	/// intensidad proporcional a K, inversa al cuadrado de la distancia (con un
	/// mínimo de 1 para evitar división por cero) y decaída exponencialmente en el tiempo.
	/// </summary>
	/// <param name="p"> Partícula sobre la que calcular la fuerza </param>
	/// <returns> Vector de fuerza radial de la explosión </returns>
	Vector3D getForce(const Particle& p) const override {
		if (!active) return Vector3D(0, 0, 0);
		const Vector3D d = p.getPosition() - c;
		const float r2 = d.dot(d);
		const float radio = radioActual();
		if (r2 >= radio * radio) return Vector3D(0, 0, 0);
		const float decay = std::exp(-static_cast<float>(elapsedTime) / tau);
		return d * (K / (std::max)(r2, 1.0f) * decay);   // max evita dividir por ~0 en el centro
	}

	/// <summary>
	/// Actualiza el tiempo acumulado de la explosión (heredado) y la desactiva
	/// automáticamente cuando ha pasado el tiempo suficiente para considerar que
	/// la fuerza se ha desvanecido por completo (≈5·tau).
	/// </summary>
	/// <param name="dt"> Tiempo transcurrido desde la última actualización, en segundos </param>
	void update(double dt) override {
		GeneradorFuerzas::update(dt);
		if (active && elapsedTime > 5.0 * tau) active = false;   // a partir de ~5 tau ya se ha desvanecido
	}
#pragma endregion
};