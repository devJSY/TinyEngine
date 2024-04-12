#include "pch.h"
#include "CPlayerHitBoxScript.h"

#include <Engine\\CPhysics2DMgr.h>

#include "CPlayerScript.h"
#include "CPlayerCameraScript.h"
#include "CEnemyScript.h"

CPlayerHitBoxScript::CPlayerHitBoxScript()
    : CScript(PLAYERHITBOXSCRIPT)
    , m_pFXImpactCritPref(nullptr)
{
}

CPlayerHitBoxScript::~CPlayerHitBoxScript()
{
}

void CPlayerHitBoxScript::begin()
{
    SetEnabled(false);

    // ÇÁ¸®ÆÕ ·Îµù
    m_pFXImpactCritPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FX_ImpactCrit.pref", L"prefab\\FX_ImpactCrit.pref");
}

void CPlayerHitBoxScript::tick()
{
    // if (nullptr == BoxCollider2D())
    //     return;

    // Vec2 Offset = BoxCollider2D()->GetOffset();
    // Vec2 Size = BoxCollider2D()->GetSize();
    // Vec3 sacle = Transform()->GetWorldScale();

    // Matrix matWorld = Transform()->GetWorldMat();
    // Matrix matTranslation = XMMatrixTranslation(Offset.x * sacle.x, Offset.y * sacle.y, 0.0f);
    // Matrix matScale = XMMatrixScaling(2.f * Size.x * sacle.x, 2.f * Size.y * sacle.y, 1.f);
    // Matrix matInvScale = XMMatrixScaling(1.f / sacle.x, 1.f / sacle.y, 1.f / sacle.z);

    // Vec3 color = BoxCollider2D()->IsEnabled() ? Vec3(1.f, 1.f, 0.f) : Vec3(0.f, 0.f, 1.f);

    // GamePlayStatic::DrawDebugRect(matScale * matTranslation * matInvScale * matWorld, color, false);
}

void CPlayerHitBoxScript::SetEnabled(bool _bEnable)
{
    if (nullptr == BoxCollider2D())
        return;

    BoxCollider2D()->SetEnabled(_bEnable);
}

void CPlayerHitBoxScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    CEnemyScript* EnemyScript = _OtherCollider->GetOwner()->GetScript<CEnemyScript>();
    if (nullptr == EnemyScript)
        return;

    CPlayerScript* PlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();

    Vec3 PlayerPos = CGameManagerScript::GetInset()->GetPlayer()->Transform()->GetWorldPos();
    Vec3 EnemyPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();

    PlayerPos.z = 0;
    EnemyPos.z = 0;

    Vec3 Dir = EnemyPos - PlayerPos;
    Dir.Normalize();

    if (EnemyScript->TakeHit(GetRandomInt(PlayerScript->m_ATK - 5, PlayerScript->m_ATK + 5), Dir))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

        // Camera Shake
        CGameObject* pPlayerCamObj = pCurLevel->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->ShakeCam(ShakeDir::Comprehensive, 0.1f, 1.f);
            }
        }

        // FX_ImpactCrit
        int EffectIdx = pCurLevel->FindLayerIndexByName(L"Effect");
        if (-1 == EffectIdx)
            EffectIdx = 0;

        CGameObject* pFXImpactObj = m_pFXImpactCritPref->Instantiate();
        pFXImpactObj->Transform()->SetRelativePos(Transform()->GetWorldPos());

        GamePlayStatic::SpawnGameObject(pFXImpactObj, EffectIdx);
    }
}
