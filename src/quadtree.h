//-----------------------------------------------------------------------------
// File:			Quadtree.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "BoundingVolumes/aabb.h"
#include "nodes/node.h"
#include "renderables/renderable.h"
#include "utility.h"

class QuadTree
{
public:
    QuadTree(glm::vec3 pos);
    QuadTree(QuadTree&) = delete;
    ~QuadTree();
    void render(const glm::mat4& view, const glm::mat4& projection) const;
    bool insert(Node* node);
    void getNodesInFrustum(std::vector<const Node*>& vec, const Utility::Frustum& frust) const;

private:
    QuadTree(glm::vec3 pos, int depth, float halfWidth);
    static const float s_AABBHeight;
    static const int s_maxElements;
    static const int s_maxDepth;
    static const float s_startHalfWidth;
    glm::vec3 m_position;
    int m_depth;
    QuadTree* m_topLeft;
    QuadTree* m_topRight;
    QuadTree* m_bottomLeft;
    QuadTree* m_bottomRight;
    std::vector<Node*> m_children;
    AABB m_aabb;
    glm::vec3 m_color;

private:
    void subDivide();
};