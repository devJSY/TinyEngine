#include "pch.h"
#include <Engine\\CScript.h>
#include "CElfilisLaser.h"

CElfilisLaser::CElfilisLaser()
    : CScript(ELFILISLASER)
    , m_Step(StateStep::Start)
    , m_bAutoPlay(false)
    , m_bCollisionFloor(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCollisionFloor, "Collision Floor");
}

CElfilisLaser::CElfilisLaser(const CElfilisLaser& _Origin)
    : CScript(_Origin)
    , m_Step(StateStep::Start)
    , m_bAutoPlay(_Origin.m_bAutoPlay)
    , m_bCollisionFloor(_Origin.m_bCollisionFloor)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCollisionFloor, "Collision Floor");
}

CElfilisLaser::~CElfilisLaser()
{
}

void CElfilisLaser::begin()
{
    if (m_bAutoPlay)
    {
        SetStart();
    }

    CGameObject* Outer = GetOwner()->GetChildObject()[0];
    UINT Layer = GetOwner()->GetLayerIdx();

    if (Outer && Outer->GetLayerIdx() != Layer)
    {
        GamePlayStatic::LayerChange(Outer, Layer);
    }
}

void CElfilisLaser::tick()
{
    if (!m_bAutoPlay)
        return;

    switch (m_Step)
    {
    case StateStep::Start:
        Start();
        break;
    case StateStep::Progress:
        Progress();
        break;
    case StateStep::End:
        End();
        break;
    }
}

void CElfilisLaser::SetStart()
{
    m_Step = StateStep::Start;
    Animator()->Play(ANIMPREFIX("Start"), false);
    PlayOuter(ANIMPREFIX("Start"), false);
    GetOwner()->SetActive(true);
}

void CElfilisLaser::SetWait()
{
    m_Step = StateStep::Progress;
    Animator()->Play(ANIMPREFIX("Loop"));
    PlayOuter(ANIMPREFIX("Loop"));
}

void CElfilisLaser::SetEnd()
{
    m_Step = StateStep::End;
    Animator()->Play(ANIMPREFIX("End"), false);
    PlayOuter(ANIMPREFIX("End"), false);
}

void CElfilisLaser::Start()
{
    if (Animator()->IsFinish())
    {
        SetWait();
    }
}

void CElfilisLaser::Progress()
{
    static vector<wstring> vecCollision{L"World Static"};

    if (m_bCollisionFloor)
    {
        RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 2000.f, vecCollision);

        if (Hit.pCollisionObj)
        {
            // @Effect Æ¢´Â ÀÌÆåÆ®, ¹Ù´ÚÀÜ»ó ¼ÒÈ¯
            GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), 2000.f, Vec3(1.f, 0.f, 0.f), true);
        }
    }
}

void CElfilisLaser::End()
{
    if (Animator()->IsFinish())
    {
        GetOwner()->SetActive(false);
    }
}

void CElfilisLaser::PlayOuter(const wstring _Anim, bool _bRepeat)
{
    CGameObject* Outer = GetOwner()->GetChildObject()[0];

    if (!Outer)
        return;

    Outer->Animator()->Play(_Anim, _bRepeat);
}

UINT CElfilisLaser::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bCollisionFloor, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CElfilisLaser::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bCollisionFloor, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}