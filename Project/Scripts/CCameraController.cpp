#include "pch.h"
#include "CCameraController.h"

#include <Engine/CLevelMgr.h>

CCameraController::CCameraController()
    : CScript(CAMERACONTROLLER)
    , m_Target(nullptr)
    , m_LookDir(0.f, 0.f, 0.f)
    , m_LookDist(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Init Look Dir", 0.05f);
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LookDist, "Init Look Dis", 0.05f);
    
    
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinSpeed, "m_MinSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MaxSpeed, "m_MaxSpeed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ThresholdDistance, "m_ThresholdDistance");
}

CCameraController::~CCameraController()
{
}

void CCameraController::begin()
{
    // Level Begin시 플레이어를 타겟으로 지정한다.
    if (m_Target == nullptr)
    {
        CLevel* CurrentLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        m_Target = CurrentLevel->FindObjectByName(L"Main Player", LAYER_PLAYER);

        // Player가 없는 경우 Assert
        assert(m_Target);
    }

    // FOV 설정
    if (nullptr != Camera())
    {
        Camera()->SetFOV(XM_PI / 4.f);
    }

    // 카메라 세팅
    Vec3 InitPos = CalCamPos(m_Target->Transform()->GetWorldPos(), m_LookDir, m_LookDist);

    m_LookEyePos = InitPos;

    m_PrevCamPos = InitPos;
    m_PrevLookDir = m_LookDir;

    m_CurCamPos = InitPos;
    m_CurLookDir = m_LookDir;

    Transform()->SetWorldPos(InitPos);
    Transform()->SetDirection(m_LookDir);
}

void CCameraController::tick()
{
    // Target이 없다면 return
    if (nullptr == m_Target)
        return;

    // ========================= Prev Data Save =========================
    m_PrevCamPos = m_CurCamPos;
    m_PrevLookDir = m_PrevLookDir;

    // ========================= Eye Pos Update =========================
    
    Vec3 TargetPos = m_Target->Transform()->GetWorldPos();
    m_LookEyePos = CalCamPos(TargetPos, m_LookDir, m_LookDist);

    // ======================= Cur Pos, Dir Update ======================
    Vec3 Diff = m_LookEyePos - m_PrevCamPos;
    float MoveLength = Diff.Length();
    Vec3 MoveDir = Diff.Normalize();

    float Ratio = clamp((MoveLength / m_ThresholdDistance),0.f,1.f) * XM_PI / 2.f;
    float CamSpeed = m_MinSpeed + (m_MaxSpeed - m_MinSpeed) * sinf(Ratio);

    m_CurCamPos = m_PrevCamPos + MoveDir * CamSpeed * DT;
    




    // =========================== 예외 처리 =============================
    // 보간 값이 목표값과 비슷하다면 그대로 세팅해준다.
    if ((m_CurCamPos - m_LookEyePos).Length() <= 1e-6f)
    {
        m_CurCamPos = m_LookEyePos;
    }

    // 카메라가 LookEyePos를 넘어서까지 이동했다면 CurPos를 LookEyePos로 세팅해준다.
    Vec3 LeftMoveDir = (m_LookEyePos - m_CurCamPos).Normalize();
    if (MoveDir.Dot(LeftMoveDir) <= 0.f)
    {
        m_CurCamPos = m_LookEyePos;
    }


    // ==================== Camera Transform Update ====================

    Transform()->SetWorldPos(m_CurCamPos);
    Transform()->SetDirection(m_CurLookDir);



    // ==================== debug ========================
    // GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), m_LookDist, Vec3(1.f, 0.f, 0.f), true);
    // GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), front, 200.f, Vec3(0.f, 0.f, 1.f), true);
    // GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), up, 200.f, Vec3(0.f, 1.f, 0.f), true);
    // GamePlayStatic::DrawDebugLine(CurPos, m_LookDir, m_LookDist, Vec3(1.f, 0.f, 0.f), true);
}

Vec3 CCameraController::CalCamPos(Vec3 _TargetWorldPos, Vec3 _LookDir, float _CamDist)
{
    _LookDir.Normalize();

    Vec3 RetVal = _TargetWorldPos - _LookDir * _CamDist;

    return RetVal;
}

UINT CCameraController::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_LookDir, sizeof(Vec3), 1, _File);
    fwrite(&m_LookDist, sizeof(float), 1, _File);
    
    fwrite(&m_MinSpeed, sizeof(float), 1, _File);
    fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_ThresholdDistance, sizeof(float), 1, _File);


    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CCameraController::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_LookDir, sizeof(Vec3), 1, _File);
    fread(&m_LookDist, sizeof(float), 1, _File);

    fread(&m_MinSpeed, sizeof(float), 1, _File);
    fread(&m_MaxSpeed, sizeof(float), 1, _File);
    fread(&m_ThresholdDistance, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
