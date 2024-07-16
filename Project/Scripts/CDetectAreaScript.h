#pragma once
#include <Engine\\CScript.h>

class CDetectAreaScript : public CScript
{
private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CDetectAreaScript);
    CDetectAreaScript();
    virtual ~CDetectAreaScript();
};
