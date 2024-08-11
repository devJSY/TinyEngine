#include "pch.h"
#include "CMorphoFSM.h"
#include "CState.h"
#include "CPlayerMgr.h"

CMorphoFSM::CMorphoFSM()
    : CFSMScript(MORPHOFSM)
    , m_CurStateGroup(MorphoStateGroup::END)
    , m_Pattern(MorphoPatternType::NONE)
    , m_PatternStep(0)
    , m_Phase(1)
    , m_ComboLevel(0)
    , m_NearDist(120.f)
    , m_bAttackRepeat(false)
    , m_MapFloorOffset(Vec3())
    , m_MapSize(Vec3(500.f, 0.f, 500.f))
    , m_TeleportAppearTime(0.f)
    , m_EmissiveTime(0.3f)
{
    for (UINT i = 0; i < (UINT)MorphoStateGroup::END; ++i)
    {
        m_StateGroup[(MorphoStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(MorphoStateGroup)i][1] = vector<wstring>();
    }
}

CMorphoFSM::~CMorphoFSM()
{
}

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
#include "CMorphoAtkA_ShockWave.h"
#include "CMorphoAtkA_DoubleSword.h"
#include "CMorphoAtkA_DoubleSwordDivision.h"
#include "CMorphoAtkA_DoubleSwordAtkR.h"
#include "CMorphoAtkA_DoubleSwordAtkL.h"
#include "CMorphoAtkA_DoubleSwordAtkLR.h"

#include "CMorphoMoveG_Teleport.h"
#include "CMorphoMoveG_Jump.h"
#include "CMorphoMoveG_HoverDash.h"
#include "CMorphoMoveA_Teleport.h"
void CMorphoFSM::begin()
{
    // add state
    AddGroupPublicState(MorphoStateGroup::Idle, L"IDLE", new CMorpho_Idle);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK1", new CMorphoAtkG_NormalNear_Atk1);
    // AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATKFLIGHT", new CMorphoAtkG_NormalNear_AtkFlight);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalFar, L"ATKG_NORMALFAR_SWORDSLASH", new CMorphoAtkG_NormalFar_SwordSlash);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_FIREWALL", new CMorphoAtkG_Teleport_FireWall);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_TORNADO", new CMorphoAtkG_Teleport_Tornado);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_TRACKINGSOUL", new CMorphoAtkG_Teleport_TrackingSoul);
    AddGroupPublicState(MorphoStateGroup::AtkAir, L"ATKA_SHOCKWAVE", new CMorphoAtkA_ShockWave);
    AddGroupPublicState(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD", new CMorphoAtkA_DoubleSword);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT", new CMorphoMoveG_Teleport);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_JUMP", new CMorphoMoveG_Jump);
    AddGroupPublicState(MorphoStateGroup::MoveToGround, L"MOVEG_HOVERDASH", new CMorphoMoveG_HoverDash);
    AddGroupPublicState(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT", new CMorphoMoveA_Teleport);

    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK2", new CMorphoAtkG_NormalNear_Atk2);
    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK3", new CMorphoAtkG_NormalNear_Atk3);
    AddGroupPrivateState(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_DIVISION", new CMorphoAtkA_DoubleSwordDivision);
    AddGroupPrivateState(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKR", new CMorphoAtkA_DoubleSwordAtkR);
    AddGroupPrivateState(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKL", new CMorphoAtkA_DoubleSwordAtkL);
    AddGroupPrivateState(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKLR", new CMorphoAtkA_DoubleSwordAtkLR);

    ChangeStateGroup(MorphoStateGroup::Idle);

    // childs
    m_WeaponL = GetOwner()->GetChildObject(L"BossMorphoSwordL");
    m_WeaponR = GetOwner()->GetChildObject(L"BossMorphoSwordR");
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveL"));
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveR"));

    // get mtrl
    for (int i = 0; i < (int)MeshRender()->GetMtrlCount(); ++i)
    {
        m_listBodyMtrl.push_back(MeshRender()->GetMaterial(i));
    }
    
    if (m_WeaponL)
    {
        for (int i = 0; i < (int)m_WeaponL->MeshRender()->GetMtrlCount(); ++i)
        {
            m_listWeaponMtrl.push_back(m_WeaponL->MeshRender()->GetMaterial(i));
        }
    }

    if (m_WeaponR)
    {
        for (int i = 0; i < (int)m_WeaponR->MeshRender()->GetMtrlCount(); ++i)
        {
            m_listWeaponMtrl.push_back(m_WeaponR->MeshRender()->GetMaterial(i));
        }
    }
}

void CMorphoFSM::tick()
{
    CFSMScript::tick();

    if (KEY_TAP(KEY::ENTER))
    {
        ChangeStateGroup(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD");
    }

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

            SetEmissive(Color);
        }
        else
        {
            m_TeleportAppearTime = 0.f;
            ClearEmissive();
        }
    }
}

void CMorphoFSM::Move()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    //@TODO ������ ����
    ChangeStateGroup(MorphoStateGroup::Idle);
    return;

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

void CMorphoFSM::Attack()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    //@TODO ������ ����
    ChangeStateGroup(MorphoStateGroup::Idle);
    return;

    // Move (To Ground)
    if (m_CurStateGroup == MorphoStateGroup::MoveToGround)
    {
        float Rand = GetRandomfloat(1.f, 100.f);

        // wait
        if (Rand <= 10.f)
        {
            ChangeStateGroup(MorphoStateGroup::AtkGroundWait);
        }

        // attack
        else
        {
            // attack normal
            if (Rand <= 55.f)
            {
                if (IsNearPlayer())
                {
                    ChangeStateGroup(MorphoStateGroup::AtkGroundNormalNear);
                }
                else
                {
                    ChangeStateGroup(MorphoStateGroup::AtkGroundNormalFar);
                }
            }

            // attack teleport
            else
            {
                ChangeStateGroup(MorphoStateGroup::AtkGroundTeleport);
            }
        }
    }

    // Move (To Air)
    else if (m_CurStateGroup == MorphoStateGroup::MoveToAir)
    {
        ChangeStateGroup(MorphoStateGroup::AtkAir);
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

    int Random = GetRandomInt(0, (int)m_StateGroup[_Group][0].size() - 1);
    wstring RandState = m_StateGroup[_Group][0][Random];

    m_CurStateGroup = _Group;
    ChangeState(RandState);
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
    // �̹� �����ϴ� Group���� �߰�
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][0].push_back(_StateName);
}

void CMorphoFSM::AddGroupPrivateState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // �̹� �����ϴ� Group���� �߰�
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
    case MorphoPatternType::DoubleSword:
    {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToAir, L"MOVEA_TELEPORT");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_DIVISION");
        }
        else if (m_PatternStep == 2) // �ܺ�ȣ��
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT");
        }
        else if (m_PatternStep == 3)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKR");
        }
        else if (m_PatternStep == 4) // �ܺ�ȣ��
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT");
        }
        else if (m_PatternStep == 5)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKL");
        }
        else if (m_PatternStep == 6) // �ܺ�ȣ��
        {
            ChangeStateGroup_Set(MorphoStateGroup::MoveToGround, L"MOVEG_TELEPORT");
        }
        else if (m_PatternStep == 7)
        {
            ChangeStateGroup_Set(MorphoStateGroup::AtkAir, L"ATKA_DOUBLESWORD_ATKLR");
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

void CMorphoFSM::ClearEmissive()
{
    for (Ptr<CMaterial> iter : m_listBodyMtrl)
    {
        iter->SetEmission(Vec4());
    }
    for (Ptr<CMaterial> iter : m_listWeaponMtrl)
    {
        iter->SetEmission(Vec4());
    }
}

void CMorphoFSM::SetEmissive(Vec3 _Color)
{
    for (Ptr<CMaterial> iter : m_listBodyMtrl)
    {
        iter->SetEmission(Vec4(_Color.x, _Color.y, _Color.z, 0.f));
    }
    for (Ptr<CMaterial> iter : m_listWeaponMtrl)
    {
        iter->SetEmission(Vec4(_Color.x, _Color.y, _Color.z, 0.f));
    }
}

void CMorphoFSM::SetWeaponEmissive(Vec3 _Color)
{
    for (Ptr<CMaterial> iter : m_listWeaponMtrl)
    {
        iter->SetEmission(Vec4(_Color.x, _Color.y, _Color.z, 0.f));
    }
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