#include "pch.h"
#include "CMonsterMoveController.h"

#include "CMonsterMgr.h"
#include "CUnitScript.h"

CMonsterMoveController::CMonsterMoveController()
    : CScript(MONSTERMOVECONTROLLER)
    , m_pTransform(nullptr)
    , m_pMgr(nullptr)
    , m_pUnitScript(nullptr)
    , m_fCurSpeed(0.f)
    , m_fLerpValue(1.f)
    , m_fTrackDistance(0.f)
    , m_bIsTrack(false)
    , m_bIsTrackObject(false)
    , m_bLerpTrack(true)
    , m_bIsRushObject(true)
    , m_bIsRush(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_bLerpTrack, "Lerp Tracking");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpValue, "Lerf Value");

    AddScriptParam(SCRIPT_PARAM::INT, &m_bIsTrackObject, "IsTracking Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTrackDistance, "Tracking Distance");

    // AddScriptParam(SCRIPT_PARAM::INT, &m_bIsRushObject, "IsRush Object");

    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_bIsRush, "IsRush");
    // AddScriptParam(SCRIPT_PARAM::INT, &m_bIsTrack, "IsTracking");
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
    (m_bIsTrack && m_bIsTrackObject) == true ? Tracking(m_bLerpTrack) : void();

    // Rush
    (m_bIsRush && m_bIsRushObject) == true ? Rush(m_bLerpTrack) : void();

    // Slip
    (m_bIsSlip && m_bIsRushObject) == true ? Slip(m_bLerpTrack) : void();

    // Patrol
    (m_bIsPatrol && m_bIsPatrolObject) == true ? Patrol(m_bLerpTrack) : void();

    // Rotating
    (m_bIsRotating && m_bIsRotateObject) == true ? Rotating() : void();
}

void CMonsterMoveController::Tracking(bool _flag)
{
    Vec3 _vDir = m_pTransform->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 _vPos = m_pTransform->GetLocalPos();
    float _fSpeed = m_pUnitScript->GetCurInfo().Speed;
    ApplyDir(_vDir, _flag);
    m_pTransform->SetLocalPos(_vPos + _vDir * _fSpeed * DT);
}

void CMonsterMoveController::Rush(bool _flag)
{
    Vec3 _vDir = m_pTransform->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 _vPos = m_pTransform->GetLocalPos();
    ApplyDir(_vDir, _flag);

    m_fCurSpeed = Lerp(0.f, m_fMaxSpeed, m_fRushLerp * DT);
    m_pTransform->SetLocalPos(_vPos + _vDir * m_fCurSpeed * DT);
}

void CMonsterMoveController::Slip(bool _flag)
{
    Vec3 _vDir = m_pTransform->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 _vPos = m_pTransform->GetLocalPos();

    m_fCurSpeed = Lerp(m_fCurSpeed, 0.f, m_fRushLerp * DT);
    m_pTransform->SetLocalPos(_vPos + _vDir * m_fCurSpeed * DT);
}

void CMonsterMoveController::Patrol(bool _flag)
{
    Vec3 _vUp = {};
    Vec3 _vPos = m_pTransform->GetLocalPos();
    float _fSpeed = m_pUnitScript->GetCurInfo().Speed;

    // 목적지까지 Patrol이 끝났는데 반복 정찰이라면 다시 원래 자리로 돌아간다.
    if ((m_pDestPos.x - 1.f <= _vPos.x && m_pDestPos.x + 1.f >= _vPos.x)
        && (m_pDestPos.x - 1.f <= _vPos.x && m_pDestPos.x + 1.f >= _vPos.x)
        && (m_pDestPos.x - 1.f <= _vPos.x && m_pDestPos.x + 1.f >= _vPos.x)
        && m_bLoopPatrol)
    {
        Vec3 _vTemp = m_pDestPos;
        m_pDestPos = m_pOriginPos;
        m_pOriginPos = m_pDestPos;
    }

    if (Vec3(0.f, 0.f, -1.f) == m_vPatrolDir)
    {
        _vUp = Vec3(0.f, -1.f, 0.f);
    }

    // Rotating시에는 WorldRotation
    Quat _vOriginQuat = m_bIsRotating ? Quat::LookRotation(m_vPatrolDir, _vUp) : Quat::LookRotation(m_pTransform->GetLocalDir(DIR_TYPE::FRONT), _vUp);

    Quat _vTargetQuat = Quat::LookRotation(-(m_pDestPos - _vPos).Normalize(), _vUp); 

    if (_flag)
    {
        _vTargetQuat = Quat::Slerp(_vOriginQuat, _vTargetQuat, m_fLerpValue * DT);
    }

    m_pTransform->SetLocalPos(m_vPatrolDir * _fSpeed * DT);

    if (m_bIsRotating)
    {
        m_vPatrolDir = CaculateDir(_vTargetQuat.ToEuler()).Normalize();
    }
    else
    {
        m_pTransform->SetWorldRotation(_vTargetQuat);
    }
}

void CMonsterMoveController::Rotating()
{
    float Angle = m_pTransform->GetLocalRotation().y;
    Angle += DT;
    Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Angle);
    m_pTransform->SetWorldRotation(Quaternion);
}

void CMonsterMoveController::ApplyDir(Vec3 _vFront, bool _flag)
{
    Vec3 _vPos = m_pTransform->GetLocalPos();

    // Caculate Quaternion Tracking Dir
    Quat _vOriginQuat = m_pTransform->GetWorldQuaternion();
    Vec3 _vTrackingDir = m_pMgr->TrackDir(_vPos);

    // Quternion Fix
    Vec3 _vUp = Vec3(0.f, 0.f, -1.f) == _vFront ? Vec3(0.f, 1.f, 0.f) : Vec3(0.f, -1.f, 0.f);

    Quat _vTrackQuat = Quat::LookRotation(-_vTrackingDir, _vUp);

    if (_flag)
        _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fLerpValue * DT);

    m_pTransform->SetWorldRotation(_vTrackQuat);
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

Vec3 CMonsterMoveController::CaculateDir(Vec3 _vRadian)
{
    Quat _LocalQuat = Quat::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), _vRadian.x) *
                        Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), _vRadian.y) *
                        Quat::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), _vRadian.z);

    Matrix matRot = Matrix::CreateFromQuaternion(_LocalQuat);
    return XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRot);
}