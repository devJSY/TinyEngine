#pragma once
#include "CUIAnimScript.h"

enum class DirState
{
    Stop,
    Up,
    Down,
    End,
};

class CUIMoveUpDownScript : public CUIAnimScript
{
private:
    Vec3 m_vOriginPos;
    DirState m_eState;
    float m_fUpDistance;
    float m_fSpeed;

public:
    void SetState(DirState _state) { m_eState = _state; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void Idle() override;

private:
    void MoveUp();
    void MoveDown();

    Vec3 MoveTransform();
    void TransformApply(Vec3 _vPos);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIMoveUpDownScript)
public:
    CUIMoveUpDownScript();
    CUIMoveUpDownScript(const CUIMoveUpDownScript& Origin);
    virtual ~CUIMoveUpDownScript();
};
