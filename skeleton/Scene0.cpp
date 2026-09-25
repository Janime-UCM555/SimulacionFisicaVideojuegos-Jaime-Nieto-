#include "Scene0.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <iostream>
void Scene0::init()
{
	Vector3D u(3.0f, 1.0f, 0.0f);
	Vector3D v(0.0f, 4.0f, 0.0f);
	Vector3D w = u.cross(v);

	u.normalize(); v.normalize(); w.normalize();
	u *= 5; v *= 5; w *= 5;
	std::cout << "Vector u: (" << u.x << ", " << u.y << ", " << u.z << ")\n";
    std::cout << "Vector v: (" << v.x << ", " << v.y << ", " << v.z << ")\n";
    std::cout << "Vector w: (" << w.x << ", " << w.y << ", " << w.z << ")\n";
    physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
    m_transforms.push_back(physx::PxTransform(u));
    m_transforms.push_back(physx::PxTransform(v));
    m_transforms.push_back(physx::PxTransform(w));

    // Se registra el RenderItem exactamente como en la plantilla original
    m_renderItems.push_back(new RenderItem(shape, &m_transforms[0], Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms[1], Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
    m_renderItems.push_back(new RenderItem(shape, &m_transforms[2], Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
}