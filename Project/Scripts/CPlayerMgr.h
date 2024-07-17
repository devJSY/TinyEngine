#pragma once
#include <Engine\\CScript.h>
#include <Engine\\CMaterial.h>

class CKirbyUnitScript;
class CKirbyFSM;
class CKirbyMoveController;

enum class FaceType
{
    Normal,
    Half,
    Close,
    Frown,
    UpTail,
    Surprised,
    DownTail,
};

struct KirbyMeshIdx
{
    UINT BodyNormal;
    UINT BodyBig;
    UINT BodyVacuum;
    UINT MouthNormal;
    UINT MouthOpen;
    UINT MouthSmileClose;
    UINT MouthSmileOpen;
    UINT MouthAngryClose;
    UINT limbs;
};

class CPlayerMgr : public CScript

{
private:
    static CGameObject*             m_PlayerObj;
    static CKirbyUnitScript*        m_PlayerUnit;
    static CKirbyFSM*               m_PlayerFSM;
    static CKirbyMoveController*    m_PlayerController;
    static Ptr<CMaterial>           m_PlayerBodyMtrl;
    static Ptr<CMaterial>           m_PlayerBodyDemoMtrl;
    static KirbyMeshIdx             m_PlayerMeshIdx;

public:
    virtual void begin() override;
    virtual void tick() override;

    // void SavePlayerInfo();

public:
    void SetPlayer(CGameObject* _PlayerObj);
    static void SetPlayerMtrl(UINT _Idx, bool _On = true);
    static void SetPlayerFace(FaceType _Type);
    static void ClearBodyMtrl();
    static void ClearMouthMtrl();

    static CGameObject* GetPlayer() { return m_PlayerObj; }
    static CKirbyUnitScript* GetPlayerScript() { return m_PlayerUnit; }
    static CKirbyFSM* GetPlayerFSM() { return m_PlayerFSM; }
    static CKirbyMoveController* GetPlayerCtrl() { return m_PlayerController; }
    static Ptr<CMaterial> GetPlayerBodyMtrl() { return m_PlayerBodyMtrl; }
    static Ptr<CMaterial> GetPlayerBodyDemoMtrl() { return m_PlayerBodyDemoMtrl; }
    static KirbyMeshIdx GetPlayerMeshIdx() { return m_PlayerMeshIdx; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CPlayerMgr);

public:
    CPlayerMgr();
    virtual ~CPlayerMgr();
};