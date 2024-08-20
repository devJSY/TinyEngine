#include "pch.h"
#include "CKirbyHatBlade.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CKirbyHatBlade::CKirbyHatBlade()
    : CScript(KIRBYHATBLADE)
    , m_AttackCoolTime(0.7f)
    , m_Acc(0.f)
    , m_bCanAttack(true)
    , m_VariationDuration(0.3f)
    , m_VariationAcc(0.f)
{
}

CKirbyHatBlade::~CKirbyHatBlade()
{
}

void CKirbyHatBlade::begin()
{
    Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
}

void CKirbyHatBlade::tick()
{

    // 공격을 할수 없는 상태일 경우
    if (!m_bCanAttack)
    {
        m_Acc += DT_ENGINE;

        // 쿨타임이 지났을 경우
        if (m_Acc > m_AttackCoolTime)
        {
            MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl", L"material\\MetalHat_BladeC.mtrl"), 0);
            Transform()->SetLocalScale(Vec3(0.f, 0.f, 0.f));

            m_bCanAttack = true;
            m_VariationAcc = 0.f;

            PLAYERFSM->SetBladeAttack(true);
        }
    }

    // 공격이 가능한 상태일 경우 Blade의 크기를 키워준다.
    if (m_bCanAttack && (m_VariationAcc <= m_VariationDuration))
    {
        m_VariationAcc += DT_ENGINE;

        Vec3 CurScale = Transform()->GetLocalScale();

        // 크기가 작다면 크기를 키워준다.
        if (CurScale < Vec3(1.f, 1.f, 1.f))
        {
            // Variation Duration 동안 일정한 속도로 크기를 키워준다.
            float Ratio = m_VariationAcc / m_VariationDuration;

            if (Ratio > 1.f)
            {
                Ratio = 1.f;
            }

            Vec3 VarScale = DirectX::SimpleMath::Vector3::Lerp(Vec3(0.f, 1.f, 0.f), Vec3(1.f, 1.f, 1.f), Ratio);

            Transform()->SetLocalScale(VarScale);
        }
    }
}

UINT CKirbyHatBlade::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbyHatBlade::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

void CKirbyHatBlade::Attack()
{
    // 공격을 하는 순간 물체가 렌더링안되게 한다.
    MeshRender()->SetMaterial(nullptr, 0);

    m_bCanAttack = false;
    m_Acc = 0.f;
    m_VariationAcc = 0.f;

    PLAYERFSM->SetBladeAttack(false);
}

void CKirbyHatBlade::Reset()
{
    // 날린 공격을 다시 받았을 경우

    PLAYERFSM->GetCurHat()->Animator()->Play(ANIMPREFIX("MetalCutterCatch"), false, false, 1.5f);
    //
    MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl", L"material\\MetalHat_BladeC.mtrl"), 0);
    Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

    m_bCanAttack = true;
    m_VariationAcc = m_VariationDuration + 0.1f;

    PLAYERFSM->SetBladeAttack(true);
}
