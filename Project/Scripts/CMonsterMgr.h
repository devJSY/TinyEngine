#pragma once

#include <Engine/CScript.h>

class CMonsterMgr:public CScript
{
private:
    CGameObject* m_pTargetObject;
    CTransform* m_pTargetObjectTr;

public:
    void begin();
    
public:
    bool IsTrack(Vec3 _position, float _trackDistance);
    Vec3 TrackDir(Vec3 _position) { return (m_pTargetObjectTr->GetLocalPos() - _position).Normalize(); }

private:
    HRESULT SetTarget();
    
    float CaculateDistance(Vec3 _position);

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE_DISABLE(CMonsterMgr)
    CMonsterMgr();
    virtual ~CMonsterMgr();
};
