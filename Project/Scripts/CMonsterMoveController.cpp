#include "pch.h"
#include "CMonsterMoveController.h"

#include "CMonsterMgr.h"
#include "CUnitScript.h"

CMonsterMoveController::CMonsterMoveController()
    : CScript(MONSTERMOVECONTROLLER)
    , m_pTransform(nullptr)
    , m_pMgr(nullptr)
    , m_pUnitScript(nullptr)
    , m_fLerpValue(1.f)
    , m_fTrackDistance(0.f)
    , m_bIsTrack(false)
    , m_bIsTrackObject(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_bIsTrackObject, "IsTracking Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTrackDistance, "Tracking Distance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpValue, "Lerf Value");
    AddScriptParam(SCRIPT_PARAM::INT, &m_bIsTrack, "IsTracking");
}

CMonsterMoveController::~CMonsterMoveController()
{
}

void CMonsterMoveController::begin()
{
    if (nullptr == Transform())
    {
        GetOwner()->AddComponent(new CTransform);
    }

    m_pTransform = Transform();

    m_pMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CMonsterMgr>();
    if (nullptr == m_pMgr)
    {
        MessageBox(nullptr, L"Manager 객체에 CMonsterMgr Script가 없습니다.", L"CMonsterMgr Script Dose not exist", MB_OK);
        return;
    }

    m_pUnitScript = GetOwner()->GetScript<CUnitScript>();
    if (nullptr == m_pUnitScript)
    {
        MessageBox(nullptr, L"MoveController가 존재하는데 UnitScript가 존재하지 않습니다.", L"Unit Script Does not exist", MB_OK);
        return;
    }
}

void CMonsterMoveController::tick()
{
    if (nullptr == m_pMgr || nullptr == m_pUnitScript)
        return;

    // Tracking
    (m_bIsTrack && m_bIsTrackObject) == true ? Tracking() : void();
}

void CMonsterMoveController::Tracking()
{
    // Moveing Value
    Vec3 _vDir = m_pTransform->GetWorldDir(DIR_TYPE::FRONT);
    float _fSpeed = m_pUnitScript->GetCurInfo().Speed;

    Vec3 _vPos = m_pTransform->GetLocalPos();

    // Caculate Quaternion Tracking Dir
    Quat _vOriginQuat = m_pTransform->GetWorldQuaternion();
    Vec3 _vTrackingDir = m_pMgr->TrackDir(_vPos);

    Quat _vTrackQuat = Quat::LookRotation(-_vTrackingDir, Vec3(0.f, 1.f, 0.f));
    Quat _vLerp = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fLerpValue * DT);
        
    m_pTransform->SetWorldRotation(_vLerp);
    m_pTransform->SetLocalPos(_vPos + _vDir * _fSpeed * DT);
}

void CMonsterMoveController::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_fLerpValue, sizeof(m_fLerpValue), 1, _File);
    fwrite(&m_fTrackDistance, sizeof(m_fTrackDistance), 1, _File);
}

void CMonsterMoveController::LoadFromLevelFile(FILE* _File)
{
    fread(&m_fLerpValue, sizeof(m_fLerpValue), 1, _File);
    fread(&m_fTrackDistance, sizeof(m_fTrackDistance), 1, _File);
}
