#pragma once
#include <Engine\\CScript.h>

class CPlayerDetectScript : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CPlayerDetectScript);

public:
    CPlayerDetectScript();
    virtual ~CPlayerDetectScript();
};
