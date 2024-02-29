//-----------------------------------------------------------------------------
// File:			DebugSample.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "DebugSample.h"
#include "../samplefw/Grid2D.h"
#include <exception>
#include <iostream>
#include <string>

DebugSample::DebugSample(wolf::App* pApp)
    : Sample(pApp, "Scene One")
    , m_dt(0)
    , m_scene(Scene::Instance())
{
}

DebugSample::~DebugSample()
{
}

// During init enable debug output
void DebugSample::init()
{
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    createNodes();
    // only needs to be called once after all the nodes are added
    m_scene.initScene();
}

void DebugSample::update(float dt)
{
    m_dt = dt;
    m_scene.update(dt);
}

void DebugSample::render(int width, int height)
{
    glClearColor(0.5, 0.5, 0.5, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene.renderScene(width, height);
}

void DebugSample::createNodes()
{
    Grid3D* grid = new Grid3D(1);
    m_scene.addNode(grid);
    // 3  level heiarchy for debugging
    // glm::vec3 color = glm::vec3(255, 0, 0);
    // glm::vec3 pos(5, 0, 0);
    // Cube* parent = new Cube(pos, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
    // parent->setColor(color);
    // m_scene.addNode(parent);

    // Cube* child = new Cube(glm::vec3(3, 3, 3), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
    // child->setColor(color);
    // parent->addChild(child);

    // Cube* gc = new Cube(glm::vec3(7, 7, 7), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
    // gc->setColor(color);
    // child->addChild(gc);

    for (int i = 0; i < 100; ++i)
    {
        glm::vec3 color(Utility::randomFloat(0, 1), Utility::randomFloat(0, 1), Utility::randomFloat(0, 1));
        glm::vec3 pos(Utility::randomFloat(-80, 80), Utility::randomFloat(-20, 20), Utility::randomFloat(-80, 80));
        glm::vec3 scale(Utility::randomFloat(0.5, 3), Utility::randomFloat(0.5, 3), Utility::randomFloat(0.5, 3));
        glm::vec3 rot(Utility::randomFloat(-2, 2), Utility::randomFloat(-2, 2), Utility::randomFloat(-2, 2));
        Cube* c = new Cube(pos, scale, rot);
        c->setColor(color);
        for (int j = 0; j < 3; ++j)
        {
            glm::vec3 childPos = glm::vec3(
                Utility::randomFloat(-5, 5), Utility::randomFloat(-5, 5), Utility::randomFloat(-5, 5));
            scale = glm::vec3(Utility::randomFloat(0.5, 3), Utility::randomFloat(0.5, 3), Utility::randomFloat(0.5, 3));
            rot = glm::vec3(Utility::randomFloat(-2, 2), Utility::randomFloat(-2, 2), Utility::randomFloat(-2, 2));
            Cube* child = new Cube(childPos, scale, rot);
            child->setColor(color);
            c->addChild(child);
        }
        m_scene.addNode(c);
    }
}