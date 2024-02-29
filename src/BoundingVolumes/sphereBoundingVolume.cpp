//-----------------------------------------------------------------------------
// File:			SphereBoundingVolume.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "BoundingVolumes/sphereBoundingVolume.h"

SphereBoundingVolume::SphereBoundingVolume(const glm::vec3& pos, float rad)
    : BoundingVolume(pos, SphereRenderer())
    , m_radius(rad)
{
    // not needed now since the wvp is passed to it
    //  m_BVOutline.SetPosition(pos);
}

float SphereBoundingVolume::getRadius() const
{
    return m_radius;
}

void SphereBoundingVolume::setRadius(float radius)
{
    m_radius = radius;
}

bool SphereBoundingVolume::inFrustum(const Utility::Frustum& frust) const
{
    return sphereInFrontPlane(frust.left) && sphereInFrontPlane(frust.right) && sphereInFrontPlane(frust.near)
           && sphereInFrontPlane(frust.far) && sphereInFrontPlane(frust.top) && sphereInFrontPlane(frust.bottom);
}

void SphereBoundingVolume::render(RenderParams& params) const
{
    params.pos = getPosition();
    params.scale = glm::vec3(m_radius, m_radius, m_radius);
    params.scale *= 2.0f;
    BoundingVolume::render(params);
}

bool SphereBoundingVolume::sphereInFrontPlane(const Utility::Plane& plane) const
{
    float distance = (getPosition().x * plane.normal.x) + (getPosition().y * plane.normal.y)
                     + (getPosition().z * plane.normal.z) + plane.distance;
    if (distance < -getRadius())
    {
        return false;
    }
    return true;
}

// void SphereBoundingVolume::setPosition(glm::vec3 pos)
// {
//     BoundingVolume::setPosition(pos);
//     // m_BVOutline.SetPosition(pos);
// }
