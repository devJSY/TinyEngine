#pragma once
#include <Engine\\CScript.h>

class CEnemyColliderScript : public CScript
{
private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionExit(CCollider2D* _OtherCollider) override;

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CEnemyColliderScript);

public:
    CEnemyColliderScript();
    virtual ~CEnemyColliderScript();
};
