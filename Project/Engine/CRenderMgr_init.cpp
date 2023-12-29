#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

void CRenderMgr::init()
{
    m_pDebugObj = new CGameObject;
    m_pDebugObj->AddComponent(new CTransform);
    m_pDebugObj->AddComponent(new CMeshRender);
}