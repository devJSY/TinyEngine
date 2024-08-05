#include "pch.h"
#include "CElfilisArrowSetScript.h"
#include "CElfilisArrowScript.h"

CElfilisArrowSetScript::CElfilisArrowSetScript()
    : CScript(ELFILISARROWSETSCRIPT)
    , m_Step(0)
    , m_Target(nullptr)
    , m_RotSpeed(3.5f)
    , m_CurSpeed(5.f)
    , m_AccTime(0.f)
    , m_bArrowStart{false,}
{
}

CElfilisArrowSetScript::~CElfilisArrowSetScript()
{
}

void CElfilisArrowSetScript::begin()
{
    if (Rigidbody())
    {
        Rigidbody()->SetKinematic(true);
    }
}

void CElfilisArrowSetScript::tick()
{
    if (!m_Target)
        return;

    switch (m_Step)
    {
    case 0:
        Spawn();
        break;
    case 1:
        Ready();
        break;
    case 2:
        Fly();
        break;
    }
}

void CElfilisArrowSetScript::Spawn()
{
    m_AccTime += DT;

    // Rotate & chase
    Vec3 Rot = Transform()->GetLocalRotation();
    Rot.y += DT * m_RotSpeed;
    Transform()->SetLocalRotation(Rot);
    Transform()->SetWorldPos(m_Target->Transform()->GetWorldPos());

    // Spawn Arrows
    const vector<CGameObject*>& ArrowParents = GetOwner()->GetChildObject();

    for (int i = 0; i < 7; ++i)
    {
        if (!m_bArrowStart[i] && m_AccTime >= 0.2f * float(i + 1))
        {
            m_bArrowStart[i] = true;
            ArrowParents[i]->GetChildObject()[0]->GetScript<CElfilisArrowScript>()->StartSpawn();
        }
    }

    if (m_bArrowStart[6])
    {
        m_CurSpeed = m_RotSpeed;
        m_Step++;
    }
}

void CElfilisArrowSetScript::Ready()
{
    m_CurSpeed -= DT * m_RotSpeed;

    // Rotate & chase
    if (m_CurSpeed >= 0.f)
    {
        Vec3 Rot = Transform()->GetLocalRotation();
        Rot.y += DT * m_CurSpeed;
        Transform()->SetLocalRotation(Rot);
        Transform()->SetWorldPos(m_Target->Transform()->GetWorldPos());
    }

    // change step
    else
    {
        const vector<CGameObject*>& ArrowParents = GetOwner()->GetChildObject();
        for (int i = 0; i < 7; ++i)
        {
            ArrowParents[i]->GetChildObject()[0]->GetScript<CElfilisArrowScript>()->StartAttack();
        }

        m_AccTime = 0.f;
        m_Step++;
    }
}

void CElfilisArrowSetScript::Fly()
{
    m_AccTime += DT;

    if (m_AccTime >= 3.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CElfilisArrowSetScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CElfilisArrowSetScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}