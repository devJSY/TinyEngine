#include "pch.h"
#include "CStageClear.h"

#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"
#include "CLevelFlowMgr.h"

#include "CCameraController.h"

CStageClear::CStageClear()
    : CScript(STAGECLEAR)
    , m_bKirbyDance(false)
    , m_KirbyPos(Vec3())
    , m_KirbyDir(Vec3())
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bKirbyDance, "Kirby Dance");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_KirbyPos, "Kirby Dance Pos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_KirbyDir, "Kirby Dance Rot");
}

CStageClear::CStageClear(const CStageClear& _Origin)
    : CScript(_Origin)
    , m_bKirbyDance(_Origin.m_bKirbyDance)
    , m_KirbyPos(_Origin.m_KirbyPos)
    , m_KirbyDir(_Origin.m_KirbyDir)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bKirbyDance, "Kirby Dance");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_KirbyPos, "Kirby Dance Pos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_KirbyDir, "Kirby Dance Rot");
}

CStageClear::~CStageClear()
{
}

UINT CStageClear::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bKirbyDance, sizeof(bool), 1, _File);
    fwrite(&m_KirbyPos, sizeof(Vec3), 1, _File);
    fwrite(&m_KirbyDir, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

UINT CStageClear::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bKirbyDance, sizeof(bool), 1, _File);
    fread(&m_KirbyPos, sizeof(Vec3), 1, _File);
    fread(&m_KirbyDir, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

void CStageClear::OnTriggerEnter(CCollider* _OtherCollider)
{
    // PLAYER와 충돌한 경우
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {

        // Object 상태일 때는 Drop UI를 띄워주고 스테이트를 바꾸지 않는다.
        if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
        {
            CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

            CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();

            FlowMgrScript->TurnOnDropUI();

            return;
        }

        if (m_bKirbyDance)
        {
            m_KirbyDir.y = 0.f;
            m_KirbyDir.Normalize();

            CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
            if (nullptr != ManagerObj)
            {
                CLevelFlowMgr* pScript = ManagerObj->GetScript<CLevelFlowMgr>();
                if (nullptr != pScript)
                {
                    pScript->AbsorbUIEndOff();
                }
            }

            PLAYERCTRL->ForcePos(m_KirbyPos);
            PLAYERCTRL->ForceDir({ForceDirType::STAGEEVENT, m_KirbyDir, true});
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
    // PLAYER와 충돌한 경우
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {
        // Object상태가 None이 아니라면 DropUI를 띄운다.
        if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
        {
            CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

            CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();

            FlowMgrScript->TurnOffDropUI();

            return;
        }
    }
}
