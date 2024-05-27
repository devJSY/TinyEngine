#pragma once
#include <Engine\\CScript.h>

class CSoundManagerScript : public CScript
{
private:
    static CSoundManagerScript* m_Inst;

public:
    static CSoundManagerScript* GetInset();

public:
    void PlayBackGround();
    void ClearAllSound();

public:
    virtual void begin() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE_DISABLE(CSoundManagerScript);

public:
    CSoundManagerScript();
    virtual ~CSoundManagerScript();
};
