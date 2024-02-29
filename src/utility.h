//-----------------------------------------------------------------------------
// File:			utility.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "../samplefw/Sample.h"
#include "renderables/renderable.h"
#include "wolf.h"
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
namespace Utility
{
// random float inclusive
float randomFloat(float min, float max);
glm::vec3 planeNormalFrom3Points(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

struct Plane
{
    glm::vec3 normal;
    float distance;
    glm::vec4 getPlaneEquation() const
    {
        return glm::vec4(normal.x, normal.y, normal.z, distance);
    }
};

struct Frustum
{
    Frustum(
        const glm::vec3& ntl,
        const glm::vec3& ntr,
        const glm::vec3& nbl,
        const glm::vec3& nbr,
        const glm::vec3& ftl,
        const glm::vec3& ftr,
        const glm::vec3& fbl,
        const glm::vec3& fbr)
        : nearTopLeft(ntl)
        , nearTopRight(ntr)
        , nearBottomLeft(nbl)
        , nearBottomRight(nbr)
        , farTopLeft(ftl)
        , farTopRight(ftr)
        , farBottomLeft(fbl)
        , farBottomRight(fbr)
    {
        createPlanes();
    }
    Plane near;
    Plane far;
    Plane top;
    Plane bottom;
    Plane left;
    Plane right;
    glm::vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight, farTopLeft, farTopRight, farBottomLeft,
        farBottomRight;

    // todo references?
    std::vector<Plane> getPlanes() const
    {
        std::vector<Plane> planeList = {left, right, near, far, top, bottom};
        return planeList;
    }

    std::vector<glm::vec3> getPoints() const
    {
        std::vector<glm::vec3> pointList = {nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight,
                                            farTopLeft,  farTopRight,  farBottomLeft,  farBottomRight};
        return pointList;
    }

    bool pointInFrust(glm::vec3 point) const
    {
        for (const auto& plane : getPlanes())
        {
            float distance = dot(plane.normal, point) + plane.distance;
            if (distance < 0) // behind plane
                return false;
        }
        return true;
    }
    void createPlanes()
    {
        near.normal = planeNormalFrom3Points(nearTopLeft, nearTopRight, nearBottomRight);
        near.distance = -dot(near.normal, nearTopLeft);

        far.normal = planeNormalFrom3Points(farTopRight, farTopLeft, farBottomLeft);
        far.distance = -dot(far.normal, farTopLeft);

        left.normal = planeNormalFrom3Points(farTopLeft, nearTopLeft, nearBottomLeft);
        left.distance = -dot(left.normal, nearTopLeft);

        right.normal = planeNormalFrom3Points(nearTopRight, farTopRight, farBottomRight);
        right.distance = -dot(right.normal, nearTopRight);

        top.normal = planeNormalFrom3Points(nearTopLeft, farTopLeft, farTopRight);
        top.distance = -dot(top.normal, nearTopRight);

        bottom.normal = planeNormalFrom3Points(nearBottomLeft, nearBottomRight, farBottomRight);
        bottom.distance = -dot(bottom.normal, nearBottomRight);
    }
};
} // namespace Utility