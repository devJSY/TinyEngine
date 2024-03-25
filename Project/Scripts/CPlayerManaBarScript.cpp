#include "pch.h"
#include "CPlayerManaBarScript.h"

#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include "CPlayerScript.h"

CPlayerManaBarScript::CPlayerManaBarScript()
    : CScript(PLAYERMANABARSCRIPT)
    , m_pPlayer(nullptr)
    , m_RenderMana(0.f)
    , m_IncreaseSpeed(20.f)
    , m_DecreaseSpeed(100.f)
{
}

CPlayerManaBarScript::~CPlayerManaBarScript()
{
}

void CPlayerManaBarScript::begin()
{
    if (MeshRender())
        MeshRender()->CreateDynamicMaterial();

    m_pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");

    if (nullptr == m_pPlayer)
        return;

    CPlayerScript* pPlyaerScript = m_pPlayer->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    m_RenderMana = (float)pPlyaerScript->GetCurMana();
}

void CPlayerManaBarScript::tick()
{
    if (nullptr == m_pPlayer)
        return;

    CPlayerScript* pPlyaerScript = m_pPlayer->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    if (nullptr == MeshRender() || nullptr == MeshRender()->GetMaterial())
        return;

    float MaxMana = (float)pPlyaerScript->GetMaxMana();
    float CurMana = (float)pPlyaerScript->GetCurMana();

    if (fabsf(m_RenderMana - CurMana) < 0.1f)
    {
        m_RenderMana = CurMana;
    }
    else if (m_RenderMana < CurMana)
    {
        m_RenderMana += m_IncreaseSpeed * DT;
    }
    else if (m_RenderMana > CurMana)
    {
        m_RenderMana -= m_DecreaseSpeed * DT;
    }

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, MaxMana);
    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_1, m_RenderMana);
    MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.f, 1.f, 0.f, 1.f));
}
