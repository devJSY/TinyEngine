#include "pch.h"
#include "CElfilisUnit.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CState.h"

CElfilisUnit::CElfilisUnit()
    : CUnitScript(ELFILISUNIT)
{
    UnitInfo ElfilisInfo = {
        700.f, // HP
        700.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(ElfilisInfo);

    m_TeleportEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref", L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisUnit::CElfilisUnit(const CElfilisUnit& _Origin)
    : CUnitScript(_Origin)
{
    UnitInfo ElfilisInfo = {
        700.f, // HP
        700.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(ElfilisInfo);

    m_TeleportEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_ElfilisTeleport.pref", L"prefab\\Effect_ElfilisTeleport.pref");
}

CElfilisUnit::~CElfilisUnit()
{
}

void CElfilisUnit::tick()
{
    CUnitScript::tick();

    // Death & Resist
    if (m_CurInfo.HP <= m_InitInfo.MAXHP * 0.05f && !ELFFSM->IsResist())
    {
        ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();
        if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar))
        {
            ELFFSM->ResetFSM();
            m_CurInfo.HP = m_InitInfo.MAXHP * 0.05f;
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_RESIST");
        }
    }

    // Phase 1
    else if (ELFFSM->GetPhase() == 1)
    {
        // Cheat : Phase 1 -> Phase 2
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = m_InitInfo.HP * 0.4f;
        }

        ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();

        if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar) &&
            GetCurInfo().HP <= GetCurInfo().MAXHP * 0.4f)
        {
            ELFFSM->ResetFSM();
            ELFFSM->SetPhase(2);
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_DAMAGE");
        }
    }

    // Phase 2
    else if (ELFFSM->GetPhase() == 2)
    {
        // Cheat : Death
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = 0.f;
        }
    }
}

void CElfilisUnit::ResistSuccess()
{
    m_CurInfo.HP += m_CurInfo.MAXHP * 0.1f;
}

void CElfilisUnit::PlayTeleportEffect(CGameObject** _BeforeUnit, CGameObject** _BeforeEffect, CGameObject** _AfterEffect, Vec3 _Pos)
{
    // off collider
    ELFFSM->DisableCollider();

    // copy object
    CGameObject* BeforeUnit = new CGameObject;
    BeforeUnit->AddComponent(Transform()->Clone());
    BeforeUnit->AddComponent(MeshRender()->Clone());
    BeforeUnit->AddComponent(Animator()->Clone());
    BeforeUnit->SetName(L"Effect_ElfilisTelport Body");
    GamePlayStatic::SpawnGameObject(BeforeUnit, LAYER_MONSTER);
    *_BeforeUnit = BeforeUnit;

    // set shader
    Vec3 BeforePos = GetOwner()->Transform()->GetWorldPos();
    BeforePos.y += 100.f;
    ELFFSM->Teleport(BeforeUnit, 2, BeforePos.y);

    Vec3 AfterPos = _Pos;
    AfterPos.y += 100.f;
    ELFFSM->Teleport(1, AfterPos.y);

    if (m_TeleportEffect != nullptr)
    {
        // teleport effect (before object)
        CGameObject* BeforeEffect = m_TeleportEffect->Instantiate();
        BeforeEffect->Transform()->SetWorldPos(BeforePos);
        GamePlayStatic::SpawnGameObject(BeforeEffect, LAYER_EFFECT);
        *_BeforeEffect = BeforeEffect;

        // teleport effect (after object)
        CGameObject* AfterEffect = m_TeleportEffect->Instantiate();
        AfterEffect->Transform()->SetWorldPos(AfterPos);
        GamePlayStatic::SpawnGameObject(AfterEffect, LAYER_EFFECT);
        *_AfterEffect = AfterEffect;
    }

    // sound
    wstring Teleport;
    int RandIdx = GetRandomInt(0, 2);

    if (RandIdx == 0)
        Teleport = L"sound\\wav\\CharaBossChimera2\\0000_Teleport.wav";
    else if (RandIdx == 1)
        Teleport = L"sound\\wav\\CharaBossChimera2\\0001_Teleport.wav";
    else if (RandIdx == 2)
        Teleport = L"sound\\wav\\CharaBossChimeraSoul\\0000_Teleport.wav";

    GamePlayStatic::Play2DSound(Teleport, 1, SOUND_ELFILIS * 1.3f);
}

UINT CElfilisUnit::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    UnitInfo ElfilisInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(ElfilisInfo);
    MemoryByte += CUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CElfilisUnit::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);
    UnitInfo ElfilisInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(ElfilisInfo);
    return MemoryByte;
}