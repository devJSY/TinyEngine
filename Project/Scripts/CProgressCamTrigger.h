#pragma once

#include <Engine/CScript.h>

class CCameraController;

class CProgressCamTrigger : public CScript
{
private:
    CCameraController* m_MainCamController;

    Vec3 m_StartPos;
    Vec3 m_EndPos;
    Vec3 m_StartOffset;
    Vec3 m_EndOffset;
    Vec3 m_StartDir;
    Vec3 m_EndDir;
    float m_StartDist;
    float m_EndDist;


public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    CLONE(CProgressCamTrigger);

public:
    CProgressCamTrigger();
    CProgressCamTrigger(const CProgressCamTrigger& _Origin);
    virtual ~CProgressCamTrigger();

};
