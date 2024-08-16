#pragma once

#include <Engine/CScript.h>

class CStageClear : public CScript
{
private:
    bool m_bKirbyDance;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    CLONE(CStageClear);
    CStageClear();
    CStageClear(const CStageClear& _Origin);
    virtual ~CStageClear();

};
