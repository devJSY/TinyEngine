#include "pch.h"
#include "CBossDropStarScript.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CBossMgr.h"

CBossDropStarScript::CBossDropStarScript()
    : CScript(BOSSDROPSTARSCRIPT)
    , m_Light(nullptr)
    , m_LifeTime(3.f)
    , m_AccTime(0.f)
    , m_LightAccTime(0.f)
{
}

CBossDropStarScript::~CBossDropStarScript()
{
}

void CBossDropStarScript::begin()
{
    m_OriginScale = GetOwner()->Transform()->GetWorldScale();
    m_AccTime = 0.f;

    m_Star = GetOwner()->GetChildObject(L"WarpStar");
    m_Light = GetOwner()->GetChildObject(L"Point Light")->Light();

    /*  Vec3 RelativeForce = Transform()->GetWorldPos() - BOSS->Transform()->GetWorldPos();
      RelativeForce.y = 0.f;
      RelativeForce.Normalize();

      Rigidbody()->AddForce(Vec3(0.f, 30.f, 0.f), ForceMode::Impulse);
      Rigidbody()->AddForce(RelativeForce * 10.f, ForceMode::Impulse);*/

    if (BOSS)
    {
        Vec3 RelativeForce = Transform()->GetWorldPos() - BOSS->Transform()->GetWorldPos();
        RelativeForce.y = 0.f;
        RelativeForce.Normalize();

        Rigidbody()->AddForce(RelativeForce * 10.f, ForceMode::Impulse);
    }
}

void CBossDropStarScript::tick()
{
    m_AccTime += DT;
    float StartDissapear = m_LifeTime - 0.5f;

    // billboard
    Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
    LookDir.y = 0.f;
    Transform()->SetDirection(LookDir);

    // rotate
    Vec3 Force = Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500.f;
    Rigidbody()->AddTorque(Force * DT);
    Vec3 StarRot = m_Star->Transform()->GetLocalRotation();
    StarRot.z += 2.f * DT;
    m_Star->Transform()->SetLocalRotation(StarRot);

    // change light color
    float period = 2.f;
    m_LightAccTime += DT;
    if (m_LightAccTime > period)
    {
        m_LightAccTime = 0.f;
    }

    float Ratio = m_LightAccTime / period;
    float Delta1 = (sin(XM_PI * 2.f * Ratio + XM_PI / 2.f) + 1.f) / 2.f; // 0~1·Î ¸ÅÇÎ
    float Delta2 = 1 - Delta1;
    Vec3 LightColor = Vec3(300.f, 100.f, 0.f) * Delta1 + Vec3(400.f, 400.f, 0.f) * Delta2;
    LightColor /= 255.f;

    m_Light->SetLightRadiance(LightColor);

    // End : destroy
    if (m_AccTime > m_LifeTime)
    {
        m_Light->SetLightRadiance(Vec3());
        GetOwner()->Transform()->SetWorldScale(Vec3());
        GamePlayStatic::DestroyGameObject(GetOwner());
    }

    // End : dissapear (scaling down)
    else if (m_AccTime > StartDissapear)
    {
        float t = (m_AccTime - StartDissapear) / 0.5f;
        t = 1.f - sinf(XM_PI / 2.f * t);
        GetOwner()->Transform()->SetWorldScale(m_OriginScale * t);

        m_Light->SetLightRadiance(LightColor * t);
    }
}

UINT CBossDropStarScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CBossDropStarScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}
