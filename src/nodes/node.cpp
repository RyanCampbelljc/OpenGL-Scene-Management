//-----------------------------------------------------------------------------
// File:			Node.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "node.h"

int Node::m_nodesCreated = 0;
Node::Node(const std::string& name, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, float rad)
    : m_id(m_nodesCreated++)
    , m_name(name)
    , m_position(pos)
    , m_scale(scale)
    , m_rotation(rot)
    , m_pParent(nullptr)
    , m_dirty(true)
    , m_boundingSphere(getWorldPosition(), rad)
    , m_dt(0.0f)
    , m_frameNumber(0)
{
}

Node::Node(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, float rad)
    : m_id(m_nodesCreated++)
    , m_name("")
    , m_position(pos)
    , m_scale(scale)
    , m_rotation(rot)
    , m_pParent(nullptr)
    , m_dirty(true)
    , m_boundingSphere(pos, rad)
    , m_dt(0.0f)
    , m_frameNumber(0)
{
}

Node::~Node()
{
    for (const auto& child : m_children)
    {
        delete child;
    }
}
// todo if any of these translation functions are called then update bounding box
void Node::setPosition(const glm::vec3& pos)
{
    m_position = pos;
    m_dirty = true;
}

void Node::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_dirty = true;
}

void Node::setRotation(const glm::vec3& rot)
{
    m_rotation = rot;
    m_dirty = true;
}

void Node::translate(const glm::vec3& pos)
{
    m_position += pos;
    m_dirty = true;
}

void Node::scale(const glm::vec3& scale)
{
    m_scale *= scale;
    m_dirty = true;
}

void Node::rotate(const glm::vec3& rot)
{
    m_rotation += rot;
    m_dirty = true;
}

void Node::addChild(Node* child)
{
    child->setParent(this);
    m_children.push_back(child);
}

glm::vec3 Node::getWorldPosition() const
{
    if (m_pParent != NULL)
    {
        return m_position + m_pParent->getWorldPosition();
    }
    return m_position;
}

glm::vec3 Node::getWorldScale() const
{
    if (m_pParent != NULL)
    {
        return m_scale * m_pParent->getWorldScale();
    }
    return m_scale;
}

glm::vec3 Node::getWorldRotation() const
{
    if (m_pParent != NULL)
    {
        return m_rotation + m_pParent->getWorldRotation();
    }
    return m_rotation;
}

glm::vec3 Node::getLocalPosition() const
{
    return m_position;
}

glm::vec3 Node::getLocalScale() const
{
    return m_scale;
}

glm::vec3 Node::getLocalRotation() const
{
    return m_rotation;
}

glm::mat4 Node::getLocalTransform() const
{
    return m_transform;
}

void Node::updateLocalTransform()
{
    if (m_dirty)
    {
        m_transform = glm::translate(glm::mat4(1.0f), m_position);
        m_transform = glm::rotate(m_transform, m_rotation.x, glm::vec3(1, 0, 0));
        m_transform = glm::rotate(m_transform, m_rotation.y, glm::vec3(0, 1, 0));
        m_transform = glm::rotate(m_transform, m_rotation.z, glm::vec3(0, 0, 1));
        m_transform = glm::scale(m_transform, m_scale);
        m_dirty = false;
        updateBoundingVolume();
    }
}

glm::mat4 Node::getWorldTransform() const
{
    if (m_pParent != NULL)
    {
        return m_pParent->getWorldTransform() * getLocalTransform();
    }
    return getLocalTransform();
}

const SphereBoundingVolume& Node::getBoundingVolume() const
{
    return m_boundingSphere;
}

SphereBoundingVolume& Node::getBoundingVolume()
{
    return m_boundingSphere;
}

std::string Node::getName() const
{
    return m_name;
}

// todo needs to be overriden by most subclasses
void Node::render(const glm::mat4& view, const glm::mat4& projection, unsigned int frameNumber) const
{
    ;
}

void Node::update(float dt)
{
    m_dt = dt;
    updateLocalTransform();
    // todo if gemoetry could move would update BV here
    for (const auto& child : m_children)
    {
        child->update(dt);
    }
}

int Node::getID() const
{
    return m_id;
}

Node* Node::getParent() const
{
    return m_pParent;
}

void Node::setParent(Node* pParent)
{
    // prevents reparenting of objects
    if (m_pParent != nullptr)
    {
        throw std::runtime_error("This node already has a parent");
    }
    m_pParent = pParent;
}

// adds nodes to a vector of nodes to render if they are in the view frustum
// and if they have not already been been added to the list this frame(frame number)
// Seemed better than having to make methods such as render not const in order to update nodes
// m_frameNumber member. Also this helps keep the list of renerable object shorter(no repeats in list)
void Node::getChildrenInFrustum(std::vector<const Node*>& vec, const Utility::Frustum& frust, unsigned int frameNumber)
{
    if (m_boundingSphere.inFrustum(frust))
    {
        if (m_frameNumber != frameNumber)
        {
            vec.push_back(this);
            m_frameNumber = frameNumber;
        }
    }

    for (const auto& child : m_children)
        child->getChildrenInFrustum(vec, frust, frameNumber);
}