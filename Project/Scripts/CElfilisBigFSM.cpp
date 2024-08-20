#include "pch.h"
#include "CElfilisBigFSM.h"
#include "CBossMgr.h"

CElfilisBigFSM::CElfilisBigFSM()
    : CFSMScript(ELFILISBIGFSM)
    , m_ComboLevel(0)
    , m_PositionOffset(1500.f)
    , m_ReverseState(L"")
{
}

CElfilisBigFSM::CElfilisBigFSM(const CElfilisBigFSM& _Origin)
    : CFSMScript(_Origin)
    , m_ComboLevel(0)
    , m_PositionOffset(_Origin.m_PositionOffset)
    , m_ReverseState(L"")
{
}

CElfilisBigFSM::~CElfilisBigFSM()
{
}

#include "CElfilisBig_Idle.h"
#include "CElfilisBig_Appear.h"
#include "CElfilisBig_Roar.h"
#include "CElfilisBig_SwingR.h"
#include "CElfilisBig_SwingL.h"
#include "CElfilisBig_SwingFinishL.h"
void CElfilisBigFSM::begin()
{
    AddState(L"IDLE", new CElfilisBig_Idle);
    AddState(L"APPEAR", new CElfilisBig_Appear);
    AddState(L"ROAR", new CElfilisBig_Roar);
    AddState(L"SWING_R", new CElfilisBig_SwingR);
    AddState(L"SWING_L", new CElfilisBig_SwingL);
    AddState(L"SWING_FINISHL", new CElfilisBig_SwingFinishL);

    ChangeState(L"IDLE");

    // Set Mtrl
    int MtrlCount = MeshRender()->GetMtrlCount();
    for (int i = 0; i < MtrlCount; ++i)
    {
        m_vecMtrls.push_back(MeshRender()->GetMaterial(i));
    }
    
    CGameObject* Weapon = GetOwner()->GetChildObject(L"Halberd");
    if (Weapon)
    {
        MtrlCount = Weapon->MeshRender()->GetMtrlCount();
        for (int i = 0; i < MtrlCount; ++i)
        {
            m_vecMtrls.push_back(Weapon->MeshRender()->GetMaterial(i));
        }
    }
    
    GetOwner()->SetActive(false);
}

void CElfilisBigFSM::Activate()
{
    Vec3 NewDir = BOSS->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    NewDir.y = 0.f;
    NewDir.Normalize();

    Vec3 NewPos = NewDir * -1.f;
    NewPos *= m_PositionOffset;

    Transform()->SetWorldPos(NewPos);
    Transform()->Slerp(NewDir, 1.f);

    GetOwner()->SetActive(true);
    SetMtrlTransparent(0.6f);

    ChangeState(L"IDLE");
}

void CElfilisBigFSM::ChangeState(const wstring& _strState)
{
    if (m_bGlobalState)
        return;

    // 1. next state check
    CState* pNextState = nullptr;

    if (!m_ReverseState.empty())
    {
        pNextState = FindState(m_ReverseState);
        m_ReverseState = L"";
    }
    
    if (!pNextState)
    {
        pNextState = FindState(_strState);
    }

    assert(pNextState);

    ChangeState_proc(pNextState);
}

void CElfilisBigFSM::SetMtrlTransparent(float _Alpha)
{
    for (Ptr<CMaterial> iter : m_vecMtrls)
    {
        iter->SetScalarParam(SCALAR_PARAM::FLOAT_2, _Alpha);
    }
}

UINT CElfilisBigFSM::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisBigFSM::LoadFromLevelFile(FILE* _File)
{
    return 0;
}