#include "pch.h"
#include "CStageClear.h"

#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"
#include "CLevelFlowMgr.h"

CStageClear::CStageClear()
    : CScript(STAGECLEAR)
    , m_bKirbyDance(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bKirbyDance, "Kirby Dance");
}

CStageClear::CStageClear(const CStageClear& _Origin)
    : CScript(_Origin)
    , m_bKirbyDance(_Origin.m_bKirbyDance)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bKirbyDance, "Kirby Dance");
}

CStageClear::~CStageClear()
{
}

UINT CStageClear::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bKirbyDance, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CStageClear::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bKirbyDance, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

void CStageClear::OnTriggerEnter(CCollider* _OtherCollider)
{
    // PLAYER와 충돌한 경우
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {
        if (m_bKirbyDance)
        {
            PLAYERCTRL->TeleportGround();
            PLAYERFSM->ChangeState(L"STAGE_CLEAR");
        }
        else
        {
            CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

            if (nullptr != Manager)
            {
                CLevelFlowMgr* FlowMgr = Manager->GetScript<CLevelFlowMgr>();
                if (FlowMgr != nullptr)
                {
                    FlowMgr->LevelEnd();
                }
            }
        }

    }


}

void CStageClear::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CStageClear::OnTriggerExit(CCollider* _OtherCollider)
{
}
