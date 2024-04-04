#pragma once
#include <Engine\\CScript.h>

class CBridgeScript : public CScript
{
private:
    bool m_bEnabled;
    float m_PassedTime;
    
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetEnabled(bool _bEnable);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE(CBridgeScript);

public:
    CBridgeScript();
    virtual ~CBridgeScript();
};
