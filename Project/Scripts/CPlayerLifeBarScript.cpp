#include "pch.h"
#include "CPlayerLifeBarScript.h"

#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include "CPlayerScript.h"

CPlayerLifeBarScript::CPlayerLifeBarScript()
    : CScript(PLAYERLIFEBARSCRIPT)
    , m_pPlayer(nullptr)
    , m_RenderLife(0.f)
    , m_FluctuationSpeed(30.f)
{
}

CPlayerLifeBarScript::~CPlayerLifeBarScript()
{
}

void CPlayerLifeBarScript::begin()
{
    if (MeshRender())
        MeshRender()->CreateDynamicMaterial();

    m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");

    if (nullptr == m_pPlayer)
        return;

    CPlayerScript* pPlyaerScript = m_pPlayer->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    m_RenderLife = (float)pPlyaerScript->GetCurLife();
}

void CPlayerLifeBarScript::tick()
{
    if (nullptr == m_pPlayer)
        return;

    CPlayerScript* pPlyaerScript = m_pPlayer->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial())
        return;

    float MaxLife = (float)pPlyaerScript->GetMaxLife();
    float CurLife = (float)pPlyaerScript->GetCurLife();

    if (fabsf(m_RenderLife - CurLife) < 0.1f)
    {
        m_RenderLife = CurLife;
    }
    else if (m_RenderLife < CurLife)
    {
        m_RenderLife += m_FluctuationSpeed * DT;
    }
    else if (m_RenderLife > CurLife)
    {
        m_RenderLife -= m_FluctuationSpeed * DT;
    }

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxLife);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, m_RenderLife);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
}
