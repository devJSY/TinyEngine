#pragma once
#include <Engine\\CScript.h>

class CDetectAreaScript : public CScript
{
private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override{};
    virtual UINT LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE(CDetectAreaScript);
    CDetectAreaScript();
    virtual ~CDetectAreaScript();
};
