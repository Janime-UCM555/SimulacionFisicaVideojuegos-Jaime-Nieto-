#pragma once
#include <PxPhysicsAPI.h>

/// <summary>
/// Representa un vector tridimensional con operaciones matemáticas básicas
/// y conversión hacia/desde el tipo PxVec3 de PhysX
/// </summary>
class Vector3D
{
public:
#pragma region Atributos
	float x, y, z;
#pragma endregion
#pragma region Constructores

	/// <summary>
	/// Construye un vector 3D a partir de sus tres componentes
	/// </summary>
	/// <param name="x">Componente X del vector</param>
	/// <param name="y">Componente Y del vector</param>
	/// <param name="z">Componente Z del vector</param>
	Vector3D(float x = 0, float y = 0, float z = 0) noexcept;

	/// <summary>
	/// Construye un vector 3D a partir de un PxVec3 de PhysX
	/// </summary>
	/// <param name="vec">Vector de PhysX del que se copian las componentes</param>
	Vector3D(const physx::PxVec3& vec) noexcept;
#pragma endregion
#pragma region Metodos

	/// <summary>
	/// Calcula la magnitud (longitud) del vector
	/// </summary>
	/// <returns>La longitud euclídea del vector</returns>
	float magnitude() const;

	/// <summary>
	/// Devuelve una copia normalizada del vector (longitud 1)
	/// </summary>
	/// <returns>El vector normalizado, o un vector nulo si la magnitud es cero</returns>
	Vector3D normalize() const;

	/// <summary>
	/// Calcula el producto escalar (dot product) con otro vector
	/// </summary>
	/// <param name="v">Vector con el que se calcula el producto escalar</param>
	/// <returns>El resultado escalar de la operación</returns>
	float dot(const Vector3D& v) const noexcept;

	/// <summary>
	/// Calcula el producto vectorial (cross product) con otro vector
	/// </summary>
	/// <param name="v">Vector con el que se calcula el producto vectorial</param>
	/// <returns>Un nuevo vector perpendicular a los dos vectores originales</returns>
	Vector3D cross(const Vector3D& v) const noexcept;
#pragma endregion
#pragma region Operadores

	/// <summary>
	/// Asigna al vector los valores de un PxVec3 de PhysX
	/// </summary>
	/// <param name="vec">Vector de PhysX del que se copian las componentes</param>
	void operator=(const physx::PxVec3& vec) noexcept;

	/// <summary>
	/// Suma este vector con otro
	/// </summary>
	/// <param name="v">Vector a sumar</param>
	/// <returns>Un nuevo vector resultado de la suma</returns>
	Vector3D operator+(const Vector3D& v) const noexcept;

	/// <summary>
	/// Resta otro vector a este vector
	/// </summary>
	/// <param name="v">Vector a restar</param>
	/// <returns>Un nuevo vector resultado de la resta</returns>
	Vector3D operator-(const Vector3D& v) const noexcept;

	/// <summary>
	/// Multiplica el vector por un escalar
	/// </summary>
	/// <param name="scalar">Valor escalar por el que se multiplica</param>
	/// <returns>Un nuevo vector escalado</returns>
	Vector3D operator*(float scalar) const noexcept;

	/// <summary>
	/// Divide el vector por un escalar
	/// </summary>
	/// <param name="scalar">Valor escalar por el que se divide</param>
	/// <returns>Un nuevo vector escalado</returns>
	/// <exception cref="std::runtime_error">Si el escalar es cero</exception>
	Vector3D operator/(float scalar) const;

	/// <summary>
	/// Suma otro vector a este y actualiza sus componentes
	/// </summary>
	/// <param name="v">Vector a sumar</param>
	/// <returns>Referencia a este vector modificado</returns>
	Vector3D& operator+=(const Vector3D& v) noexcept;

	/// <summary>
	/// Resta otro vector a este y actualiza sus componentes
	/// </summary>
	/// <param name="v">Vector a restar</param>
	/// <returns>Referencia a este vector modificado</returns>
	Vector3D& operator-=(const Vector3D& v) noexcept;

	/// <summary>
	/// Multiplica este vector por un escalar y actualiza sus componentes
	/// </summary>
	/// <param name="scalar">Valor escalar por el que se multiplica</param>
	/// <returns>Referencia a este vector modificado</returns>
	Vector3D& operator*=(float scalar) noexcept;

	/// <summary>
	/// Divide este vector por un escalar y actualiza sus componentes
	/// </summary>
	/// <param name="scalar">Valor escalar por el que se divide</param>
	/// <returns>Referencia a este vector modificado</returns>
	/// <exception cref="std::runtime_error">Si el escalar es cero</exception>
	Vector3D& operator/=(float scalar);

	/// <summary>
	/// Convierte este vector a un PxVec3 de PhysX
	/// </summary>
	/// <returns>Un PxVec3 equivalente a este vector</returns>
	operator physx::PxVec3() const;
#pragma endregion
};