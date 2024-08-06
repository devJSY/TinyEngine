#include "pch.h"
#include "CSurpriseBoardScript.h"

CSurpriseBoardScript::CSurpriseBoardScript()
    : CScript(SURPRISEBOARDSCRIPT)
    , m_pWallColObject(nullptr)
    , m_pAtkObject1(nullptr)
    , m_pAtkObject2(nullptr)
    , m_eState(SurpriseBoardState::Wait)
    , m_bIsRight(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsRight, "IsRight");
}

CSurpriseBoardScript::CSurpriseBoardScript(const CSurpriseBoardScript& Origin)
    : CScript(Origin)
    , m_pWallColObject(nullptr)
    , m_pAtkObject1(nullptr)
    , m_pAtkObject2(nullptr)
    , m_eState(SurpriseBoardState::Wait)
    , m_bIsRight(Origin.m_bIsRight)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsRight, "IsRight");
}

CSurpriseBoardScript::~CSurpriseBoardScript()
{
}

void CSurpriseBoardScript::begin()
{
    ChangeState(SurpriseBoardState::Wait);
    CGameObject* pObj = GetOwner()->GetChildObject(L"SurpriseBoardColSet");
    m_pWallColObject = pObj != nullptr ? pObj->GetChildObject(L"SurpriseBoardColWall") : nullptr;
    m_pAtkObject1 = pObj != nullptr ? pObj->GetChildObject(L"SurpriseBoardColAttack1") : nullptr;
    m_pAtkObject2 = pObj != nullptr ? pObj->GetChildObject(L"SurpriseBoardColAttack2") : nullptr;

    nullptr != m_pWallColObject ? m_pWallColObject->BoxCollider()->SetEnabled(false) : void();
    nullptr != m_pAtkObject1 ? m_pAtkObject1->BoxCollider()->SetEnabled(false) : void();
    nullptr != m_pAtkObject2 ? m_pAtkObject2->BoxCollider()->SetEnabled(false) : void();
}

void CSurpriseBoardScript::tick()
{
    switch (m_eState)
    {
    case SurpriseBoardState::ArmMove:
    {
        ArmMove();
    }
        break;
    case SurpriseBoardState::PopOut:
    {
        PopOut();
    }
        break;
    case SurpriseBoardState::PrePopOut:
    {
        PrePopOut();
    }
        break;
    case SurpriseBoardState::Return:
    {
        Return();
    }
        break;
    case SurpriseBoardState::End:
        break;
    default:
        break;
    }
}

void CSurpriseBoardScript::ChangeState(SurpriseBoardState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CSurpriseBoardScript::EnterState(SurpriseBoardState _state)
{
    switch (m_eState)
    {
    case SurpriseBoardState::ArmMove: {
        true == m_bIsRight ? Animator()->Play(ANIMPREFIX("ArmMoveR"), false) : Animator()->Play(ANIMPREFIX("ArmMoveL"), false);
    }
    break;
    case SurpriseBoardState::Break:
        break;
    case SurpriseBoardState::PopOut: {
        true == m_bIsRight ? Animator()->Play(ANIMPREFIX("PopOutR"), false) : Animator()->Play(ANIMPREFIX("PopOutL"), false);
    }
    break;
    case SurpriseBoardState::PrePopOut: {
        m_pWallColObject->BoxCollider()->SetEnabled(true);
        true == m_bIsRight ? m_pAtkObject2->BoxCollider()->SetEnabled(true) : m_pAtkObject1->BoxCollider()->SetEnabled(true);

        true == m_bIsRight ? Animator()->Play(ANIMPREFIX("PrePopOutR"), false) : Animator()->Play(ANIMPREFIX("PrePopOutL"), false);
    }
    break;
    case SurpriseBoardState::Return: {
        true == m_bIsRight ? Animator()->Play(ANIMPREFIX("ReturnR"), false) : Animator()->Play(ANIMPREFIX("ReturnL"), false);
    }
    break;
    case SurpriseBoardState::Wait: {
        true == m_bIsRight ? Animator()->Play(ANIMPREFIX("WaitR"), false) : Animator()->Play(ANIMPREFIX("WaitL"), false);
    }
    break;
    case SurpriseBoardState::End:
        break;
    default:
        break;
    }
}

void CSurpriseBoardScript::ExitState(SurpriseBoardState _state)
{
    switch (m_eState)
    {
    case SurpriseBoardState::ArmMove:
        break;
    case SurpriseBoardState::Break:
        break;
    case SurpriseBoardState::PopOut: {
        true == m_bIsRight ? m_pAtkObject2->BoxCollider()->SetEnabled(false) : m_pAtkObject1->BoxCollider()->SetEnabled(false);
    }
    break;
    case SurpriseBoardState::PrePopOut:
        break;
    case SurpriseBoardState::Return: {
        m_pWallColObject->BoxCollider()->SetEnabled(false);
    }
    break;
    case SurpriseBoardState::Wait:
        break;
    case SurpriseBoardState::End:
        break;
    default:
        break;
    }
}

void CSurpriseBoardScript::ArmMove()
{
    if (true == Animator()->IsFinish())
    {
        ChangeState(SurpriseBoardState::Return);
    }
}

void CSurpriseBoardScript::PopOut()
{
    if (true == Animator()->IsFinish())
    {
        ChangeState(SurpriseBoardState::ArmMove);
    }
}

void CSurpriseBoardScript::PrePopOut()
{
    if (true == Animator()->IsFinish())
    {
        ChangeState(SurpriseBoardState::PopOut);
    }
}

void CSurpriseBoardScript::Return()
{
    if (true == Animator()->IsFinish())
    {
        ChangeState(SurpriseBoardState::Wait);
    }
}

void CSurpriseBoardScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (L"Main Player" == Obj->GetName())
    {
        SurpriseBoardState::Wait == m_eState ? ChangeState(SurpriseBoardState::PrePopOut) : void();
    }
}

UINT CSurpriseBoardScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bIsRight, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CSurpriseBoardScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bIsRight, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);

    return MemoryByte;
}