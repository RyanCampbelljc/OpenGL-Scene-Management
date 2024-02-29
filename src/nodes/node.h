//-----------------------------------------------------------------------------
// File:			Node.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "BoundingVolumes/SphereBoundingVolume.h"
#include "utility.h"
#include "wolf.h"

class Node
{
public:
    // todo
    //  should parent be const here
    Node(const std::string& name, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, float rad);
    Node(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rot, float rad);
    Node(Node&) = delete;
    ~Node();
    void setPosition(const glm::vec3& pos);
    void setScale(const glm::vec3& scale);
    void setRotation(const glm::vec3& rot);
    void translate(const glm::vec3& pos);
    void scale(const glm::vec3& scale);
    void rotate(const glm::vec3& rot);
    void addChild(Node* child);
    // todo should these return a reference
    glm::vec3 getWorldPosition() const;
    glm::vec3 getWorldScale() const;
    glm::vec3 getWorldRotation() const;
    glm::vec3 getLocalPosition() const;
    glm::vec3 getLocalScale() const;
    glm::vec3 getLocalRotation() const;
    glm::mat4 getLocalTransform() const;
    glm::mat4 getWorldTransform() const;
    const SphereBoundingVolume& getBoundingVolume() const;
    SphereBoundingVolume& getBoundingVolume();
    std::string getName() const;
    virtual void render(const glm::mat4& view, const glm::mat4& projection, unsigned int frameNumber) const;
    void update(float dt);
    int getID() const;
    Node* getParent() const;
    virtual void updateBoundingVolume() = 0;
    virtual float calcRadius(const glm::vec3& scale) = 0;
    void getChildrenInFrustum(std::vector<const Node*>& vec, const Utility::Frustum& frust, unsigned int frameNumber);

private:
    int m_id;
    std::string m_name;
    // local position/scale/rotation
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    glm::mat4 m_transform;
    Node* m_pParent;

    bool m_dirty;
    SphereBoundingVolume m_boundingSphere;
    static int m_nodesCreated;
    void updateLocalTransform();
    void setParent(Node* pParent);

protected: // todo just make a getter?
    float m_dt;
    std::vector<Node*> m_children;
    unsigned int m_frameNumber;
};
