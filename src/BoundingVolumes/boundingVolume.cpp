//-----------------------------------------------------------------------------
// File:			BoundingVolume.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "BoundingVolumes/boundingVolume.h"
BoundingVolumeBase::BoundingVolumeBase(const glm::vec3& pos)
    : m_pos(pos)
{
}

BoundingVolumeBase::~BoundingVolumeBase()
{
    // delete m_BVOutline
}

void BoundingVolumeBase::setPosition(const glm::vec3& pos)
{
    m_pos = pos;
}

glm::vec3 BoundingVolumeBase::getPosition() const
{
    return m_pos;
}
