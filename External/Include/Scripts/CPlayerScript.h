#pragma once
#include <Engine\\CScript.h>

class CPlayerScript : public CScript
{
private:
    float m_Speed;
    float m_Force;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider);
    virtual void OnCollisionStay(CCollider2D* _OtherCollider);
    virtual void OnCollisionExit(CCollider2D* _OtherCollider);

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider);
    virtual void OnTriggerStay(CCollider2D* _OtherCollider);
    virtual void OnTriggerExit(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};
