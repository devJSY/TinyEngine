#include "pch.h"
#include "CKirbyDropOutUIScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CState.h"

#include <Engine\CRenderMgr.h>

CKirbyDropOutUIScript::CKirbyDropOutUIScript()
    : CScript(KIRBYDROPOUTUISCRIPT)
    , m_pProgressUI{}
    , m_pCircleProgressUI(nullptr)
    , m_eState(DropOutUIState::Idle)
    , m_vOffset{}
    , m_bFailed(false)
    , m_fAccTime(0.f)
    , m_fAppearTime(0.f)
    , m_fWaitTime(0.1f)
    , m_fDisappearTime(0.f)
    , m_bInterAppear(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAppearTime, "Appear Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDisappearTime, "Disappear Time");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOffset, "Offset");
}

CKirbyDropOutUIScript::CKirbyDropOutUIScript(const CKirbyDropOutUIScript& Origin)
    : CScript(Origin)
    , m_pProgressUI{}
    , m_pCircleProgressUI(nullptr)
    , m_eState(DropOutUIState::Idle)
    , m_vOffset(Origin.m_vOffset)
    , m_bFailed(false)
    , m_fAccTime(0.f)
    , m_fWaitTime(0.1f)
    , m_fAppearTime(Origin.m_fAppearTime)
    , m_fDisappearTime(Origin.m_fDisappearTime)
    , m_bInterAppear(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAppearTime, "Appear Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDisappearTime, "Disappear Time");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOffset, "Offset");
}

CKirbyDropOutUIScript::~CKirbyDropOutUIScript()
{
}

void CKirbyDropOutUIScript::begin()
{
    m_pProgressUI = GetOwner()->GetChildObject();

    m_pCircleProgressUI = GetOwner()->GetChildObject(L"UI_CircleProgress");

    GetOwner()->Transform()->SetWorldPos(Vec3(0.f, 0.f, 0.f));
    GetOwner()->Transform()->SetLocalScale(Vec3(2.f, 2.f, 1.f));

    m_pCircleProgressUI->Transform()->SetLocalScale(Vec3(46.f, 46.f, 1.f));
    m_pCircleProgressUI->Transform()->SetLocalPos(Vec3(-49.f, 10.f, 0.f));

    GetOwner()->GetChildObject(L"UI_CircleProgressOutLine1")->Transform()->SetLocalScale(Vec3(132.f, 64.f, 1.f));
    GetOwner()->GetChildObject(L"UI_CircleProgressOutLine1")->Transform()->SetLocalPos(Vec3(-5.f, 0.f, 0.f));

    for (size_t i = 0; i < m_pProgressUI.size(); i++)
    {
        if (nullptr != m_pProgressUI[i])
        {
            m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_3, 1);
            m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);

            if (L"UI_CircleProgressFontTexture" == m_pProgressUI[i]->GetName())
            {
                m_pProgressUI[i]->Transform()->SetLocalPos(Vec3(6.f, -5.f, 0.f));
                m_pProgressUI[i]->Transform()->SetLocalScale(Vec3(58.f, 17.f, 1.f));
            }
        }
    }

    m_fAppearTime = m_fDisappearTime = 0.5f;
}

void CKirbyDropOutUIScript::tick()
{
    TrackPlayer();

    switch (m_eState)
    {
    case DropOutUIState::Idle: {
        Idle();
    }
    break;
    case DropOutUIState::Wait: {
        Wait();
    }
    break;
    case DropOutUIState::Appear: {
        Appear();
    }
    break;
    case DropOutUIState::Disappear: {
        Disappear();
    }
    break;
    case DropOutUIState::End:
        break;
    default:
        break;
    }
}

void CKirbyDropOutUIScript::Idle()
{
    if (PLAYERFSM->GetYPressedTime() >= 0.2f || (m_bInterAppear && PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE))
    {
        ChangeState(DropOutUIState::Appear);
    }
}

void CKirbyDropOutUIScript::Wait()
{
    m_fAccTime += DT;

    if (m_fWaitTime <= m_fAccTime)
    {
        ChangeState(DropOutUIState::Idle);
    }

    for (size_t i = 0; i < m_pProgressUI.size(); i++)
    {
        if (nullptr != m_pProgressUI[i])
        {
            m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
        }
    }
}

