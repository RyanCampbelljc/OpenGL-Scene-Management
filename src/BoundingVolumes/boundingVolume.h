//-----------------------------------------------------------------------------
// File:			BoundingVolume.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "renderables/renderable.h"
#include "utility.h"
class BoundingVolumeBase : public Renderable
{
public:
    BoundingVolumeBase(BoundingVolumeBase&) = delete;
    ~BoundingVolumeBase();
    virtual bool inFrustum(const Utility::Frustum& frust) const = 0;
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition() const;

protected:
    BoundingVolumeBase(const glm::vec3& pos);

private:
    glm::vec3 m_pos;
};

// Generic BV class that can have a different types as the outline(sphere for nodes. aabb for quadtree sections)
template<typename T>
class BoundingVolume : public BoundingVolumeBase
{
public:
    void render(RenderParams& params) const override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_BVOutline.render(params);
    }

protected:
    BoundingVolume(const glm::vec3& position, T&& outline)
        : BoundingVolumeBase(position)
        , m_BVOutline(std::move(outline))
    {
    }
    T m_BVOutline;
};