#include "pch.h"
#include "CMorphoFSM.h"
#include "CState.h"
#include "CPlayerMgr.h"
#include <Engine\CMaterial.h>

CMorphoFSM::CMorphoFSM()
    : CFSMScript(MORPHOFSM)
    , m_CurStateGroup(MorphoStateGroup::END)
    , m_Pattern(MorphoPatternType::NONE)
    , m_PatternStep(0)
    , m_Phase(1)
    , m_ComboLevel(0)
    , m_NearDist(150.f)
    , m_BodyCollider(nullptr)
    , m_WeaponL(nullptr)
    , m_WeaponR(nullptr)
    , m_bAttackRepeat(false)
    , m_TeleportAppearTime(0.f)
    , m_EmissiveTime(0.3f)
    , m_MapFloorOffset(Vec3())
    , m_MapSize(Vec3(325.f, 0.f, 325.f))
{
    for (UINT i = 0; i < (UINT)MorphoStateGroup::END; ++i)
    {
        m_StateGroup[(MorphoStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(MorphoStateGroup)i][1] = vector<wstring>();
    }

    m_DropStarPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BossDropStar.pref");
}

CMorphoFSM::CMorphoFSM(const CMorphoFSM& _Origin)
    : CFSMScript(_Origin)
    , m_StateGroup{}
    , m_StateSelectionCount{}
    , m_CurStateGroup(MorphoStateGroup::END)
    , m_Pattern(MorphoPatternType::NONE)
    , m_PatternStep(0)
    , m_Phase(1)
    , m_ComboLevel(0)
    , m_NearDist(_Origin.m_NearDist)
    , m_bAttackRepeat(false)
    , m_BodyCollider(nullptr)
    , m_WeaponL(nullptr)
    , m_WeaponR(nullptr)
    , m_vecShockWave{}
    , m_listBodyMtrl{}
    , m_listWeaponMtrl{}
    , m_listBodyEmissive{}
    , m_listWeaponEmissive{}
    , m_TeleportAppearTime(0.f)
    , m_EmissiveTime(_Origin.m_EmissiveTime)
    , m_MapFloorOffset(_Origin.m_MapFloorOffset)
    , m_MapSize(_Origin.m_MapFloorOffset)
{
    for (UINT i = 0; i < (UINT)MorphoStateGroup::END; ++i)
    {
        m_StateGroup[(MorphoStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(MorphoStateGroup)i][1] = vector<wstring>();
    }

    m_DropStarPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BossDropStar.pref");
}

CMorphoFSM::~CMorphoFSM()
{
}

#include "CMorphoDemo_Appear.h"
#include "CMorphoDemo_Phase2.h"
#include "CMorphoDemo_Death.h"

#include "CMorpho_Idle.h"
#include "CMorphoAtkG_NormalNear_Atk1.h"
#include "CMorphoAtkG_NormalNear_Atk2.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
// #include "CMorphoAtkG_NormalNear_AtkFlight.h"
#include "CMorphoAtkG_NormalFar_SwordSlash.h"
#include "CMorphoAtkG_Teleport_FireWall.h"
#include "CMorphoAtkG_Teleport_Tornado.h"
#include "CMorphoAtkG_Teleport_TrackingSoul.h"
#include "CMorphoAtkG_Teleport_TrackingSoulCombo.h"
#include "CMorphoAtkG_Wait_LeftSideMove.h"
#include "CMorphoAtkG_Wait_RightSideMove.h"
#include "CMorphoAtkG_Wait_FrontMove.h"
#include "CMorphoAtkA_ShockWave.h"
#include "CMorphoAtkA_ShockWaveCombo.h"
#include "CMorphoAtkA_DoubleSword.h"
#include "CMorphoAtkA_DoubleSwordDivision.h"
#include "CMorphoAtkA_DoubleSwordAtkR.h"
#include "CMorphoAtkA_DoubleSwordAtkL.h"
#include "CMorphoAtkA_DoubleSwordAtkLR.h"

#include "CMorphoMoveG_TeleportNear.h"
#include "CMorphoMoveG_TeleportFar.h"
#include "CMorphoMoveG_TeleportCombo.h"
#include "CMorphoMoveG_Jump.h"
#include "CMorphoMoveG_HoverDash.h"
#include "CMorphoMoveG_JumpFall.h"
#include "CMorphoMoveA_Teleport.h"
void CMorphoFSM::begin()
{
    CFSMScript::begin();

    // add state
    AddGroupPublicState(MorphoStateGroup::Idle, L"IDLE", new CMorpho_Idle);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK1", new CMorphoAtkG_NormalNear_Atk1);
    // AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATKFLIGHT", new CMorphoAtkG_NormalNear_AtkFlight);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalFar, L"ATKG_NORMALFAR_SWORDSLASH", new CMorphoAtkG_NormalFar_SwordSlash);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport1, L"ATKG_TELEPORT_FIREWALL", new CMorphoAtkG_Teleport_FireWall);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport1, L"ATKG_TELEPORT_TORNADO", new CMorphoAtkG_Teleport_Tornado);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport1, L"ATKG_TELEPORT_TRACKINGSOUL", new CMorphoAtkG_Teleport_TrackingSoul);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport2, L"ATKG_TELEPORT_TRACKINGSOULCOMBO", new CMorphoAtkG_Teleport_TrackingSoulCombo);
    AddGroupPublicState(MorphoStateGroup::AtkGroundWait, L"ATKG_WAIT_LEFTSIDEMOVE", new CMorphoAtkG_Wait_LeftSideMove);
    AddGroupPublicState(MorphoStateGroup::AtkGroundWait, L"ATKG_WAIT_RIGHTSIDEMOVE", new CMorphoAtkG_Wait_RightSideMove);
    AddGroupPublicState(MorphoStateGroup::AtkGroundWait, L"ATKG_WAIT_FRONTMOVE", new CMorphoAtkG_Wait_FrontMove);
    AddGroupPublicState(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE", new CMorphoAtkA_ShockWave);
    AddGroupPublicState(MorphoStateGroup::AtkAir2, L"ATKA_SHOCKWAVECOMBO", new CMorphoAtkA_ShockWaveCombo);
    AddGroupPublicState(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD", new CMorphoAtkA_DoubleSword);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_NEAR", new CMorphoMoveG_TeleportNear);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR", new CMorphoMoveG_TeleportFar);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORTCOMBO", new CMorphoMoveG_TeleportCombo);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_JUMP", new CMorphoMoveG_Jump);
    AddGroupPublicState(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT", new CMorphoMoveA_Teleport);

    AddGroupPrivateState(MorphoStateGroup::DEMO, L"DEMO_APPEAR", new CMorphoDemo_Appear);
    AddGroupPrivateState(MorphoStateGroup::DEMO, L"DEMO_PHASE2", new CMorphoDemo_Phase2);
    AddGroupPrivateState(MorphoStateGroup::DEMO, L"DEMO_DEATH", new CMorphoDemo_Death);
    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK2", new CMorphoAtkG_NormalNear_Atk2);
    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK3", new CMorphoAtkG_NormalNear_Atk3);
    AddGroupPrivateState(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_DIVISION", new CMorphoAtkA_DoubleSwordDivision);
    AddGroupPrivateState(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKR", new CMorphoAtkA_DoubleSwordAtkR);
    AddGroupPrivateState(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKL", new CMorphoAtkA_DoubleSwordAtkL);
    AddGroupPrivateState(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKLR", new CMorphoAtkA_DoubleSwordAtkLR);
    AddGroupPrivateState(MorphoStateGroup::MoveToGround, L"MOVEG_HOVERDASH", new CMorphoMoveG_HoverDash);
    AddGroupPrivateState(MorphoStateGroup::MoveToGround, L"MOVEG_JUMPFALL", new CMorphoMoveG_JumpFall);

    ChangeStateGroup(MorphoStateGroup::Idle);

    // childs
    m_BodyCollider = GetOwner()->GetChildObject(L"Body Collider")->CapsuleCollider();
    m_WeaponL = GetOwner()->GetChildObject(L"BossMorphoSwordL");
    m_WeaponR = GetOwner()->GetChildObject(L"BossMorphoSwordR");
    m_ParticleButterfly_Yellow = GetOwner()->GetChildObject(L"Particle_MorphoButterfly_Yellow");
    m_ParticleButterfly_Pink = GetOwner()->GetChildObject(L"Particle_MorphoButterfly_Pink");
    m_ParticleButterfly_YellowPink = GetOwner()->GetChildObject(L"Particle_MorphoButterfly_YellowPink");
    m_ParticleCircleDust = GetOwner()->GetChildObject(L"Particle_MorphoCircleDust");
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveL"));
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveR"));

    for (CGameObject* iter : m_vecShockWave)
    {
        iter->begin();
        iter->SetActive(false);
    }

    // get mtrl
    for (int i = 0; i < (int)MeshRender()->GetMtrlCount(); ++i)
    {
        Ptr<CMaterial> Mtrl = MeshRender()->GetDynamicMaterial(i);
        m_listBodyMtrl.push_back(Mtrl);
        m_listBodyEmissive.push_back(Mtrl->GetEmission());
    }

    if (m_WeaponL)
    {
        for (int i = 0; i < (int)m_WeaponL->MeshRender()->GetMtrlCount(); ++i)
        {
            Ptr<CMaterial> Mtrl = m_WeaponL->MeshRender()->GetDynamicMaterial(i);
            m_listWeaponMtrl.push_back(Mtrl);
            m_listWeaponEmissive.push_back(Mtrl->GetEmission());
        }

        if (m_WeaponL->BoxCollider())
        {
            m_WeaponL->BoxCollider()->SetEnabled(false);
        }
    }

    if (m_WeaponR)
    {
        for (int i = 0; i < (int)m_WeaponR->MeshRender()->GetMtrlCount(); ++i)
        {
            Ptr<CMaterial> Mtrl = m_WeaponR->MeshRender()->GetDynamicMaterial(i);
            m_listWeaponMtrl.push_back(Mtrl);
            m_listWeaponEmissive.push_back(Mtrl->GetEmission());
        }

        if (m_WeaponR->BoxCollider())
        {
            m_WeaponR->BoxCollider()->SetEnabled(false);
        }
    }
}

void CMorphoFSM::tick()
{
    CFSMScript::tick();

    //if (KEY_TAP(KEY::ENTER))
    //{
    //    ChangeStateGroup(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
    //}

    // Emissive
    if (m_TeleportAppearTime > 0.f)
    {
        m_TeleportAppearTime -= DT;

        if (m_TeleportAppearTime > 0.f)
        {
            float delta = m_TeleportAppearTime / MRPFSM->GetEmissiveTime();
            float t1 = 1.f - delta;
            float t2 = cosf(delta * XM_PI / 2.f);
            Vec3 Color = Vec3(t1);
            Color.x = t2;

            AddEmissive(Color);
        }
        else
        {
            m_TeleportAppearTime = 0.f;
            ResetEmissive();
        }
    }
}

void CMorphoFSM::Move()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    // case : In Air
    if (m_CurStateGroup == MorphoStateGroup::AtkAir1 || m_CurStateGroup == MorphoStateGroup::AtkAir2)
    {
        ChangeStateGroup(MorphoStateGroup::MoveToGround);

        if (GetCurState()->GetName() == L"MOVEG_JUMP")
        {
            ChangeState(L"MOVEG_HOVERDASH");
        }
    }

    // case : On Ground
    else
    {
        float Rand = GetRandomfloat(1.f, 100.f);

        if (Rand <= 70.f)
        {
            ChangeStateGroup(MorphoStateGroup::MoveToGround);
        }
        else
        {
            ChangeStateGroup(MorphoStateGroup::MoveToAir);
        }
    }
}

void CMorphoFSM::Attack()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    // case : On Ground
    if (m_CurStateGroup == MorphoStateGroup::MoveToGround || m_CurStateGroup == MorphoStateGroup::AtkGroundWait)
    {
        float Rand = GetRandomfloat(1.f, 100.f);

        // wait
        if (Rand <= 10.f && m_CurStateGroup == MorphoStateGroup::MoveToGround)
        {
            ChangeStateGroup(MorphoStateGroup::AtkGroundWait);
        }

        // attack
        else
        {
            if (IsNearPlayer())
            {
                ChangeStateGroup(MorphoStateGroup::AtkGroundNormalNear);
            }
            else
            {
                if (Rand <= 55.f)
                {
                    ChangeStateGroup(MorphoStateGroup::AtkGroundNormalFar);
                }
                else
                {
                    if (m_Phase == 1)
                    {
                        ChangeStateGroup(MorphoStateGroup::AtkGroundTeleport1);
                    }
                    else
                    {
                        if (Rand <= 90.f)
                        {
                            ChangeStateGroup(MorphoStateGroup::AtkGroundTeleport1);
                        }
                        else
                        {
                            ChangeStateGroup(MorphoStateGroup::AtkGroundTeleport2);
                        }
                    }
                }
            }
        }
    }

    // case : In Air
    else if (m_CurStateGroup == MorphoStateGroup::MoveToAir)
    {
        if (m_Phase == 1)
        {
            ChangeStateGroup(MorphoStateGroup::AtkAir1);
        }
        else
        {
            float Rand = GetRandomfloat(1.f, 100.f);

            if (Rand <= 50.f)
            {
                ChangeStateGroup(MorphoStateGroup::AtkAir1);
            }
            else
            {
                ChangeStateGroup(MorphoStateGroup::AtkAir2);
            }
        }
    }
}

void CMorphoFSM::RepeatState(wstring _State)
{
    m_bAttackRepeat = true;

    if (_State != L"")
    {
        ChangeState(_State);
    }
    else
    {
        ChangeState(GetCurState()->GetName());
    }
}

void CMorphoFSM::ChangeStateGroup(MorphoStateGroup _Group, const wstring& _State)
{
    if (m_Pattern != MorphoPatternType::NONE)
    {
        ProcPatternStep();
        return;
    }

    if (_State.empty())
    {
        ChangeStateGroup_Random(_Group);
    }
    else
    {
        ChangeStateGroup_Set(_Group, _State);
    }
}

void CMorphoFSM::ChangeStateGroup_Random(MorphoStateGroup _Group)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    // get min counted state
    UINT MinCount = UINT_MAX;
    vector<wstring> MinCountedStates;

    for (pair<wstring, UINT> iter : m_StateSelectionCount[(int)_Group])
    {
        if (iter.second > MinCount)
        {
            continue;
        }
        else if (iter.second == MinCount)
        {
            MinCountedStates.push_back(iter.first);
        }
        else
        {
            MinCount = iter.second;
            MinCountedStates.clear();
            MinCountedStates.push_back(iter.first);
        }
    }

    int Random = GetRandomInt(0, (int)MinCountedStates.size() - 1);
    wstring RandState = MinCountedStates[Random];

    m_CurStateGroup = _Group;
    ChangeState(RandState);
    m_StateSelectionCount[(int)_Group][RandState]++;
}

void CMorphoFSM::ChangeStateGroup_Set(MorphoStateGroup _Group, const wstring& _State)
{
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    vector<wstring>::iterator iter1 = find(m_StateGroup[_Group][0].begin(), m_StateGroup[_Group][0].end(), _State);
    vector<wstring>::iterator iter2 = find(m_StateGroup[_Group][1].begin(), m_StateGroup[_Group][1].end(), _State);
    if (iter1 == m_StateGroup[_Group][0].end() && iter2 == m_StateGroup[_Group][1].end())
        return;

    m_CurStateGroup = _Group;
    ChangeState(_State);
}

void CMorphoFSM::AddGroupPublicState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][0].push_back(_StateName);
    m_StateSelectionCount[(int)_Group][_StateName] = 0;
}

void CMorphoFSM::AddGroupPrivateState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][1].push_back(_StateName);
}

