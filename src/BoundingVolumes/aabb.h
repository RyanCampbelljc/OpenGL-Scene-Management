//-----------------------------------------------------------------------------
// File:			aabb.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "BoundingVolumes/sphereBoundingVolume.h"
#include "renderables/aabbRenderer.h"
#include "utility.h"
class AABB : public BoundingVolume<aabbRenderer>
{
public:
    AABB(const glm::vec3&, float height, float width);
    AABB(AABB&) = delete;
    bool inFrustum(const Utility::Frustum& frust) const override;
    void render(RenderParams& params) const override;
    bool containsSphere(const SphereBoundingVolume& sphere) const;
    std::vector<Utility::Plane> getAABBPlanes() const;
    void setHeight(float h);
    void setHalfWidth(float w);
    float getHalfWidth() const;
    std::vector<glm::vec3> getAABBPoints() const;

private:
    float m_halfWidth;
    float m_height;
};
