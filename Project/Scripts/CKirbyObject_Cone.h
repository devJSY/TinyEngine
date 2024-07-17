#pragma once
#include "CKirbyObject.h"

class CKirbyObject_Cone : public CKirbyObject
{
private:
    Ptr<CMeshData> m_Crater;
    bool m_bFrmEnter;

public:
    //virtual void Idle();
    //virtual void IdleEnter();
    //virtual void IdleExit();

    //virtual void Run(){};
    //virtual void RunEnter(){};
    //virtual void RunExit(){};

    //virtual void RunStart(){};
    //virtual void RunStartEnter(){};
    //virtual void RunStartExit(){};

    //virtual void Jump(){};
    //virtual void JumpEnter(){};
    //virtual void JumpExit(){};

    //virtual void JumpStart(){};
    //virtual void JumpStartEnter(){};
    //virtual void JumpStartExit(){};

    //virtual void JumpFall(){};
    //virtual void JumpFallEnter(){};
    //virtual void JumpFallExit(){};

    //virtual void Landing(){};
    //virtual void LandingEnter();
    //virtual void LandingExit();

    //virtual void LandingEnd(){};
    //virtual void LandingEndEnter();
    //virtual void LandingEndExit();

    virtual void Attack() {};
    virtual void AttackEnter(){};
    virtual void AttackExit(){};

    //virtual void AttackEnd(){};
    //virtual void AttackEndEnter(){};
    //virtual void AttackEndExit(){};

    //virtual void AttackCharge1(){};
    //virtual void AttackCharge1Enter(){};
    //virtual void AttackCharge1Exit(){};

    //virtual void AttackCharge1Start(){};
    //virtual void AttackCharge1StartEnter(){};
    //virtual void AttackCharge1StartExit(){};

    //virtual void AttackCharge1End(){};
    //virtual void AttackCharge1EndEnter(){};
    //virtual void AttackCharge1EndExit(){};

    //virtual void AttackCharge2(){};
    //virtual void AttackCharge2Enter(){};
    //virtual void AttackCharge2Exit(){};

    //virtual void AttackCharge2Start(){};
    //virtual void AttackCharge2StartEnter(){};
    //virtual void AttackCharge2StartExit(){};

    //virtual void AttackCharge3(){};
    //virtual void AttackCharge3Enter(){};
    //virtual void AttackCharge3Exit(){};

    //virtual void AttackCharge3Start(){};
    //virtual void AttackCharge3StartEnter(){};
    //virtual void AttackCharge3StartExit(){};

    //virtual void AttackCharge3End(){};
    //virtual void AttackCharge3EndEnter(){};
    //virtual void AttackCharge3EndExit(){};

    //virtual void JumpAttack(){};
    //virtual void JumpAttackEnter(){};
    //virtual void JumpAttackExit(){};

    //virtual void JumpAttackStart(){};
    //virtual void JumpAttackStartEnter(){};
    //virtual void JumpAttackStartExit(){};

    // Change/Drop Object
    virtual void DropObject();
    virtual void DropObjectEnter();
    virtual void DropObjectExit();

    //virtual void ChangeObject();
    //virtual void ChangeObjectEnter();

public:
    CLONE_DISABLE(CKirbyObject_Cone)
    CKirbyObject_Cone();
    virtual ~CKirbyObject_Cone();
};
