#include "pch.h"
#include "CCinematicScript.h"

#include "CPlayerScript.h"
#include "CPlayerCameraScript.h"

CCinematicScript::CCinematicScript()
    : CScript(CINEMATICSCRIPT)
    , m_bTrigger(true)
    , m_ActionDuration(3.f)
    , m_PassedTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ActionDuration, "Action Duration");
}

CCinematicScript::CCinematicScript(const CCinematicScript& origin)
    : CScript(origin)
    , m_bTrigger(origin.m_bTrigger)
    , m_ActionDuration(origin.m_ActionDuration)
    , m_PassedTime(origin.m_PassedTime)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ActionDuration, "Action Duration");
}

CCinematicScript::~CCinematicScript()
{
}

void CCinematicScript::begin()
{
    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (UINT i = 0; i < vecChild.size(); i++)
    {
        if (nullptr != vecChild[i]->BoxCollider2D())
        {
            vecChild[i]->BoxCollider2D()->SetEnabled(false);
        }
    }
}

void CCinematicScript::tick()
{
    if (!m_bTrigger)
    {
        m_PassedTime += DT;

        if (m_PassedTime < m_ActionDuration)
        {
            // 화면 효과 활성화
        }
        else
        {
            // 화면 효과 비활성화
            CGameObject* pCinematicPostProcess = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"CinematicPostProcess");
            if (nullptr != pCinematicPostProcess)
            {
                pCinematicPostProcess->GetRenderComponent()->GetMaterial()->SetScalarParam(INT_0, 0);
            }

            CPlayerScript* pPlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
            if (nullptr != pPlayerScript)
            {
                pPlayerScript->ChangeState(PLAYER_STATE::Idle);
            }
            GamePlayStatic::RemoveComponent(GetOwner(), COMPONENT_TYPE::BOXCOLLIDER2D);
            m_bTrigger = true;
        }
    }
}

void CCinematicScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (!m_bTrigger || L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    m_bTrigger = false;
    m_PassedTime = 0.f;

    CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
    if (nullptr != pPlayerCamObj)
    {
        CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
        if (nullptr != pScript)
        {
            pScript->Lock(true, Transform()->GetWorldPos());
        }
    }

    CGameObject* pCinematicPostProcess = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"CinematicPostProcess");
    if (nullptr != pCinematicPostProcess)
    {
        pCinematicPostProcess->GetRenderComponent()->GetMaterial()->SetScalarParam(INT_0, 1);
    }

    CPlayerScript* pPlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr != pPlayerScript)
    {
        pPlayerScript->ChangeState(PLAYER_STATE::Cinematic);
    }

    const vector<CGameObject*>& vecChild = GetOwner()->GetChildObject();
    for (UINT i = 0; i < vecChild.size(); i++)
    {
        if (nullptr != vecChild[i]->BoxCollider2D())
        {
            vecChild[i]->BoxCollider2D()->SetEnabled(true);
        }
    }
}

void CCinematicScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_ActionDuration, sizeof(float), 1, _File);
}

void CCinematicScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_ActionDuration, sizeof(float), 1, _File);
}
