#include "pch.h"
#include "CMovingObjScript.h"

CMovingObjScript::CMovingObjScript()
    : CScript(MOVINGOBJSCRIPT)
    , m_vDest{}
    , m_vDir{}
    , m_vOriginPos{}
    , m_eState(MovingObjState::Stop)
    , m_fSpeed(0.f)
    , m_bLoop(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bLoop, "Loop");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Dir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
}

CMovingObjScript::CMovingObjScript(const CMovingObjScript& Origin)
    : CScript(Origin)
    , m_vDest{}
    , m_vDir{}
    , m_vOriginPos{}
    , m_eState(MovingObjState::Stop)
    , m_fSpeed(Origin.m_fSpeed)
    , m_bLoop(Origin.m_bLoop)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bLoop, "Loop");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDest, "Destination");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Dir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
}

CMovingObjScript::~CMovingObjScript()
{
}

void CMovingObjScript::begin()
{
    m_vOriginPos = true == m_bLoop ? Transform()->GetLocalPos() : Vec3(0.f, 0.f, 0.f);
}

void CMovingObjScript::tick()
{
    switch (m_eState)
    {
    case MovingObjState::Move: {
        Move();
    }
    break;
    case MovingObjState::Stop: {
        Stop();
    }
    break;
    case MovingObjState::End:
        break;
    default:
        break;
    }
}

void CMovingObjScript::Move()
{
    Vec3 vPos = Transform()->GetLocalPos();
    if ((m_vDest.x - 1.f <= vPos.x && vPos.x <= m_vDest.x + 1.f) && (m_vDest.y - 1.f <= vPos.y && vPos.y <= m_vDest.y + 1.f) &&
        (m_vDest.z - 1.f <= vPos.z && vPos.z <= m_vDest.z + 1.f))
    {
        Transform()->SetLocalPos(m_vDest);
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        if (m_bLoop)
        {
            Vec3 temp = m_vDest;
            m_vDest = m_vOriginPos;
            m_vOriginPos = temp;

            m_vDir *= -1.f;
        }
        else
        {
            m_eState = MovingObjState::Stop;
        }
    }
    else
    {
        Rigidbody()->SetVelocity(m_vDir * m_fSpeed * DT);
    }
}

void CMovingObjScript::Stop()
{
}

void CMovingObjScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    if (L"Body Collider" == Obj->GetName())
    {
        wstring name = GetOwner()->GetName();
        GetOwner()->AddChild(Obj->GetParent());
    }
}

void CMovingObjScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* Obj = _OtherCollider->GetOwner();
    //if (L"Body Collider" == Obj->GetName())
        //GetOwner()->GetChildObject(L"Main Player")->DisconnectWithParent();
}

UINT CMovingObjScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_vDest, sizeof(Vec3), 1, _File);
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_bLoop, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CMovingObjScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_vDest, sizeof(Vec3), 1, _File);
    fread(&m_vDir, sizeof(Vec3), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_bLoop, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}