#pragma once
#include "nodes/node.h"
#include "utility.h"
#include "wolf.h"
class Grid3D : public Node
{
public:
    Grid3D(int linesPerHalfSpace, float gridSpacing = 1.0f);
    ~Grid3D();

    void render(const glm::mat4& view, const glm::mat4& projection, unsigned int frameNumber) const override;
    void showAxes()
    {
        m_showAxes = true;
    }
    void hideAxes()
    {
        m_showAxes = false;
    }
    float calcRadius(const glm::vec3& scale) override;
    void updateBoundingVolume() override;

private:
    void _createGrid(int linesPerHalfSpace, float gridSpacing);
    void _createAxes();

    wolf::VertexBuffer* m_pVB = 0;
    wolf::VertexDeclaration* m_pDecl = 0;
    wolf::Color4 m_color;

    wolf::VertexBuffer* m_pAxesVB = 0;
    wolf::VertexDeclaration* m_pAxesDecl = 0;

    wolf::Material* m_pMatGrid;
    wolf::Material* m_pMatAxes;

    int m_numVerts = 0;
    bool m_showAxes = true;
};