void CMorphoFSM::SetPattern(MorphoPatternType _Pattern)
{
    m_PatternStep = 0;
    m_Pattern = _Pattern;
}

void CMorphoFSM::ProcPatternStep()
{
    if (m_Pattern == MorphoPatternType::NONE)
        return;

    bool bFinish = false;

    switch (m_Pattern)
    {
    case MorphoPatternType::DoubleSword: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_DIVISION");
        }
        else if (m_PatternStep == 1) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_NEAR");
        }
        else if (m_PatternStep == 2)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKR");
        }
        else if (m_PatternStep == 3) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_NEAR");
        }
        else if (m_PatternStep == 4)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKL");
        }
        else if (m_PatternStep == 5) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
        }
        else if (m_PatternStep == 6)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKLR");
            bFinish = true;
        }
    }
    break;
    case MorphoPatternType::Demo_TeleportDoubleSword: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_DIVISION");
        }
        else if (m_PatternStep == 2) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_NEAR");
        }
        else if (m_PatternStep == 3)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKR");
        }
        else if (m_PatternStep == 4) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_NEAR");
        }
        else if (m_PatternStep == 5)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKL");
        }
        else if (m_PatternStep == 6) // 외부호출
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
        }
        else if (m_PatternStep == 7)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir2, L"ATKA_DOUBLESWORD_ATKLR");
            bFinish = true;
        }
    }
    break;
    case MorphoPatternType::TeleportCombo: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
        }
        else if (m_PatternStep == 2)
        {
            //@CAMERA
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
            bFinish = true;
        }
    }
    break;
    case MorphoPatternType::TrackingSoulCombo: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkGroundTeleport1, L"ATKG_TELEPORT_TRACKINGSOUL");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT_FAR");
        }
        else if (m_PatternStep == 2)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkGroundTeleport1, L"ATKG_TELEPORT_TRACKINGSOUL");
            bFinish = true;
        }
    }
    break;
    case MorphoPatternType::ShockWaveCombo: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
        }
        else if (m_PatternStep == 2)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 3)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
        }
        else if (m_PatternStep == 4)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 5)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
            float Rand = GetRandomfloat(1.f, 10.f);
            bFinish = (Rand > 5.f) ? true : false;
        }
        else if (m_PatternStep == 6)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 7)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
            float Rand = GetRandomfloat(1.f, 10.f);
            bFinish = (Rand > 5.f) ? true : false;
        }
        else if (m_PatternStep == 8)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 9)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir1, L"ATKA_SHOCKWAVE");
            bFinish = true;
        }
    }
    break;
    }

    if (bFinish)
    {
        SetPattern(MorphoPatternType::NONE);
    }
    else
    {
        m_PatternStep++;
    }
}

