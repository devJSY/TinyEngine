#pragma once

#include <Engine/CScript.h>

// Transform의 Scale,Pos 사용
// Sclae Absolute 사용(Transform)
// Front - Down, Back - Up
// Box Collider
// Collider의 center, scale 건들지 x

class CLadderScript : public CScript
{
private:
    Vec3 m_UpDir;
    Vec3 m_DownDir;
    Vec3 m_TopPos;
    Vec3 m_BottomPos;
    float m_Scale; // 사다리 세로 길이
    float m_PlayerCapsuleScale;
    float m_Pivot; // 사다리의 중심부터 플레이어까지의 거리

public:
    virtual void begin() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    CLONE(CLadderScript);
    CLadderScript();
    CLadderScript(const CLadderScript& _Origin);
    virtual ~CLadderScript();
};
