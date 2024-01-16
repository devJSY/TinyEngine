#include "pch.h"
#include "CShockWaveScript.h"

CShockWaveScript::CShockWaveScript()
    : CScript(SCRIPT_TYPE::SHOCKWAVESCRIPT)
    , m_Time(0.f)
{
}

CShockWaveScript::~CShockWaveScript()
{
}

void CShockWaveScript::begin()
{
}

void CShockWaveScript::tick()
{
    m_Time += DT;

    MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &m_Time);

    if (m_Time > 1.f)
        GamePlayStatic::DestroyGameObject(GetOwner());
}

void CShockWaveScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CShockWaveScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CShockWaveScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}
