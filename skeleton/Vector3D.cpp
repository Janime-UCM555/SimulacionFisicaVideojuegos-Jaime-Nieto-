#include "Vector3D.h"
#include <stdexcept>
#pragma region Constructores
Vector3D::Vector3D(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

Vector3D::Vector3D(const physx::PxVec3& vec) noexcept
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}
#pragma endregion
#pragma region Metodos
float Vector3D::magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalize() const
{
	const float mag = magnitude();
	if (mag == 0)
	{
		// Evitamos la división por cero devolviendo un vector nulo
		return Vector3D(0, 0, 0);
	}
	return Vector3D(x / mag, y / mag, z / mag);
}

float Vector3D::dot(const Vector3D& v) const noexcept
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(const Vector3D& v) const noexcept
{
	return Vector3D(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}
#pragma endregion
#pragma region Operadores

void Vector3D::operator=(const physx::PxVec3& vec) noexcept
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vector3D Vector3D::operator+(const Vector3D& v) const noexcept
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator-(const Vector3D& v) const noexcept
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator*(float scalar) const noexcept
{
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(float scalar) const
{
	if (scalar == 0)
	{
		throw std::runtime_error("Division by zero in Vector3D::operator/.");
	}
	return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D& Vector3D::operator+=(const Vector3D& v) noexcept
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v) noexcept
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3D& Vector3D::operator*=(float scalar) noexcept
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3D& Vector3D::operator/=(float scalar)
{
	if (scalar == 0)
	{
		throw std::runtime_error("Division by zero in Vector3D::operator/=.");
	}
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector3D::operator physx::PxVec3() const
{
	return physx::PxVec3(x, y, z);
}
#pragma endregion