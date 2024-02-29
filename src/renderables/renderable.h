//-----------------------------------------------------------------------------
// File:			renderable.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "wolf.h"

struct RenderParams
{
    RenderParams(
        const glm::vec3& pos,
        const glm::vec3& scale,
        const glm::vec3& rot,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& color)
        : pos(pos)
        , scale(scale)
        , rot(rot)
        , view(view)
        , projection(projection)
        , color(color)
    {
    }
    glm::mat4 getMWVP() const
    {
        return projection * view * getWorld();
    }

    glm::mat4 getWorld() const
    {
        glm::mat4 tansform = glm::translate(glm::mat4(1.0f), pos);
        tansform = glm::rotate(tansform, rot.x, glm::vec3(1, 0, 0));
        tansform = glm::rotate(tansform, rot.y, glm::vec3(0, 1, 0));
        tansform = glm::rotate(tansform, rot.z, glm::vec3(0, 0, 1));
        tansform = glm::scale(tansform, scale);
        return tansform;
    }

    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rot;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 color;
};

class Renderable
{
public:
    // purposley not const
    virtual void render(RenderParams& params) const = 0;
};
