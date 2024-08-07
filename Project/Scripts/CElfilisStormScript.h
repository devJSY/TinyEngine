#pragma once
#include <Engine\\CScript.h>

class CElfilisStormScript : public CScript
{
private:
    CGameObject* m_StormWhite;
    CGameObject* m_StormRed;
    Vec3 m_CenterPosition;
    Vec3 m_OriginScale;
    int m_StormIdx;
    float m_AccTime;
    float m_PlayTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetInitPosition(Vec3 _Pos) { m_CenterPosition = _Pos; }
    void SetStormIdx(int _Idx) { m_StormIdx = _Idx; }
    float GetRemainTime() { return m_PlayTime - m_AccTime; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisStormScript);
    CElfilisStormScript();
    CElfilisStormScript(const CElfilisStormScript& _Origin);
    virtual ~CElfilisStormScript();
};
