#include "pch.h"
#include "CMorphoShockWaveWave.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"

CMorphoShockWaveWave::CMorphoShockWaveWave()
    : CScript(MORPHOSHOCKWAVEWAVE)
    , m_AccTime(0.f)
    , m_PlayTime(1.5f)
{
}

CMorphoShockWaveWave::~CMorphoShockWaveWave()
{
}

void CMorphoShockWaveWave::begin()
{
    m_OriginScale = Transform()->GetLocalScale();
    MeshRender()->GetDynamicMaterial(0);
}

void CMorphoShockWaveWave::tick()
{
    m_AccTime += DT;

    // billboard
    Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
    Transform()->SetDirection(LookDir);

    if (m_AccTime <= m_PlayTime)
    {
        float t = m_AccTime / m_PlayTime;
        t = sinf(XM_PI / 2.f * t);
        Transform()->SetLocalScale(m_OriginScale * t);

        float Alpha = cosf(XM_PI / 2.f * t);
        MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, Alpha);
    }
    else
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CMorphoShockWaveWave::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoShockWaveWave::LoadFromLevelFile(FILE* _File)
{
    return 0;
}