#pragma once
#include <Engine\\CScript.h>

class CPlayerCameraScript : public CScript
{
private:
    CGameObject* m_Player;
    Vec3 m_OffsetPos;
    float m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerCameraScript);

public:
    CPlayerCameraScript();
    CPlayerCameraScript(const CPlayerCameraScript& origin); // 멤버 변수 AddScriptParam() 등록시 주소를 전달하기 때문에 복사생성자를 구현해줘야함
    virtual ~CPlayerCameraScript();
};
