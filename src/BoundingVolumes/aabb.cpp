//-----------------------------------------------------------------------------
// File:			aabb.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "BoundingVolumes/aabb.h"
AABB::AABB(const glm::vec3& pos, float height, float width)
    : BoundingVolume(pos, aabbRenderer())
    , m_height(height)
    , m_halfWidth(width)
{
}

// if all the aabb points are behind any of the frust planes -> no intersection
// if all the frstum points are behind any of the aabb planes -> no intersection
// else intersect
// according to the article below this method should work 99% of the time
// especially when the frustum isnt super small
// the other one percent will just be culled with the sphere check anyway.
//  https://iquilezles.org/articles/frustumcorrect/
bool AABB::inFrustum(const Utility::Frustum& frust) const
{
    // checks to see if all the aabb points are behind any of the frustum planes
    for (const auto& plane : frust.getPlanes())
    {
        bool allPointBehindPlane = true;
        auto planeEq = plane.getPlaneEquation();
        for (const auto& point : getAABBPoints())
        {
            // infront of plane
            if (dot(planeEq, glm::vec4(point, 1.0f)) > 0)
            {
                allPointBehindPlane = false;
                break;
            }
        }
        if (allPointBehindPlane) // all points behind plane, no intersection
            return false;
    }

    // checks to see if all the frustum points are behind all the aabb planes
    for (const auto& plane : getAABBPlanes())
    {
        bool allPointBehindPlane = true;
        auto planeEq = plane.getPlaneEquation();
        for (const auto& point : frust.getPoints())
        {
            // infront of plane
            if (dot(planeEq, glm::vec4(point, 1.0f)) > 0)
            {
                allPointBehindPlane = false;
                break;
            }
        }
        if (allPointBehindPlane) // all points behind plane, no intersection
            return false;
    }

    return true;
}

void AABB::render(RenderParams& params) const
{
    params.pos = getPosition();
    params.scale.y = m_height * 2;
    params.scale.x = m_halfWidth * 2;
    params.scale.z = m_halfWidth * 2;
    BoundingVolume::render(params);
}

// contains a sphere if the sphere is infront(atleast partially)of all the planes
bool AABB::containsSphere(const SphereBoundingVolume& sphere) const
{
    for (const auto& plane : getAABBPlanes())
    {
        // sphere is completely behind one plane. Therefore cannt be in AABB
        if (!sphere.sphereInFrontPlane(plane))
        {
            return false;
        }
    }
    return true;
}

// Returns a list of all the aabb planes
std::vector<Utility::Plane> AABB::getAABBPlanes() const
{
    Utility::Plane top, bottom, left, right, near, far;
    top.normal = glm::vec3(0, -1, 0);
    glm::vec3 topPoint = getPosition();
    topPoint.y += m_height;
    top.distance = -dot(top.normal, topPoint);

    bottom.normal = glm::vec3(0, 1, 0);
    glm::vec3 bottomPoint = getPosition();
    bottomPoint.y -= m_height;
    bottom.distance = -dot(bottom.normal, bottomPoint);

    left.normal = glm::vec3(1, 0, 0);
    glm::vec3 leftPoint = getPosition();
    leftPoint.x -= m_halfWidth;
    left.distance = -dot(left.normal, leftPoint);

    right.normal = glm::vec3(-1, 0, 0);
    glm::vec3 rightPoint = getPosition();
    rightPoint.x += m_halfWidth;
    right.distance = -dot(right.normal, rightPoint);

    near.normal = glm::vec3(0, 0, -1);
    glm::vec3 nearPoint = getPosition();
    nearPoint.z += m_halfWidth;
    near.distance = -dot(near.normal, nearPoint);

    far.normal = glm::vec3(0, 0, 1);
    glm::vec3 farPoint = getPosition();
    farPoint.z -= m_halfWidth;
    far.distance = -dot(far.normal, farPoint);

    std::vector<Utility::Plane> p = {top, bottom, left, right, near, far};
    return p;
}

void AABB::setHeight(float h)
{
    m_height = h;
}

void AABB::setHalfWidth(float w)
{
    m_halfWidth = w;
}

float AABB::getHalfWidth() const
{
    return m_halfWidth;
}

// returns list of all 8 points of the AABB
std::vector<glm::vec3> AABB::getAABBPoints() const
{
    float minX, maxX, minY, maxY, minZ, maxZ;
    glm::vec3 pos = getPosition();
    minX = pos.x - m_halfWidth;
    maxX = pos.x + m_halfWidth;
    minY = pos.y - m_height;
    maxY = pos.y + m_height;
    minZ = pos.z - m_halfWidth;
    maxZ = pos.z + m_halfWidth;
    glm::vec3 a, b, c, d, e, f, g, h;
    a = glm::vec3(minX, minY, minZ);
    b = glm::vec3(maxX, minY, minZ);
    c = glm::vec3(minX, maxY, minZ);
    d = glm::vec3(maxX, maxY, minZ);
    e = glm::vec3(minX, minY, maxZ);
    f = glm::vec3(maxX, minY, maxZ);
    g = glm::vec3(minX, maxY, maxZ);
    h = glm::vec3(maxX, maxY, maxZ);
    return std::vector<glm::vec3>{a, b, c, d, e, f, g, h};
}