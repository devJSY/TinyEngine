#include "pch.h"
#include "CSolarOnceScript.h"

#include "CElevatorScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSolarOnceScript::CSolarOnceScript()
    : CScript(SOLARONCESCRIPT)
    , m_MovingObjName{}
    , m_eState(SolarOnceState::OffWait)
    , m_IsArea(false)
    , m_fTermTime(0.f)
    , m_bOnOffFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarOnceScript::CSolarOnceScript(const CSolarOnceScript& Origin)
    : CScript(Origin)
    , m_MovingObjName{}
    , m_eState(SolarOnceState::OffWait)
    , m_IsArea(false)
    , m_fTermTime(0.f)
    , m_bOnOffFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarOnceScript::~CSolarOnceScript()
{
}

void CSolarOnceScript::begin()
{
    ChangeState(SolarOnceState::OffWait);

    MeshRender()->GetDynamicMaterial(0);
    MeshRender()->GetDynamicMaterial(2);
    MeshRender()->GetDynamicMaterial(3);
    MeshRender()->GetDynamicMaterial(4);
    MeshRender()->GetDynamicMaterial(7);
    MeshRender()->GetDynamicMaterial(8);
    MeshRender()->GetDynamicMaterial(9);
    MeshRender()->GetDynamicMaterial(12);
    MeshRender()->GetDynamicMaterial(13);
    MeshRender()->GetDynamicMaterial(14);
    MeshRender()->GetDynamicMaterial(17);
    MeshRender()->GetDynamicMaterial(18);
}

void CSolarOnceScript::tick()
{
    switch (m_eState)
    {
    case SolarOnceState::ChargeOn: {
        ChargeOn();
    }
    break;
    case SolarOnceState::ChargeOff: {
        ChargeOff();
    }
    break;
    case SolarOnceState::OnWait:
        break;
    case SolarOnceState::OnWaitStart: {

        OnWaitStart();
    }
    break;
    case SolarOnceState::OffWait: {
        OffWait();
    }
    break;
    case SolarOnceState::End:
        break;
    default:
        break;
    }
}

void CSolarOnceScript::ChangeState(SolarOnceState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CSolarOnceScript::EnterState(SolarOnceState _state)
{
    switch (_state)
    {
    case SolarOnceState::ChargeOn: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(2)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(3)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(4)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(9)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(14)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(17)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(18)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        Animator()->Play(ANIMPREFIX("Charge"), false, false, 1.5f);
    }
    break;
    case SolarOnceState::ChargeOff: {
        Animator()->SetReverse(true);
    }
    break;
    case SolarOnceState::OnWait: {
        Animator()->Play(ANIMPREFIX("OnWait"), false);
    }
    break;
    case SolarOnceState::OnWaitStart: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(2)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(3)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(4)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(9)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(14)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(17)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(18)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));

        CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(ToWstring(m_MovingObjName));
        if (nullptr != pObj)
            pObj->GetScript<CElevatorScript>()->SetState(ElevatorState::Move);
        else
        {
            string tmp = string("Not Exist Elevator");
            LOG(LOG_LEVEL::Log, tmp.c_str());
        }

        Animator()->Play(ANIMPREFIX("OnWaitStart"), false);
    }
    break;
    case SolarOnceState::OffWait: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(2)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(3)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(4)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(9)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(14)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(17)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(18)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));

        Animator()->Play(ANIMPREFIX("OffWait"), false);
    }
    break;
    case SolarOnceState::End:
        break;
    default:
        break;
    }
}

void CSolarOnceScript::ExitState(SolarOnceState _state)
{
    switch (_state)
    {
    case SolarOnceState::ChargeOn: {
        m_fTermTime = 0.f;
        m_bOnOffFlag = false;
    }
    break;
    case SolarOnceState::ChargeOff:
        break;
    case SolarOnceState::OnWait:
        break;
    case SolarOnceState::OnWaitStart:
        break;
    case SolarOnceState::OffWait:
        break;
    case SolarOnceState::End:
        break;
    default:
        break;
    }
}

void CSolarOnceScript::ChargeOn()
{
    if (!m_IsArea || !PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarOnceState::ChargeOff);
    }
    else
    {
        m_fTermTime += DT * 2.f;
        if (m_bOnOffFlag)
        {
            if (m_fTermTime >= 1.f)
            {
                m_bOnOffFlag = false;
                m_fTermTime = 0.f;
            }
            else
            {
                GetOwner()->MeshRender()->GetMaterial(0)->SetEmission(Vec4(m_fTermTime, m_fTermTime, 0.f, 1.f));
            }
        }
        else
        {
            if (m_fTermTime >= 1.f)
            {
                m_bOnOffFlag = true;
                m_fTermTime = 0.f;
            }
            else
            {
                GetOwner()->MeshRender()->GetMaterial(0)->SetEmission(Vec4((1.f - m_fTermTime), (1.f - m_fTermTime), 0.f, 1.f));
            }
        }
    }

    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OnWaitStart);
    }
}

void CSolarOnceScript::ChargeOff()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OffWait);
    }
}

void CSolarOnceScript::OnWaitStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarOnceState::OnWait);
    }
}

void CSolarOnceScript::OffWait()
{
    if (m_IsArea && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarOnceState::ChargeOn);
    }
}

void CSolarOnceScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = true;
    }
}

void CSolarOnceScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = false;
    }
}

UINT CSolarOnceScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = ToWstring(m_MovingObjName);

    MemoryByte += SaveWStringToFile(wStr, _File);

    return MemoryByte;
}

UINT CSolarOnceScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = {};
    MemoryByte += LoadWStringFromFile(wStr, _File);

    m_MovingObjName = ToString(wStr);

    return MemoryByte;
}