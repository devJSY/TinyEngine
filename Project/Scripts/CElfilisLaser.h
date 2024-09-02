#pragma once
#include <Engine\\CScript.h>

class CElfilisLaser : public CScript
{
private:
    Ptr<CPrefab> m_MagmaPref;
    Ptr<CPrefab> m_CollisionParticlePref;
    CGameObject* m_CollisionParticle;
    StateStep m_Step;
    bool m_bAutoPlay;
    bool m_bCollisionFloor;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

    void SetAutoPlay(bool _Play) { m_bAutoPlay = _Play; }
    void SetStart();
    void SetWait();
    void SetEnd();

public:
    StateStep GetStep() { return m_Step; }
    void SafeDestroy();

private:
    void Start();
    void Progress();
    void End();

    void PlayOuter(const wstring _Anim, bool _bRepeat = true);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisLaser);
    CElfilisLaser();
    CElfilisLaser(const CElfilisLaser& _Origin);
    virtual ~CElfilisLaser();
};
