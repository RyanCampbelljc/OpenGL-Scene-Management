//-----------------------------------------------------------------------------
// File:			CubeRenderer.h
// Original Author:	Ryan Campbell
// Everything necessary to render a cube
//-----------------------------------------------------------------------------
#pragma once
#include "renderable.h"
#include "utility.h"
class aabbRenderer : public Renderable
{
public:
    //----------------------------------------------------------
    // PUBLIC INTERFACE
    //----------------------------------------------------------
    aabbRenderer();
    aabbRenderer(aabbRenderer&) = delete;
    aabbRenderer(aabbRenderer&& other);
    ~aabbRenderer();

    void render(RenderParams& params) const override;

    //----------------------------------------------------------
private:
    void freeBuffers();
    void initStaticMembers();

private:
    struct Vertex
    {
        GLfloat x, y, z;
    };
    static wolf::VertexBuffer* s_pVertexBuffer;
    static wolf::VertexDeclaration* s_pVAO;
    static wolf::IndexBuffer* s_pIndexBuffer;
    static wolf::Material* s_pMaterial;
    static int s_refCount;
    static int s_bufferArrLen;
    static int s_vertexArrLen;
    static const Vertex gs_aabbVertices[];
    static const GLushort gs_aabbIndices[];
};
