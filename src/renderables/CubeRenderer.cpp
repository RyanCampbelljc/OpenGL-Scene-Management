//-----------------------------------------------------------------------------
// File:			CubeRenderer.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "CubeRenderer.h"

// static members
wolf::VertexBuffer* CubeRenderer::s_pVertexBuffer = 0;
wolf::VertexDeclaration* CubeRenderer::s_pVAO = 0;
wolf::IndexBuffer* CubeRenderer::s_pIndexBuffer = 0;
wolf::Material* CubeRenderer::s_pMaterial = 0;
int CubeRenderer::s_refCount = 0;
int CubeRenderer::s_vertexArrLen = 0;
int CubeRenderer::s_bufferArrLen = 0;
//----------------------------------------------------------
// vertices for a base cube
//----------------------------------------------------------
const CubeRenderer::Vertex CubeRenderer::gs_cubeVertices[] = {
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
// indices for a base cube
//----------------------------------------------------------
const GLushort CubeRenderer::gs_cubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    3, 2, 6, 6, 7, 3, // right
    4, 5, 1, 1, 0, 4, // left
    1, 5, 6, 6, 2, 1, // top
    4, 0, 3, 3, 7, 4, // bottom
    7, 6, 5, 5, 4, 7, // back

};

CubeRenderer::CubeRenderer()
{
    if (s_refCount == 0)
    {
        initStaticMembers();
    }
    ++s_refCount;
}

CubeRenderer::CubeRenderer(CubeRenderer&& other)
{
    ++s_refCount;
}

CubeRenderer::~CubeRenderer()
{
    --s_refCount;
    if (s_refCount == 0)
    {
        freeBuffers();
    }
}

void CubeRenderer::render(RenderParams& params) const
{
    s_pMaterial->SetUniform("u_mWVP", params.getMWVP());
    s_pMaterial->SetUniform("u_color", params.color);
    s_pMaterial->Apply();
    s_pVAO->Bind();
    // 36 is length of index array
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, s_bufferArrLen, GL_UNSIGNED_SHORT, 0);
}

void CubeRenderer::freeBuffers()
{
    wolf::MaterialManager::DestroyMaterial(s_pMaterial);
    wolf::BufferManager::DestroyBuffer(s_pVertexBuffer);
    wolf::BufferManager::DestroyBuffer(s_pIndexBuffer);
    delete s_pVAO;
    std::cout << "Cube buffers freed" << std::endl;
}

void CubeRenderer::initStaticMembers()
{
    s_pMaterial = wolf::MaterialManager::CreateMaterial("Cube");
    s_pMaterial->SetProgram("shaders/cubeShader.vsh", "shaders/cubeShader.fsh");

    s_vertexArrLen = sizeof(gs_cubeVertices) / sizeof(gs_cubeVertices[0]);
    s_bufferArrLen = sizeof(gs_cubeIndices) / sizeof(gs_cubeIndices[0]);
    s_pVertexBuffer = wolf::BufferManager::CreateVertexBuffer(gs_cubeVertices, sizeof(Vertex) * s_vertexArrLen);
    s_pIndexBuffer = wolf::BufferManager::CreateIndexBuffer(gs_cubeIndices, sizeof(gs_cubeIndices) * s_bufferArrLen);

    s_pVAO = new wolf::VertexDeclaration();
    s_pVAO->Begin();
    s_pVAO->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    s_pVAO->SetVertexBuffer(s_pVertexBuffer);
    s_pVAO->SetIndexBuffer(s_pIndexBuffer);
    s_pVAO->End();
}