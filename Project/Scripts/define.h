#pragma once

#define PLAYER CPlayerMgr::GetPlayer()
#define PLAYERFSM CPlayerMgr::GetPlayerFSM()

#define KEY_TAP_ARROW (KEY_TAP(KEY::UP) || KEY_TAP(KEY::DOWN) || KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT))
#define KEY_PRESSED_ARROW (KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
#define KEY_RELEASED_ARROW (KEY_RELEASED(KEY::UP) || KEY_RELEASED(KEY::DOWN) || KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
#define KEY_NONE_ARROW (KEY_NONE(KEY::UP) && KEY_NONE(KEY::DOWN) && KEY_NONE(KEY::LEFT) && KEY_NONE(KEY::RIGHT))

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

#define KIRBYANIM(NAME) ((wstring)L"Armature|" + NAME)

enum class DIRECTION_TYPE
{
    LEFT,
    RIGHT,
};
