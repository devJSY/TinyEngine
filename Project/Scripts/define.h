#pragma once

#define PLAYER CPlayerMgr::GetPlayer()
#define PLAYERUNIT CPlayerMgr::GetPlayerScript()
#define PLAYERFSM CPlayerMgr::GetPlayerFSM()
#define PLAYERCTRL CPlayerMgr::GetPlayerCtrl()
#define PLAYERMTRL CPlayerMgr::GetPlayerBodyMtrl()
#define PLAYERMESH(part) CPlayerMgr::GetPlayerMeshIdx().part

#define KEY_TAP_ARROW (KEY_TAP(KEY::UP) || KEY_TAP(KEY::DOWN) || KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT))
#define KEY_PRESSED_ARROW (KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
#define KEY_RELEASED_ARROW (KEY_RELEASED(KEY::UP) || KEY_RELEASED(KEY::DOWN) || KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
#define KEY_NONE_ARROW (KEY_NONE(KEY::UP) && KEY_NONE(KEY::DOWN) && KEY_NONE(KEY::LEFT) && KEY_NONE(KEY::RIGHT))

#define KEY_JUMP KEY::C
#define KEY_ATK KEY::X
#define KEY_GUARD KEY::Z

#define LAYER_DEFAULT                   0
#define LAYER_MANAGER                   1
#define LAYER_STATIC                    2
#define LAYER_DYNAMIC                   3
#define LAYER_PLAYER                    4
#define LAYER_PLAYER_TRIGGER            5
#define LAYER_PLAYERATK                6
#define LAYER_PLAYERATK_TRIGGER        7
#define LAYER_MONSTER                   8
#define LAYER_MONSTER_TRIGGER           9
#define LAYER_MONSTERATK               10
#define LAYER_MONSTERATK_TRIGGER       11
#define LAYER_EFFECT                    14
#define LAYER_UI                        15

#define PLAY_CURSTATE(STATE)                                                                                                                         \
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();                                                                                                \
    if (KirbyFSM->GetCurObject())                                                                                                                    \
    {                                                                                                                                                \
        KirbyFSM->GetCurObject()->STATE();                                                                                                           \
    }                                                                                                                                                \
    else                                                                                                                                             \
    {                                                                                                                                                \
        KirbyFSM->GetCurAbility()->STATE();                                                                                                          \
    }

#define ANIMPREFIX(NAME) L"Armature|" L##NAME

enum class AbilityCopyType
{
    NONE,
    NORMAL,
    FIRE,
    CUTTER,
    SWORD,
    SLIP,

    END,
};

enum class ObjectCopyType
{
    NONE,
    CONE,
    VENDING_MACHINE,
    LIGHT,

    END,
};