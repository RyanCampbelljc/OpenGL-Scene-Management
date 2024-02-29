//-----------------------------------------------------------------------------
// File:			Cube.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "nodes/Cube.h"

Cube::Cube(const std::string& name, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot)
    : Node(name, pos, scale, rot, calcRadius(scale))
    , m_color(1, 1, 1)
{
}

Cube::Cube(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot)
    : Node(pos, scale, rot, calcRadius(scale))
    , m_color(1, 1, 1)
{
}

// takes param so it can be use in the cpp constructor (else it would have a random value)
float Cube::calcRadius(const glm::vec3& scale)
{
    static const float rootThree = 1.73205080757f;
    return (rootThree * std::max<float>({scale.x, scale.y, scale.z})) / 2.0f;
}

// todo something to make it start at the top of a branch
/**
 * calculate BV position and radius
 * Updates all the children BV first
 * Gets the average position of children BV and this cubes position
 * This will now be the position of this BV.
 * Calculates the radius by finding max distance from BV pos to child BV pos + rad
 */
void Cube::updateBoundingVolume()
{
    if (m_children.empty())
    {
        getBoundingVolume().setPosition(getWorldPosition());
        getBoundingVolume().setRadius(calcRadius(getWorldScale()));
        return;
    }
    for (const auto& child : m_children)
    {
        child->updateBoundingVolume();
    }
    // now all children are up to date.
    // so the average position should be the average of the boudning
    // position of just this nodes children( dont need to account for grandchildren cause they are already calculated)
    glm::vec3 center = getWorldPosition();
    glm::vec3 scale = getWorldScale();
    for (const auto& child : m_children)
    {
        // should be world position of child BV
        center += child->getBoundingVolume().getPosition();
    }
    center /= (m_children.size() + 1);

    float radius = glm::distance(getWorldPosition(), center) + (calcRadius(getWorldScale()));
    for (const auto& child : m_children)
    {
        float distance = glm::distance(child->getBoundingVolume().getPosition(), center);
        distance += child->getBoundingVolume().getRadius();
        radius = std::max<float>(radius, distance);
    }

    getBoundingVolume().setPosition(center);
    getBoundingVolume().setRadius(radius);
}

Cube::~Cube()
{
}

//----------------------------------------------------------
// Renders this cube.
//----------------------------------------------------------
void Cube::render(const glm::mat4& view, const glm::mat4& projection, unsigned int frameNumber) const
{
    RenderParams params(getWorldPosition(), getWorldScale(), getWorldRotation(), view, projection, m_color);
    cubeRenderer.render(params);
    if (Scene::Instance().renderBoundingVolumes())
    {
        getBoundingVolume().render(params);
    }
}
glm::vec3 Cube::getColor() const
{
    return m_color;
}

void Cube::setColor(glm::vec3 color)
{
    m_color = color;
}