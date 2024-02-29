//-----------------------------------------------------------------------------
// File:			debugSample.h
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#pragma once
#include "nodes/Cube.h"
#include "nodes/Grid3D.h"
#include "scene.h"
#include "utility.h"

class DebugSample : public Sample
{
public:
    //------------------------------------
    // PUBLIC INTERFACE
    //------------------------------------
    DebugSample(wolf::App* pApp);
    ~DebugSample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;
    void createNodes();
    //------------------------------------

private:
    //------------------------------------
    // PRIVATE MEMBERS
    //------------------------------------
    float m_dt;
    Scene& m_scene;
    //------------------------------------
};
