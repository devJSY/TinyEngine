#include "pch.h"
#include "CCollider2D.h"

#include "CPhysics2DMgr.h"

#include <box2d/b2_fixture.h>
#include "CScript.h"

CCollider2D::CCollider2D(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeFixture(nullptr)
    , m_Mtrl(nullptr)
    , m_bTrigger(false)
    , m_Offset(Vec2(0.f, 0.f))
    , m_TriggerCount(0)
    , m_CollisionCount(0)
    , m_PrevScale()
    , m_bEnabled(true)
{
}

CCollider2D::CCollider2D(const CCollider2D& origin)
    : CComponent(origin)
    , m_RuntimeFixture(nullptr)
    , m_Mtrl(origin.m_Mtrl)
    , m_bTrigger(origin.m_bTrigger)
    , m_Offset(origin.m_Offset)
    , m_TriggerCount(0)
    , m_CollisionCount(0)
    , m_PrevScale()
    , m_bEnabled(origin.m_bEnabled)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::begin()
{
    m_PrevScale = Transform()->GetWorldScale();
}

void CCollider2D::finaltick()
{
    // 트랜스폼의 스케일이 변경되었다면 재생성
    Vec3 TransformWorldScale = Transform()->GetTransformWorldScale();
    if ((m_PrevScale - TransformWorldScale).Length() > 1e-3f)
    {
        GamePlayStatic::Physics_Event(GetOwner(), Physics_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = TransformWorldScale;

    if (nullptr == m_RuntimeFixture)
        return;

    // 트랜스폼 위치 정보 업데이트
    b2Body* body = ((b2Fixture*)m_RuntimeFixture)->GetBody();
    float PPM = CPhysics2DMgr::GetInst()->GetPPM();
    Vec3 WorldPos = Transform()->GetWorldPos();
    body->SetTransform(b2Vec2(WorldPos.x / PPM, WorldPos.y / PPM), Transform()->GetWorldRotation().z);
}

void CCollider2D::SetMaterial(Ptr<CPhysics2DMaterial> _Mtrl)
{
    if (m_Mtrl == _Mtrl)
        return;

    m_Mtrl = _Mtrl;

    if (nullptr != m_RuntimeFixture)
    {
        ((b2Fixture*)m_RuntimeFixture)->SetFriction(m_Mtrl->GetFriction());
        ((b2Fixture*)m_RuntimeFixture)->SetRestitution(m_Mtrl->GetBounciness());
    }
}

void CCollider2D::SetOffset(Vec2 _offset)
{
    if (m_Offset == _offset)
        return;

    m_Offset = _offset;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

bool CCollider2D::IsEnabled()
{
    if (nullptr != Rigidbody2D())
        return Rigidbody2D()->IsSimulated();
    else
        return m_bEnabled;
}

void CCollider2D::SetEnabled(bool _bEnabled)
{
    if (m_bEnabled == _bEnabled)
        return;

    m_bEnabled = _bEnabled;

    if (nullptr != Rigidbody2D())
        Rigidbody2D()->SetSimulated(m_bEnabled);
    else
        GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

bool CCollider2D::IsCollision(Vec2 _Point)
{
    if (nullptr == m_RuntimeFixture)
        return false;

    float PPM = CPhysics2DMgr::GetInst()->GetPPM();
    return ((b2Fixture*)m_RuntimeFixture)->TestPoint(_Point / PPM);
}

bool CCollider2D::RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, RaycastHit2D& _Hit)
{
    if (nullptr == m_RuntimeFixture)
        return false;

    float PPM = CPhysics2DMgr::GetInst()->GetPPM();

    _Dirction.Normalize();
    Vec2 p1 = _Origin / PPM;
    Vec2 p2 = (_Origin + _Dirction * _Distance) / PPM;

    b2RayCastInput input = b2RayCastInput();
    input.p1 = p1;
    input.p2 = p2;
    input.maxFraction = 1.f;

    b2RayCastOutput output = b2RayCastOutput();
    if (((b2Fixture*)m_RuntimeFixture)->RayCast(&output, input, 0))
    {
        b2Vec2 HitPoint = input.p1 + output.fraction * (input.p2 - input.p1);
        HitPoint *= PPM;

        _Hit.Centroid = _Origin;
        _Hit.Fraction = output.fraction;
        _Hit.Normal = output.normal;
        _Hit.Point = HitPoint;
        _Hit.Distance = (_Hit.Point - _Origin).Length();
        _Hit.pCollisionObj = GetOwner();

        return true;
    }

    return false;
}

void CCollider2D::SetTrigger(bool _trigger)
{
    if (m_bTrigger == _trigger)
        return;

    m_bTrigger = _trigger;

    if (nullptr != m_RuntimeFixture)
        ((b2Fixture*)m_RuntimeFixture)->SetSensor(m_bTrigger);
}

void CCollider2D::OnCollisionEnter(CCollider2D* _OtherCollider)
{
    ++m_CollisionCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionEnter(_OtherCollider);
}

void CCollider2D::OnCollisionStay(CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionStay(_OtherCollider);
}

void CCollider2D::OnCollisionExit(CCollider2D* _OtherCollider)
{
    --m_CollisionCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnCollisionExit(_OtherCollider);
}

void CCollider2D::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    ++m_TriggerCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerEnter(_OtherCollider);
}

void CCollider2D::OnTriggerStay(CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerStay(_OtherCollider);
}

void CCollider2D::OnTriggerExit(CCollider2D* _OtherCollider)
{
    --m_TriggerCount;

    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerExit(_OtherCollider);
}

UINT CCollider2D::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveAssetRef(m_Mtrl, _File);
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
    fwrite(&m_bEnabled, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec2);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CCollider2D::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += LoadAssetRef(m_Mtrl, _File);
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_bTrigger, sizeof(bool), 1, _File);
    fread(&m_bEnabled, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec2);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