void CMorphoFSM::OnWeaponLTrigger()
{
    if (!m_WeaponL)
        return;

    m_WeaponL->BoxCollider()->SetEnabled(true);
}

void CMorphoFSM::OnWeaponRTrigger()
{
    if (!m_WeaponR)
        return;

    m_WeaponR->BoxCollider()->SetEnabled(true);
}

void CMorphoFSM::OffWeaponLTrigger()
{
    if (!m_WeaponL)
        return;

    m_WeaponL->BoxCollider()->SetEnabled(false);
}

void CMorphoFSM::OffWeaponRTrigger()
{
    if (!m_WeaponR)
        return;

    m_WeaponR->BoxCollider()->SetEnabled(false);
}

void CMorphoFSM::ResetFSM()
{
    SetGlobalState(false);
    SetPattern(MorphoPatternType::NONE);
    ClearComboLevel();
    EnableRender();
    EnableCollider();
    OffWeaponLTrigger();
    OffWeaponRTrigger();
    ResetEmissive();
}

void CMorphoFSM::SpawnDropStar(Vec3 _Pos)
{
    if (m_DropStarPref == nullptr)
        return;

    CGameObject* pDropStar = m_DropStarPref->Instantiate();
    pDropStar->Transform()->SetWorldPos(_Pos);
    GamePlayStatic::SpawnGameObject(pDropStar, LAYER_DYNAMIC);
}

