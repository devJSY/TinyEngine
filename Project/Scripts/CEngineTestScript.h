#pragma once
#include <Engine\\CScript.h>

class CEngineTestScript : public CScript
{
private:
    float m_Power;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void CharacterControllerTest();
    void AnimatorTest();
    void QuaternionExample();

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEngineTestScript);

public:
    CEngineTestScript();
    CEngineTestScript(const CEngineTestScript& origin);
    virtual ~CEngineTestScript();
};
