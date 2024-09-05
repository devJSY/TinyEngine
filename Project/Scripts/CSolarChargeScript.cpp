#include "pch.h"
#include "CSolarChargeScript.h"

#include "CPushOutScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSolarChargeScript::CSolarChargeScript()
    : CScript(SOLARCHARGESCRIPT)
    , m_pObj(nullptr)
    , m_MovingObjName{}
    , m_eState(SolarChargeState::OffWaitStart)
    , m_IsArea(false)
    , m_fTermTime(0.f)
    , m_bOnOffFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarChargeScript::CSolarChargeScript(const CSolarChargeScript& Origin)
    : CScript(Origin)
    , m_pObj(nullptr)
    , m_MovingObjName{}
    , m_eState(SolarChargeState::OffWaitStart)
    , m_IsArea(false)
    , m_fTermTime(0.f)
    , m_bOnOffFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_MovingObjName, "Moving Object Name");
}

CSolarChargeScript::~CSolarChargeScript()
{
}

void CSolarChargeScript::begin()
{
    m_pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(ToWstring(m_MovingObjName));
    ChangeState(SolarChargeState::OffWaitStart);

    GetOwner()->MeshRender()->GetDynamicMaterial(1);
    GetOwner()->MeshRender()->GetDynamicMaterial(6);
    GetOwner()->MeshRender()->GetDynamicMaterial(7);
    GetOwner()->MeshRender()->GetDynamicMaterial(8);
    GetOwner()->MeshRender()->GetDynamicMaterial(12);
    GetOwner()->MeshRender()->GetDynamicMaterial(13);
}

void CSolarChargeScript::tick()
{
    switch (m_eState)
    {
    case SolarChargeState::OffWait: {
        OffWait();
    }
    break;
    case SolarChargeState::ChargeOn: {
        ChargeOn();
    }
    break;
    case SolarChargeState::ChargeOff: {
        ChargeOff();
    }
    break;
    case SolarChargeState::ChargedStart: {
        ChargedStart();
    }
    break;
    case SolarChargeState::ChargedWait:
        break;
    case SolarChargeState::Decreases: {
        Decrease();
    }
    break;
    case SolarChargeState::OffWaitStart: {
        OffWaitStart();
    }
    break;
    case SolarChargeState::End:
        break;
    default:
        break;
    }
}

void CSolarChargeScript::ChangeState(SolarChargeState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CSolarChargeScript::EnterState()
{
    Vec3 vWorldPos = Transform()->GetWorldPos();
    switch (m_eState)
    {
    case SolarChargeState::OffWait: {
        Animator()->Play(ANIMPREFIX("OffWait"), false);
    }
    break;
    case SolarChargeState::ChargeOn: {
        GetOwner()->MeshRender()->GetMaterial(1)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(6)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(0.f, 1.f, 0.f, 1.f));
        GamePlayStatic::Play2DSound(L"sound\\wav\\GimmickSolarPanel\\0000.wav", 1, MONSTER_EFFECTSOUND);
        Animator()->Play(ANIMPREFIX("Charge"), false);
    }
    break;
    case SolarChargeState::ChargeOff: {
        Animator()->SetReverse(true);
    }
    break;
    case SolarChargeState::ChargedStart: {
        GetOwner()->MeshRender()->GetMaterial(1)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(6)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(1.f, 1.f, 0.f, 1.f));

        GamePlayStatic::Play2DSound(L"sound\\wav\\GimmickSolarPanel\\0001.wav", 1, MONSTER_EFFECTSOUND);
        Animator()->Play(ANIMPREFIX("ChargedStart"), false);
    }
    break;
    case SolarChargeState::ChargedWait:
        break;
    case SolarChargeState::Decreases: {
        GamePlayStatic::Play2DSound(L"sound\\wav\\GimmickSolarPanel\\0002.wav", 1, 0.5f);
        Animator()->Play(ANIMPREFIX("Decreases"), false);
    }
    break;
    case SolarChargeState::OffWaitStart: {
        GetOwner()->MeshRender()->GetMaterial(1)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(6)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(7)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(8)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(12)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        GetOwner()->MeshRender()->GetMaterial(13)->SetEmission(Vec4(1.f, 0.f, 0.f, 1.f));
        Animator()->Play(ANIMPREFIX("OffWaitStart"), false);
    }
    break;
    case SolarChargeState::End:
        break;
    default:
        break;
    }
}

void CSolarChargeScript::ExitState()
{
    switch (m_eState)
    {
    case SolarChargeState::OffWait:
        break;
    case SolarChargeState::ChargeOn: {
        CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\wav\\GimmickSolarPanel\\0000.wav")->Stop();
        m_fTermTime = 0.f;
        m_bOnOffFlag = false;
    }
    break;
    case SolarChargeState::ChargeOff:
        break;
    case SolarChargeState::ChargedStart: {
        if (nullptr != m_pObj)
        {
            CPushOutScript* Script = m_pObj->GetScript<CPushOutScript>();
            if (nullptr != Script)
            {
                Script->ChangeState(PushOutState::MoveDest);
            }
        }
    }
    break;
    case SolarChargeState::ChargedWait:
        break;
    case SolarChargeState::Decreases:
        break;
    case SolarChargeState::OffWaitStart:
        break;
    case SolarChargeState::End:
        break;
    default:
        break;
    }
}

void CSolarChargeScript::OffWait()
{
    if (m_IsArea && PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarChargeState::ChargeOn);
    }
}

void CSolarChargeScript::ChargeOn()
{
    if (!m_IsArea || !PLAYERFSM->IsAttackEvent())
    {
        ChangeState(SolarChargeState::ChargeOff);
    }
    else
    {
        m_fTermTime += DT;
        if (m_bOnOffFlag)
        {
            if (m_fTermTime >= 1.f)
            {
                m_bOnOffFlag = false;
                m_fTermTime = 0.f;
            }
            else
            {
                GetOwner()->MeshRender()->GetMaterial(1)->SetEmission(Vec4(m_fTermTime, m_fTermTime, 0.f, 1.f));
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
                GetOwner()->MeshRender()->GetMaterial(1)->SetEmission(Vec4((1.f - m_fTermTime), (1.f - m_fTermTime), 0.f, 1.f));
            }
        }
    }

    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::ChargedStart);
    }
}

void CSolarChargeScript::ChargeOff()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWait);
    }
}

void CSolarChargeScript::ChargedStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::Decreases);
    }
}

void CSolarChargeScript::ChargedWait()
{
}

void CSolarChargeScript::Decrease()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWaitStart);
    }
}

void CSolarChargeScript::OffWaitStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SolarChargeState::OffWait);
    }
}

void CSolarChargeScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = true;
    }
}

void CSolarChargeScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_IsArea = false;
    }
}

UINT CSolarChargeScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = ToWstring(m_MovingObjName);
    MemoryByte += SaveWStringToFile(wStr, _File);

    return MemoryByte;
}

UINT CSolarChargeScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring wStr = {};
    MemoryByte += LoadWStringFromFile(wStr, _File);

    m_MovingObjName = ToString(wStr);

    return MemoryByte;
}