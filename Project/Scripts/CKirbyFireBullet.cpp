#include "pch.h"
#include "CKirbyFireBullet.h"

#include "CMonsterUnitScript.h"

CKirbyFireBullet::CKirbyFireBullet()
    : CScript(KIRBYFIREBULLET)
    , m_Step((UINT)Fire_Step::LV0)
    , m_AccFireNumber(0)
    , m_RequiredFireNumber(10)
    , m_Acc(0.f)
    , m_Duration(4.f)
    , m_bFirstTime(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Step, "Fire Level(0~2)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_RequiredFireNumber, "Required Fire Number");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CKirbyFireBullet::CKirbyFireBullet(const CKirbyFireBullet& _Origin)
    : CScript(KIRBYFIREBULLET)
    , m_Step(_Origin.m_Step)
    , m_AccFireNumber(_Origin.m_AccFireNumber)
    , m_RequiredFireNumber(_Origin.m_RequiredFireNumber)
    , m_Acc(_Origin.m_Acc)
    , m_Duration(_Origin.m_Duration)
    , m_bFirstTime(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_Step, "Fire Level(0~2)");
    AddScriptParam(SCRIPT_PARAM::INT, &m_RequiredFireNumber, "Required Fire Number");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CKirbyFireBullet::~CKirbyFireBullet()
{
}

void CKirbyFireBullet::begin()
{
    m_Acc = 0.f;
    m_AccFireNumber = 0;
    m_DestroyDuration = 0.1f;
    m_bDestroy = false;

    // Rigidbody가 없다면 추가한다.
    if (Rigidbody() == nullptr)
    {
        GetOwner()->AddComponent(new CRigidbody);
        Rigidbody()->SetUseGravity(true);
    }
    // Kirby에게서 나온것이라면 날라가게 한다.
    if (m_bFirstTime)
    {
        m_Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Rigidbody()->AddForce(m_Dir * 15.f, ForceMode::Impulse);
        m_bFirstTime = false;
    }
}

void CKirbyFireBullet::tick()
{
    m_Acc += DT;

    if (m_bIsFreeze)
    {
        Rigidbody()->SetFreezePosition(AXIS_TYPE::X, true);
        Rigidbody()->SetFreezePosition(AXIS_TYPE::Y, true);
        Rigidbody()->SetFreezePosition(AXIS_TYPE::Z, true);
        m_bIsFreeze = false;
    }

    if (m_bDestroy == false && m_Acc > m_Duration)
    {
        StepDown();
    }

    if (m_bDestroy && m_Acc > m_DestroyDuration)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CKirbyFireBullet::StepUp()
{
    Ptr<CPrefab> NextFirePref = nullptr;

    switch (m_Step)
    {
    case (UINT)Fire_Step::LV0:
    {
        NextFirePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV1.pref");
    }
        break;

    case (UINT)Fire_Step::LV1:
    {
        NextFirePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV2.pref");
    }
        break;

    case (UINT)Fire_Step::LV2:
    {
        return;
    }
        break;

    default:
        break;
    }

    if (NextFirePref.Get() != nullptr)
    {
        Vec3 CurPos = Transform()->GetWorldPos();

        // 다음 단계의 Fire 오브젝트를 생성하여 나의 자리에 넣어준다.
        CGameObject* NextFireObj = NextFirePref->Instantiate();
        NextFireObj->Transform()->SetWorldPos(CurPos);

        // 만든 오브젝트를 스폰
        GamePlayStatic::SpawnGameObject(NextFireObj, NextFireObj->GetLayerIdx());

        // 기존의 오브젝트는 삭제한다
        //GamePlayStatic::DestroyGameObject(GetOwner());

        GetOwner()->SphereCollider()->SetEnabled(false);
    }
}

void CKirbyFireBullet::StepDown()
{
    Ptr<CPrefab> NextFirePref = nullptr;

    switch (m_Step)
    {
    case (UINT)Fire_Step::LV0: {
    }
    break;

    case (UINT)Fire_Step::LV1: {
        NextFirePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV0.pref");
    }
    break;

    case (UINT)Fire_Step::LV2: {
        NextFirePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV1.pref");
    }
    break;

    default:
        break;
    }

    if (NextFirePref.Get() != nullptr)
    {
        Vec3 CurPos = Transform()->GetWorldPos();

        // 다음 단계의 Fire 오브젝트를 생성하여 나의 자리에 넣어준다.
        CGameObject* NextFireObj = NextFirePref->Instantiate();
        NextFireObj->Transform()->SetWorldPos(CurPos);

        // 만든 오브젝트를 스폰
        GamePlayStatic::SpawnGameObject(NextFireObj, NextFireObj->GetLayerIdx());

        // 기존의 오브젝트는 삭제한다
        // GamePlayStatic::DestroyGameObject(GetOwner());
    }
    GetOwner()->SphereCollider()->SetEnabled(false);

    m_Acc = 0.f;
    m_bDestroy = true;
}

void CKirbyFireBullet::OnCollisionEnter(CCollider* _OtherCollider)
{
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    // World Static이랑 충돌할 경우
    if (LayerIdx == LAYER_STATIC)
    {
        m_bIsFreeze = true;

        // @TODO Fire Decal 만들기
    }

    // FireBulluet(자기 자신과 충돌한 경우
    if (LayerIdx == LAYER_FIREBULLET)
    {
        CKirbyFireBullet* OtherFireScript = _OtherCollider->GetOwner()->GetScript<CKirbyFireBullet>();

        // 상대방이 FireScript가 없을 경우 삭제한다.
        if (OtherFireScript == nullptr)
        {
            GamePlayStatic::DestroyGameObject(_OtherCollider->GetOwner());
            return;
        }

        UINT OtherStep = OtherFireScript->GetSetp();

        // 상대가 나보다 단계가 높다면 아무처리도 하지 않는다.
        if (OtherStep > m_Step)
        {
            return;
        }
        else if (OtherStep == m_Step)
        {
            // 내 Acc가 더 짧은 경우 처리하지 않는다.
            float OtherAcc = OtherFireScript->GetAcc();
            if (OtherAcc >= m_Acc)
            {
                return;
            }
        }
        else if (m_Step == (UINT)Fire_Step::LV2)
        {
            m_Acc = 0.f;
        }

        // 상대보다 Step이 크거나 Acc가 큰 Fire의 경우 반대쪽 Fire를 삭제하고 자신의 AccNumber에 더해준다.
        GamePlayStatic::DestroyGameObject(_OtherCollider->GetOwner());
        ++m_AccFireNumber;

        // AccFireNumber가 필요한 개수가 넘어갔을 경우 불을 큰 불로 바꿔준다.
        if (m_AccFireNumber >= m_RequiredFireNumber)
        {
            StepUp();
        }
    }
}

void CKirbyFireBullet::OnTriggerEnter(CCollider* _OtherCollider)
{
    // Monster와 충돌
 
    UINT LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    if (LayerIdx == LAYER_MONSTER_TRIGGER && _OtherCollider->GetOwner()->GetName() == L"Body Collider")
    {
        float Damage = 3.f;

        if (m_Step == 1)
        {
            Damage = 6.f;
        }
        else if (m_Step == 2)
        {
            Damage = 9.f;
        }


        CUnitScript* Monster = _OtherCollider->GetOwner()->GetParent()->GetScript<CUnitScript>();
        if (nullptr != Monster)
        {
            Vec3 HitDir = _OtherCollider->Transform()->GetWorldPos() - Transform()->GetWorldPos();
            HitDir.Normalize();
            UnitHit HitInfo = {DAMAGE_TYPE::NORMAL, HitDir, Damage, 0.f, 0.f};

            Monster->GetDamage(HitInfo);
        }

        GamePlayStatic::DestroyGameObject(GetOwner());
    }

}

UINT CKirbyFireBullet::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Step, sizeof(UINT), 1, _File);
    fwrite(&m_RequiredFireNumber, sizeof(int), 1, _File);
    fwrite(&m_Duration, sizeof(float), 1, _File);

    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CKirbyFireBullet::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Step, sizeof(UINT), 1, _File);
    fread(&m_RequiredFireNumber, sizeof(int), 1, _File);
    fread(&m_Duration, sizeof(float), 1, _File);

    MemoryByte += sizeof(UINT);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