void CMorphoFSM::DestroySumon()
{
    for (CGameObject* Obj : CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(LAYER_MONSTERATK)->GetParentObjects())
    {
        GamePlayStatic::DestroyGameObject(Obj);
    }
    
    for (CGameObject* Obj : CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(LAYER_MONSTERATK_TRIGGER)->GetParentObjects())
    {
        GamePlayStatic::DestroyGameObject(Obj);
    }
}

void CMorphoFSM::ResetEmissive()
{
    for (int i = 0; i < m_listBodyMtrl.size(); ++i)
    {
        m_listBodyMtrl[i]->SetEmission(Vec4(m_listBodyEmissive[i], 0.f));
    }

    for (int i = 0; i < m_listWeaponMtrl.size(); ++i)
    {
        m_listWeaponMtrl[i]->SetEmission(Vec4(m_listWeaponEmissive[i], 0.f));
    }
}

void CMorphoFSM::AddEmissive(Vec3 _Color)
{
    for (int i = 0; i < m_listBodyMtrl.size(); ++i)
    {
        Vec4 Color = Vec4(m_listBodyEmissive[i] + _Color, 0.f);
        m_listBodyMtrl[i]->SetEmission(Color);
    }

    for (int i = 0; i < m_listWeaponMtrl.size(); ++i)
    {
        Vec4 Color = Vec4(m_listWeaponEmissive[i] + _Color, 0.f);
        m_listWeaponMtrl[i]->SetEmission(Color);
    }
}

