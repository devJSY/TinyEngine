#pragma once

#include <Engine\\CScript.h>

class CGroundScript : public CScript
{
private:
    float m_Life;

public:
    virtual void tick();

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider);
    virtual void OnCollisionStay(CCollider2D* _OtherCollider);
    virtual void OnCollisionExit(CCollider2D* _OtherCollider);

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider);
    virtual void OnTriggerStay(CCollider2D* _OtherCollider);
    virtual void OnTriggerExit(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CGroundScript);

public:
    CGroundScript();
    virtual ~CGroundScript();
};
