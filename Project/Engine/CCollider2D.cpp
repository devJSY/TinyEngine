#include "pch.h"
#include "CCollider2D.h"

#include "CPhysics2DMgr.h"

#include <box2d\\b2_fixture.h>
#include "CScript.h"

CCollider2D::CCollider2D(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_RuntimeFixture(nullptr)
    , m_Mtrl(nullptr)
    , m_bTrigger(false)
    , m_Offset(Vec2(0.f, 0.f))
    , m_CollisionCount(0)
    , m_PrevScale()
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
    // 스케일이 변경되었다면 Body 재생성
    if (m_PrevScale != Transform()->GetRelativeScale())
    {
        GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
    }

    m_PrevScale = Transform()->GetRelativeScale();

    if (nullptr == m_RuntimeFixture)
        return;

    // 트랜스폼 위치 정보 업데이트
    b2Body* body = ((b2Fixture*)m_RuntimeFixture)->GetBody();
    float PPM = CPhysics2DMgr::GetInst()->GetPPM();
    body->SetTransform(b2Vec2(Transform()->GetWorldPos().x / PPM, Transform()->GetWorldPos().y / PPM), Transform()->GetRelativeRotation().z);
}

void CCollider2D::SetMaterial(Ptr<CPhysics2DMaterial> _Mtrl)
{
    m_Mtrl = _Mtrl;

    if (nullptr != m_RuntimeFixture)
    {
        ((b2Fixture*)m_RuntimeFixture)->SetFriction(m_Mtrl->GetFriction());
        ((b2Fixture*)m_RuntimeFixture)->SetRestitution(m_Mtrl->GetBounciness());
    }
}

void CCollider2D::SetOffset(Vec2 _offset)
{
    m_Offset = _offset;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CCollider2D::SetTrigger(bool _trigger)
{
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
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (UINT i = 0; i < vecScript.size(); i++)
        vecScript[i]->OnTriggerExit(_OtherCollider);
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
    SaveAssetRef(m_Mtrl, _File);
    fwrite(&m_Offset, sizeof(Vec2), 1, _File);
    fwrite(&m_bTrigger, sizeof(bool), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
    LoadAssetRef(m_Mtrl, _File);
    fread(&m_Offset, sizeof(Vec2), 1, _File);
    fread(&m_bTrigger, sizeof(bool), 1, _File);
}
