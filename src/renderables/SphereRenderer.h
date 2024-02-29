//-----------------------------------------------------------------------------
// File:			SphereRenderer.h
// Original Author:	Ryan Campbell
// Everything necessary to a render sphere
//-----------------------------------------------------------------------------
#pragma once
#include "renderable.h"
#include "wolf.h"
class SphereRenderer : public Renderable
{
public:
    SphereRenderer();
    SphereRenderer(SphereRenderer&& other);
    SphereRenderer(SphereRenderer&) = delete;
    ~SphereRenderer();

    void render(RenderParams& params) const override;

private:
    void _genVerts(int sectorCount, int stackCount);

    static wolf::VertexBuffer* s_pVertexBuffer;
    static wolf::IndexBuffer* s_pIndexBuffer;
    static wolf::VertexDeclaration* s_pVAO;
    static int s_numIndices;
    static int s_refCount;
    static wolf::Material* s_pMaterial;
};