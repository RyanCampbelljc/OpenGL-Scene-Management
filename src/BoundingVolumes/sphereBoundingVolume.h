//-----------------------------------------------------------------------------
// File:			SphereBoundingVolume.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "BoundingVolumes/boundingVolume.h"
#include "renderables/SphereRenderer.h"

class SphereBoundingVolume : public BoundingVolume<SphereRenderer>
{
public:
    SphereBoundingVolume(const glm::vec3& pos, float rad);
    SphereBoundingVolume(SphereBoundingVolume&) = delete;
    float getRadius() const;
    void setRadius(float radius);
    bool inFrustum(const Utility::Frustum& frust) const override;
    void render(RenderParams& params) const override;
    bool sphereInFrontPlane(const Utility::Plane& plane) const;

private:
    float m_radius;
};