void CMorphoFSM::SetWeaponEmissive(Vec3 _Color)
{
    for (Ptr<CMaterial> iter : m_listWeaponMtrl)
    {
        iter->SetEmission(Vec4(_Color.x, _Color.y, _Color.z, 0.f));
    }
}

void CMorphoFSM::EnableTeleportParticle(bool _Enable)
{
    if (!m_ParticleButterfly_Yellow || !m_ParticleButterfly_YellowPink || !m_ParticleCircleDust)
        return;

    m_ParticleButterfly_Yellow->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, _Enable);
    m_ParticleButterfly_YellowPink->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, _Enable);
    m_ParticleCircleDust->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, _Enable);
}

void CMorphoFSM::EnableRender()
{
    MeshRender()->SetEnabled(true);
    m_WeaponL->MeshRender()->SetEnabled(true);
    m_WeaponR->MeshRender()->SetEnabled(true);
}

void CMorphoFSM::DisableRender()
{
    MeshRender()->SetEnabled(false);
    m_WeaponL->MeshRender()->SetEnabled(false);
    m_WeaponR->MeshRender()->SetEnabled(false);
}

void CMorphoFSM::EnableCollider()
{
    CapsuleCollider()->SetEnabled(true);
    
    if (m_BodyCollider)
    {
        m_BodyCollider->SetEnabled(true);
    }

    OnWeaponLTrigger();
    OnWeaponRTrigger();
}

void CMorphoFSM::DisableCollider()
{
    CapsuleCollider()->SetEnabled(false);

    if (m_BodyCollider)
    {
        m_BodyCollider->SetEnabled(false);
    }

    OffWeaponLTrigger();
    OffWeaponRTrigger();
}

void CMorphoFSM::SetTeleportTime(bool _Set)
{
    if (_Set)
    {
        m_TeleportAppearTime = m_EmissiveTime;
    }
    else
    {
        m_TeleportAppearTime = 0.f;
    }
}

float CMorphoFSM::GetPlayerDist() const
{
    Vec3 Dist = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dist.y = 0.f;
    return Dist.Length();
}

bool CMorphoFSM::IsGround() const
{
    bool bGround = false;
    Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
    float GroundHeight = GetOwner()->CapsuleCollider()->GetHeight() + GetOwner()->CapsuleCollider()->GetCenter().y;
    GroundHeight *= GetOwner()->Transform()->GetWorldScale().y;

    if (CurPos.y <= GroundHeight / 2.f && CurPos.y >= 0.f)
    {
        bGround = true;
    }

    return bGround;
}

UINT CMorphoFSM::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoFSM::LoadFromLevelFile(FILE* _File)
{
    return 0;
}