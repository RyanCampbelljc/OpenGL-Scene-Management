//-----------------------------------------------------------------------------
// File:			scene.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "scene.h"

Scene::Scene()
    : m_cam(new Camera(1280, 720))
    , m_dt(0.0f)
    , m_renderBoundingVolumes(false)
    , m_pApp((wolf::App*)(glfwGetWindowUserPointer(glfwGetCurrentContext())))
    , m_quadTree(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_frameNumber(0)
{
}

Scene::~Scene()
{
    for (const auto& node : m_nodes)
    {
        delete node;
    }
}

// scene singleton
Scene& Scene::Instance()
{
    static Scene instance;
    return instance;
}

void Scene::setActiveCamera(Camera* cam)
{
    m_cam = cam;
}

void Scene::addNode(Node* node)
{
    m_nodes.push_back(node);
}

/**
 * Renders all the nodes to be rendered this frame
 * Renders the bounding volumes and quadtree of enabled
 */
void Scene::renderScene(int width, int height) const
{
    m_cam->setWidth(width);
    m_cam->setHeight(height);
    std::cout << m_nodesToRender.size() << std::endl;
    for (const auto& node : m_nodesToRender)
    {
        node->render(m_cam->getView(), m_cam->getProjection(), m_frameNumber);
    }
    if (m_renderBoundingVolumes)
    {
        glDisable(GL_CULL_FACE);
        m_quadTree.render(m_cam->getView(), m_cam->getProjection());
        glEnable(GL_CULL_FACE);
    }
}

void Scene::update(float dt)
{
    ++m_frameNumber;
    m_dt = dt;
    m_cam->update(dt);
    for (const auto& node : m_nodes)
    {
        node->update(m_dt);
    }
    if (m_pApp->isKeyJustDown('k'))
    {
        m_renderBoundingVolumes = !m_renderBoundingVolumes;
    }
    preformCulling();
}

// geometry is static so only need to update BV's once
void Scene::initScene()
{
    for (const auto& node : m_nodes)
    {
        node->updateBoundingVolume();
    }
    createQuadTree();
}

bool Scene::renderBoundingVolumes() const
{
    return m_renderBoundingVolumes;
}

unsigned int Scene::getFrameNumber() const
{
    return m_frameNumber;
}

// node should return all of its children that are in the frustum.
void Scene::preformCulling()
{
    m_nodesToRender.clear();
    m_quadTree.getNodesInFrustum(m_nodesToRender, m_cam->getFrustum());
    // std::cout << m_nodesToRender.size() << std::endl;
}

// insert top level nodes into quad tree
void Scene::createQuadTree()
{
    for (const auto& node : m_nodes)
    {
        m_quadTree.insert(node);
    }
}
