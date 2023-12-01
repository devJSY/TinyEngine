#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_NextID = 0;

CEntity::CEntity()
    : m_ID(g_NextID++)
{
}

CEntity::~CEntity()
{
}
