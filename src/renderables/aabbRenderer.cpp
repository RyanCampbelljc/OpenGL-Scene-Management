//-----------------------------------------------------------------------------
// File:			CubeRenderer.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "renderables/aabbRenderer.h"

// static members
wolf::VertexBuffer* aabbRenderer::s_pVertexBuffer = 0;
wolf::VertexDeclaration* aabbRenderer::s_pVAO = 0;
wolf::IndexBuffer* aabbRenderer::s_pIndexBuffer = 0;
wolf::Material* aabbRenderer::s_pMaterial = 0;
int aabbRenderer::s_refCount = 0;
int aabbRenderer::s_vertexArrLen = 0;
int aabbRenderer::s_bufferArrLen = 0;

//----------------------------------------------------------
// vertices for a base aabb
//----------------------------------------------------------
const aabbRenderer::Vertex aabbRenderer::gs_aabbVertices[] = {
    // Front
    {-0.5f, -0.5f, 0.5f}, // bottom left
    {-0.5f, 0.5f, 0.5f}, // top left
    {0.5f, 0.5f, 0.5f}, // top right
    {0.5f, -0.5f, 0.5f}, // bottom right

    // Back
    {-0.5f, -0.5f, -0.5f}, // bottom left
    {-0.5f, 0.5f, -0.5f}, // top left
    {0.5f, 0.5f, -0.5f}, // top right
    {0.5f, -0.5f, -0.5f}, // bottom right
};

//----------------------------------------------------------
// indices for a base aabb
//----------------------------------------------------------
const GLushort aabbRenderer::gs_aabbIndices[] = {
    0, 1, 0, 3, 1, 2, 2, 3, // front
    3, 2, 7, 6, 2, 6, 3, 7, // right
    5, 1, 4, 0, 5, 4, 1, 0, // left
    5, 6, 1, 2, 5, 1, 6, 2, // top
    0, 4, 3, 7, 0, 3, 4, 7, // bottom
    4, 5, 6, 7, 5, 6, 4, 7 // back

};

aabbRenderer::aabbRenderer()
{
    if (s_refCount == 0)
    {
        initStaticMembers();
    }
    ++s_refCount;
}

aabbRenderer::aabbRenderer(aabbRenderer&& other)
{
    ++s_refCount;
}

aabbRenderer::~aabbRenderer()
{
    --s_refCount;
    if (s_refCount == 0)
    {
        freeBuffers();
    }
}

void aabbRenderer::render(RenderParams& params) const
{
    s_pMaterial->SetUniform("u_mWVP", params.getMWVP());
    s_pMaterial->SetUniform("u_color", params.color);
    s_pMaterial->Apply();
    s_pVAO->Bind();
    // 36 is length of index array
    glDrawElements(GL_LINES, s_bufferArrLen, GL_UNSIGNED_SHORT, 0);
}

void aabbRenderer::freeBuffers()
{
    wolf::MaterialManager::DestroyMaterial(s_pMaterial);
    wolf::BufferManager::DestroyBuffer(s_pVertexBuffer);
    wolf::BufferManager::DestroyBuffer(s_pIndexBuffer);
    delete s_pVAO;
    // std::cout << "Cube buffers freed" << std::endl;
}

void aabbRenderer::initStaticMembers()
{
    s_pMaterial = wolf::MaterialManager::CreateMaterial("aabb");
    s_pMaterial->SetProgram("shaders/cubeShader.vsh", "shaders/cubeShader.fsh");

    s_vertexArrLen = sizeof(gs_aabbVertices) / sizeof(gs_aabbVertices[0]);
    s_bufferArrLen = sizeof(gs_aabbIndices) / sizeof(gs_aabbIndices[0]);
    s_pVertexBuffer = wolf::BufferManager::CreateVertexBuffer(gs_aabbVertices, sizeof(Vertex) * s_vertexArrLen);
    s_pIndexBuffer = wolf::BufferManager::CreateIndexBuffer(gs_aabbIndices, sizeof(gs_aabbIndices) * s_bufferArrLen);

    s_pVAO = new wolf::VertexDeclaration();
    s_pVAO->Begin();
    s_pVAO->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    s_pVAO->SetVertexBuffer(s_pVertexBuffer);
    s_pVAO->SetIndexBuffer(s_pIndexBuffer);
    s_pVAO->End();
}