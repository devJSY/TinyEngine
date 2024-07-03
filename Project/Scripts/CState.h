#pragma once
#include <Engine\\CEntity.h>

#include <Engine\\CTimeMgr.h>
#include <Engine\\CKeyMgr.h>
#include <Engine\\components.h>
#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyObject.h"
#include "CKirbyAbility.h"

class CFSMScript;

class CState : public CEntity
{
private:
    CFSMScript* m_OwnerFSM; // 이 state를 소유한 FSM (AI)

public:
    virtual void tick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

public:
    CGameObject* GetOwner();

protected:
    void ChangeState(const wstring& _strStateName);
    CFSMScript* GetOwnerFSM() const { return m_OwnerFSM; }

public:

    CState* Clone() = 0;
public:
    CState();
    virtual ~CState();

    friend class CFSMScript;
};