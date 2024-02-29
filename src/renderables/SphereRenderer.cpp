//-----------------------------------------------------------------------------
// File:			SphereRenderer.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "SphereRenderer.h"

wolf::VertexBuffer* SphereRenderer::s_pVertexBuffer = nullptr;
wolf::IndexBuffer* SphereRenderer::s_pIndexBuffer = nullptr;
wolf::VertexDeclaration* SphereRenderer::s_pVAO = nullptr;
int SphereRenderer::s_numIndices = 0;
int SphereRenderer::s_refCount = 0;
wolf::Material* SphereRenderer::s_pMaterial = nullptr;

struct SphereVertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};
SphereRenderer::SphereRenderer()
{
    if (s_refCount == 0)
    {
        s_pMaterial = wolf::MaterialManager::CreateMaterial("__sphereMat");
        s_pMaterial->SetProgram("shaders/sphere.vsh", "shaders/sphere.fsh");
        s_pMaterial->SetDepthTest(true);
        s_pMaterial->SetDepthWrite(true);
        s_pMaterial->SetBlend(true);
        s_pMaterial->SetBlendMode(wolf::BlendMode::BM_SrcAlpha, wolf::BlendMode::BM_OneMinusSrcAlpha);
        _genVerts(20, 20);
    }
    ++s_refCount;
}

SphereRenderer::SphereRenderer(SphereRenderer&& other)
{
    ++s_refCount; // account for other object getting destroyed
}

SphereRenderer::~SphereRenderer()
{
    --s_refCount;
    if (s_refCount == 0)
    {
        wolf::MaterialManager::DestroyMaterial(s_pMaterial);
        wolf::BufferManager::DestroyBuffer(s_pVertexBuffer);
        wolf::BufferManager::DestroyBuffer(s_pIndexBuffer);
        delete s_pVAO;
    }
}

void SphereRenderer::render(RenderParams& params) const
{
    glm::mat4 world = params.getWorld();
    s_pMaterial->SetUniform("projection", params.projection);
    s_pMaterial->SetUniform("view", params.view);
    s_pMaterial->SetUniform("world", world);
    s_pMaterial->SetUniform("worldIT", glm::transpose(glm::inverse(world)));
    s_pMaterial->SetUniform("u_color", params.color);
    s_pMaterial->Apply();

    s_pVAO->Bind();
    glDrawElements(GL_TRIANGLES, s_numIndices, GL_UNSIGNED_SHORT, 0);
}

void SphereRenderer::_genVerts(int sectorCount, int stackCount)
{
    SphereVertex v;
    std::vector<SphereVertex> verts;
    std::vector<unsigned short> indices;

    float x, y, z, xy; // vertex position
    float nx, ny, nz, lengthInv = 1.0f / 0.5f; // vertex normal
    float s, t; // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = 0.5f * cosf(stackAngle); // r * cos(u)
        z = 0.5f * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            v.x = x;
            v.y = y;
            v.z = z;
            v.nx = nx;
            v.ny = ny;
            v.nz = nz;
            v.u = s;
            v.v = t;

            verts.push_back(v);
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    s_numIndices = (int)indices.size();

    s_pVertexBuffer = wolf::BufferManager::CreateVertexBuffer(
        verts.data(), (unsigned int)(sizeof(SphereVertex) * verts.size()));
    s_pIndexBuffer = wolf::BufferManager::CreateIndexBuffer(indices.data(), s_numIndices);

    s_pVAO = new wolf::VertexDeclaration();
    s_pVAO->Begin();
    s_pVAO->SetVertexBuffer(s_pVertexBuffer);
    s_pVAO->SetIndexBuffer(s_pIndexBuffer);
    s_pVAO->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    s_pVAO->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    s_pVAO->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    s_pVAO->End();
}
