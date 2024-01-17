#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
    : CScript(SCRIPT_TYPE::BULLETSCRIPT)
    , m_Speed(2500.f)
    , m_Duration(1.f)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::begin()
{
}

void CBulletScript::tick()
{
    if (GetOwner()->IsDead())
        return;

    m_Duration -= DT;

    if (m_Duration <= 0.f)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());

        CGameObject* pShockWave = new CGameObject;
        pShockWave->SetName(L"Shock Wave");
        pShockWave->AddComponent(new CTransform);
        pShockWave->AddComponent(new CMeshRender);
        pShockWave->AddComponent(new CCollider2D);
        pShockWave->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::SHOCKWAVESCRIPT));

        Vec3 pos = Transform()->GetRelativePos();
        pShockWave->Transform()->SetRelativePos(pos);
        pShockWave->Transform()->SetRelativeScale(Vec3(10000.f, 10000.f, 1.f)); // 10000 사이즈 고정
        pShockWave->Collider2D()->SetColliderType(COLLIDER2D_TYPE::CIRCLE);
        pShockWave->Collider2D()->SetRadius(1.f);
        pShockWave->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        pShockWave->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockWaveMtrl"));

        GamePlayStatic::SpawnGameObject(pShockWave, 14);
    }

    Transform()->SetRelativePos(Transform()->GetRelativePos() + (m_Dir.Normalize() * m_Speed * DT));
}

void CBulletScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CBulletScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CBulletScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}
