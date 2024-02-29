//-----------------------------------------------------------------------------
// File:			scene.h
// Original Author:	Ryan Campbell
// singleton to represent the scene
//-----------------------------------------------------------------------------
#pragma once
#include "nodes/camera.h"
#include "nodes/node.h"
#include "quadtree.h"
#include "utility.h"

class Scene
{
public:
    ~Scene();
    Scene(Scene& s) = delete; // delete copy constructor
    static Scene& Instance();
    void setActiveCamera(Camera* cam);
    void addNode(Node* node);
    void renderScene(int width, int height) const;
    void update(float dt);
    void initScene();
    bool renderBoundingVolumes() const;
    unsigned int getFrameNumber() const;

private:
    void preformCulling();
    void createQuadTree();
    Scene();

private:
    std::vector<const Node*> m_nodesToRender;
    std::vector<Node*> m_nodes;
    Camera* m_cam;
    float m_dt;
    bool m_renderBoundingVolumes;
    wolf::App* m_pApp;
    QuadTree m_quadTree;
    unsigned int m_frameNumber;
};