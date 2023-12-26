#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_NextID = 0;

CEntity::CEntity()
    : m_ID(g_NextID++)
{
}

CEntity::CEntity(const CEntity& origin)
    : m_ID(g_NextID++)
    , m_strName(origin.m_strName)
{
}

CEntity::~CEntity()
{
}
