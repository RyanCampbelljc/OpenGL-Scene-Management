//-----------------------------------------------------------------------------
// File:			quadtree.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "quadtree.h"
#include "scene.h"
const float QuadTree::s_AABBHeight = 40.0f;
const int QuadTree::s_maxElements = 5;
const int QuadTree::s_maxDepth = 3;
const float QuadTree::s_startHalfWidth = 100.0f;

// public constructor that starts depth with 1
QuadTree::QuadTree(glm::vec3 pos)
    : m_position(pos)
    , m_depth(1)
    , m_bottomLeft(nullptr)
    , m_bottomRight(nullptr)
    , m_topLeft(nullptr)
    , m_topRight(nullptr)
    , m_aabb(pos, s_AABBHeight, s_startHalfWidth)
    , m_color(1, 1, 1)
{
}

// private constructor that will be used to create more quadtrees when this one is full of chilren
QuadTree::QuadTree(glm::vec3 pos, int depth, float halfWidth)
    : m_position(pos)
    , m_depth(depth)
    , m_bottomLeft(nullptr)
    , m_bottomRight(nullptr)
    , m_topLeft(nullptr)
    , m_topRight(nullptr)
    , m_aabb(pos, s_AABBHeight, halfWidth)
{
    if (m_depth == 2)
    {
        m_color = glm::vec3(1, 0, 0);
    }
    else
    {
        m_color = glm::vec3(0, 0, 1);
    }
}

QuadTree::~QuadTree()
{
    delete m_topLeft;
    delete m_topRight;
    delete m_bottomLeft;
    delete m_bottomRight;
}

void QuadTree::render(const glm::mat4& view, const glm::mat4& projection) const
{
    RenderParams params(m_position, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), view, projection, m_color);
    m_aabb.render(params);
    if (m_topLeft != nullptr)
    {
        m_topLeft->render(view, projection);
        m_bottomLeft->render(view, projection);
        m_topRight->render(view, projection);
        m_bottomRight->render(view, projection);
    }
}

bool QuadTree::insert(Node* node)
{
    // if not in boundrys return false
    if (!m_aabb.containsSphere(node->getBoundingVolume()))
    {
        return false;
    }
    // if at the max depth insert node here
    if (m_depth == s_maxDepth)
    {
        m_children.push_back(node);
        return true;
    }
    // if this hasnt subdivided and theres room insert into children return true
    if (m_topLeft == nullptr && m_children.size() < s_maxElements)
    {
        m_children.push_back(node);
        return true;
    }
    // if its hasnt subdivided &its children is full then subdivide(no return here keep going)
    if (m_topLeft == nullptr)
    {
        subDivide();
    }

    // after subdivision attemp to put this node in one of the 4 child quadtrees
    bool check = false;
    if (m_topLeft->insert(node))
        check = true;
    if (m_topRight->insert(node))
        check = true;
    if (m_bottomLeft->insert(node))
        check = true;
    if (m_bottomRight->insert(node))
        check = true;

    // have to try and place node in all subtrees incase it is in more than 1
    if (check) // has been inserted in atleast one tree
    {
        return true;
    }
    else // hasnt been inserted in any tree
    {
        // should never run
        throw std::runtime_error("node wasnt inserted");
    }
}

/**
 * Subdivides this quadtree into 4 smaller quadtrees.
 * Then inserts all children into subquadtree & clears children list
 */
void QuadTree::subDivide()
{
    if (m_depth + 1 > s_maxDepth)
    {
        throw std::runtime_error("Subdivide should not have been called. Node should have been inserted");
    }
    glm::vec3 pos = m_position;
    float width = m_aabb.getHalfWidth();
    float addition = width / 2.0f;
    pos.x -= addition;
    pos.z -= addition;
    m_topLeft = new QuadTree(pos, m_depth + 1, addition);

    pos = m_position;
    pos.x += addition;
    pos.z -= addition;
    m_topRight = new QuadTree(pos, m_depth + 1, addition);

    pos = m_position;
    pos.x -= addition;
    pos.z += addition;
    m_bottomLeft = new QuadTree(pos, m_depth + 1, addition);

    pos = m_position;
    pos.x += addition;
    pos.z += addition;
    m_bottomRight = new QuadTree(pos, m_depth + 1, addition);

    for (const auto& child : m_children)
    {
        insert(child);
    }
    m_children.clear();
}

// quadtree only needs to keep track of top level node
void QuadTree::getNodesInFrustum(std::vector<const Node*>& vec, const Utility::Frustum& frust) const
{
    // this quadtree is not in frustum;
    if (!m_aabb.inFrustum(frust))
    {
        // std::cout << "not in view " << m_depth << std::endl;
        return;
    }
    // std::cout << "in view " << m_depth << std::endl;

    //  leaf node
    if (m_topLeft == nullptr)
    {
        // loops through list of children and checks them against frustum
        for (const auto& child : m_children)
        {
            if (child->getBoundingVolume().inFrustum(frust))
            {
                // if a child is in the frustum gets that node and checks its subchildren against the frustum
                // aswell as checking if it has already been added to the render queue this frame
                child->getChildrenInFrustum(vec, frust, Scene::Instance().getFrameNumber());
            }
        }
        return;
    }

    // if not a leaf node check all child quadtrees
    m_topLeft->getNodesInFrustum(vec, frust);
    m_topRight->getNodesInFrustum(vec, frust);
    m_bottomLeft->getNodesInFrustum(vec, frust);
    m_bottomRight->getNodesInFrustum(vec, frust);
}