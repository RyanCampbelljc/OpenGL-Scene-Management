//-----------------------------------------------------------------------------
// File:			Cube.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------

#pragma once
#include "node.h"
#include "renderables/CubeRenderer.h"
#include "renderables/renderable.h"
#include "scene.h"
#include "utility.h"

class Cube : public Node
{
public:
    //----------------------------------------------------------
    // PUBLIC INTERFACE
    //----------------------------------------------------------
    Cube(const std::string& name, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot);
    Cube(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot);
    Cube(Cube&) = delete;
    ~Cube();
    void render(const glm::mat4& view, const glm::mat4& projection, unsigned int frameNumber) const override;
    glm::vec3 getColor() const;
    void setColor(glm::vec3 color);
    float calcRadius(const glm::vec3& scale) override;
    void updateBoundingVolume() override;

    //----------------------------------------------------------

private:
    CubeRenderer cubeRenderer;
    glm::vec3 m_color;
    //----------------------------------------------------------
};