void CKirbyDropOutUIScript::Appear()
{
    if (PLAYERFSM->GetCurState()->GetName() == L"DROP_OBJECT" || PLAYERFSM->GetCurState()->GetName() == L"DROP_ABILITY")
    {
        ChangeState(DropOutUIState::Wait);
        return;
    }

    FadeFunc(true);

    float fRatio = PLAYERFSM->GetYPressedTime() / PLAYERFSM->GetDropCopyTime();
    if (PLAYERFSM->GetYPressedTime() <= 0.f && !(m_bInterAppear && PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE))
    {
        ChangeState(DropOutUIState::Disappear);
        m_bFailed = true;
    }

    if (nullptr != m_pCircleProgressUI)
    {
        m_pCircleProgressUI->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, 360.f * fRatio);
    }
}

void CKirbyDropOutUIScript::Disappear()
{
    FadeFunc(false);

    float fRatio = PLAYERFSM->GetYPressedTime() / PLAYERFSM->GetDropCopyTime();

    if (PLAYERFSM->GetYPressedTime() >= 0.1f || (m_bInterAppear && PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE))
    {
        ChangeState(DropOutUIState::Appear);
    }

    if (nullptr != m_pCircleProgressUI)
    {
        m_pCircleProgressUI->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, 360.f * fRatio);
    }
}

void CKirbyDropOutUIScript::ChangeState(DropOutUIState _eState)
{
    ExitState();
    m_eState = _eState;
    EnterState();
}

void CKirbyDropOutUIScript::EnterState()
{
    switch (m_eState)
    {
    case DropOutUIState::Wait: {
        m_fAccTime = 0.f;
        for (size_t i = 0; i < m_pProgressUI.size(); i++)
        {
            if (nullptr != m_pProgressUI[i])
            {
                m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
            }
        }
    }
    break;
    case DropOutUIState::Appear:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::DropOut:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::Disappear:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::End:
        break;
    default:
        break;
    }
}

void CKirbyDropOutUIScript::ExitState()
{
    switch (m_eState)
    {
    case DropOutUIState::Wait:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::Appear:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::DropOut:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::Disappear:
        m_fAccTime = 0.f;
        break;
    case DropOutUIState::End:
        break;
    default:
        break;
    }
}

void CKirbyDropOutUIScript::FadeFunc(bool _fadeIn)
{
    if (_fadeIn)
    {
        m_fAccTime += DT;

        float fRatio = m_fAccTime / m_fAppearTime;

        for (size_t i = 0; i < m_pProgressUI.size(); i++)
        {
            if (nullptr != m_pProgressUI[i])
            {
                m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, fRatio);
            }
        }

        if (m_fAccTime >= m_fAppearTime)
        {
            m_fAccTime = m_fAppearTime;
        }
    }
    else
    {
        m_fAccTime += DT;

        float fRatio = m_fAccTime / m_fAppearTime;

        for (size_t i = 0; i < m_pProgressUI.size(); i++)
        {
            if (nullptr != m_pProgressUI[i])
            {
                m_pProgressUI[i]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, (1.f - fRatio));
            }
        }

        if (m_fAccTime >= m_fAppearTime)
        {
            m_fAccTime = 0.f;
            ChangeState(DropOutUIState::Wait);
        }
    }
}

void CKirbyDropOutUIScript::TrackPlayer()
{
    Vec3 playerPos = PLAYER->Transform()->GetWorldPos();

    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetCamera(0);

    Matrix _VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec3 _vPlayerNDCPos = Vector3::Transform(playerPos, _VPMatrix);

    // UICam
    _pCam = CRenderMgr::GetInst()->GetCamera(2);

    if (nullptr == _pCam)
        return;

    // NDC -> WorldPos
    Matrix _VPInverseMatrix = _pCam->GetProjInvMat() * _pCam->GetViewInvMat();

    Vec3 WorldPos = Vector3::Transform(_vPlayerNDCPos, _VPInverseMatrix);

    GetOwner()->Transform()->SetWorldPos(WorldPos + m_vOffset);
}

UINT CKirbyDropOutUIScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fAppearTime, sizeof(float), 1, _File);
    fwrite(&m_fDisappearTime, sizeof(float), 1, _File);
    fwrite(&m_vOffset, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

UINT CKirbyDropOutUIScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fAppearTime, sizeof(float), 1, _File);
    fread(&m_fDisappearTime, sizeof(float), 1, _File);
    fread(&m_vOffset, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}
