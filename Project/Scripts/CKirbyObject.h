#pragma once

// ====================================================================================
// USAGE: Kirby의 Object 종속적인 States를 모아 정의해둔 States Group Class
// ====================================================================================
// - Object 종속적인 기능(해당 class에 포함된 함수)은 State에서 call하는 방식으로 구현
// - Object와 관계없이 공유할 수 있는 기능은 State에서 직접 구현
// - Ability와 Object: 둘은 서로 관여하는 기능이 다르므로 서로 다른 parent class를 상속

class CKirbyAbility
{
protected:
    // 사용 메쉬: mesh

public:
    // 필수: Deform, Attack
    // 선택: Move

    virtual void Idle(){};
    virtual void IdleEnter(){};
    virtual void IdleExit(){};

    virtual void Run(){};
    virtual void RunEnter(){};
    virtual void RunExit(){};

    virtual void Jump(){};
    virtual void JumpEnter(){};
    virtual void JumpExit(){};

    // virtual void Hovering();

    // virtual void Evasiveness();

    // virtual void Sliding();

    virtual void Attack() = 0;
    virtual void AttackEnter() = 0;
    virtual void AttackExit() = 0;

    virtual void ChargeAttack1(){};
    virtual void ChargeAttack1Enter(){};
    virtual void ChargeAttack1Exit(){};

    virtual void ChargeAttack2(){};
    virtual void ChargeAttack2Enter(){};
    virtual void ChargeAttack2Exit(){};

    virtual void JumpAttack() = 0;
    virtual void JumpAttackEnter() = 0;
    virtual void JumpAttackExit() = 0;

    // virtual void Throw();

    // virtual void Deform();

public:
    virtual CKirbyAbility* Clone() = 0;

public:
    CKirbyAbility();
    virtual ~CKirbyAbility();
};
