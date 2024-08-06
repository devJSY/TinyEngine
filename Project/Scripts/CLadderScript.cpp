#include "pch.h"
#include "CLadderScript.h"

#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"

CLadderScript::CLadderScript()
    : CScript(LADDERSCRIPT)
{
}

CLadderScript::CLadderScript(const CLadderScript& _Origin)
    : CScript(LADDERSCRIPT)
{
}

CLadderScript::~CLadderScript()
{
}

void CLadderScript::begin()
{
    Vec3 WorldPos = Transform()->GetWorldPos();
    Vec3 WorldScale = Transform()->GetWorldScale();
    WorldScale.x = 0.f;
    WorldScale.z = 0.f;

    m_UpDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT).Normalize();
    m_DownDir = Transform()->GetWorldDir(DIR_TYPE::FRONT).Normalize();
    m_TopPos = WorldPos + WorldScale;
    m_BottomPos = WorldPos;
    m_Scale = WorldScale.y;

    m_PlayerCapsuleScale = 30.f;
    m_Pivot = 5.f;
}

UINT CLadderScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CLadderScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}

void CLadderScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {
        PLAYERFSM->SetLadderUpSightDir(m_UpDir);
        PLAYERFSM->SetLadderDownSightDir(m_DownDir);
        PLAYERFSM->SetLadderTop(m_TopPos);
        PLAYERFSM->SetLadderBottom(m_BottomPos);
        PLAYERFSM->SetEscapeLadder(false);

    }

}

void CLadderScript::OnTriggerStay(CCollider* _OtherCollider)
{
    // PLAYER랑 충돌한 경우
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {
        Vec3 InputDir = PLAYERCTRL->GetInputWorld().Normalize();
        Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();

        // 입력이 없으면 사다리를 타지 않는다
        if (InputDir.Length() == 0.f)
            return;

        // 플레이어의 위치가 사다리의 약간 아래보다 낮은 경우
        if (PlayerPos.y < m_TopPos.y - m_PlayerCapsuleScale)
        {
            // 플레이어의 입력이 벽쪽(올라가는 쪽)이라면 플레이어를 사다리를 타게 한다(현재 플레이어의 위치에서).
            if (!PLAYERFSM->GetCollisionLadder() && InputDir.Dot(m_UpDir) > cosf(60.f * XM_PI / 180.f))
            {
                // 플레이어의 방향 설정 (사다리의 Up방향(Back)을 바라보도록 한다).
                PLAYERCTRL->ForceDir(ForceDirInfo{ForceDirType::STATE, m_UpDir, true});

                // 플레이어의 위치 설정(사다리의 중앙에서 Down방향(Front) 피벗만큼 떨어진거리
                Vec3 NewPlayerPos = m_BottomPos + m_DownDir * m_Pivot;
                NewPlayerPos.y = PlayerPos.y;

                PLAYER->Transform()->SetWorldPos(NewPlayerPos);

                PLAYERCTRL->ClearVelocityY();

                // 플레이어의 상태 변경
                PLAYERFSM->ChangeState(L"LADDER_WAIT");
            }

        }
        // 플레이어가 사다리 위쪽에 있는 경우
        else
        {
            // 플레이어의 입력이 공중 쪽(내려가는 쪽)이라면 플레이어를 사다리를 타게 한다.
            if (!PLAYERFSM->GetCollisionLadder() && InputDir.Dot(m_DownDir) > cosf(60.f * XM_PI / 180.f))
            {
                // 플레이어의 방향 설정 (사다리의 Up방향(Back)을 바라보도록 한다).
                PLAYERCTRL->ForceDir(ForceDirInfo{ForceDirType::STATE, m_UpDir, true});

                // 플레이어의 위치 설정(사다리의 중앙에서 Down방향(Front) 피벗만큼 떨어진거리
                Vec3 NewPlayerPos = m_TopPos + m_DownDir * m_Pivot;
                NewPlayerPos.y -= m_PlayerCapsuleScale;
                PLAYER->Transform()->SetWorldPos(NewPlayerPos);

                PLAYERCTRL->ClearVelocityY();

                // 플레이어의 상태 변경
                PLAYERFSM->ChangeState(L"LADDER_WAIT");
            }

            // 플레이어의 입력이 벽쪽(올라가는 쪽)이라면 플레이어를 사다리에서 벗어나게 한다.
            if (PLAYERFSM->GetCollisionLadder() && InputDir.Dot(m_UpDir) > 0.f && PLAYERFSM->GetEscapeLadder() == false)
            {
                // 플레이어의 위치 설정
                Vec3 NewPlayerPos = m_TopPos;
                NewPlayerPos.y -= 10.f;
                PLAYER->Transform()->SetWorldPos(NewPlayerPos);

                PLAYERCTRL->ClearVelocityY();

                // 플레이어의 상태 변경
                PLAYERFSM->ChangeState(L"LADDER_EXIT");

                // 다시 충돌하지 않도록 Player를 사다리에서 탈출 중인 상태로 변경
                PLAYERFSM->SetEscapeLadder(true);
            }
        }
    }

}

void CLadderScript::OnTriggerExit(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_PLAYER)
    {
        // 사다리와 다시 충돌할수 있도록 escape를 꺼준다.
        PLAYERFSM->SetEscapeLadder(false);

        PLAYERFSM->SetCollisionLadder(false);
    }

}
