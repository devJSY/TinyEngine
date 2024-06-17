#pragma once

#include <Engine/CScript.h>

class CKirbyMoveController : public CScript
{
private:
    Vec3 m_Dir;
    Vec3 m_LookDir;

    bool m_Move;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void SetDir();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CKirbyMoveController);
    CKirbyMoveController();
    CKirbyMoveController(const CKirbyMoveController& _Origin);
    virtual ~CKirbyMoveController();
};
