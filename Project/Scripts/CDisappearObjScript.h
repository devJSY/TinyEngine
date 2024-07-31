#pragma once
#include <Engine\CScript.h>

class CDisappearObjScript : public CScript
{
private:
    float m_fAccTime;
    float m_fBreakTime;
    float m_fCreateTime;
    bool m_bExist;

public:
    virtual void tick() override;

private:
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CDisappearObjScript);

public:
    CDisappearObjScript();
    CDisappearObjScript(const CDisappearObjScript& Origin);
    virtual ~CDisappearObjScript();
};
