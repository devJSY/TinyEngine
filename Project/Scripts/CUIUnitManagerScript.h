#pragma once
#include <Engine/CScript.h>

enum class UnitTask
{
};

class CUIUnitManagerScript : public CScript
{
private:
    vector<UnitTask> m_vUnitTasks;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE_DISABLE(CUIUnitManagerScript)
public:
    CUIUnitManagerScript();
    virtual ~CUIUnitManagerScript();
};
