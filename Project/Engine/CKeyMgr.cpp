#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CDevice.h"
#include "CTaskMgr.h"
#include "CEditorMgr.h"

int g_KeySync[KEY::KEY_END] = {
    '0',        '1',        '2',        '3',         '4',        '5',        '6',        '7',        '8',        '9',

    VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3,  VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,

    VK_F1,      VK_F2,      VK_F3,      VK_F4,       VK_F5,      VK_F6,      VK_F7,      VK_F8,      VK_F9,      VK_F10,     VK_F11, VK_F12,

    'Q',        'W',        'E',        'R',         'T',        'Y',        'U',        'I',        'O',        'P',

    'A',        'S',        'D',        'F',         'G',        'H',        'J',        'K',        'L',

    'Z',        'X',        'C',        'V',         'B',        'N',        'M',

    VK_TAB,     VK_LSHIFT,  VK_LMENU,   VK_LCONTROL, VK_SPACE,

    VK_ESCAPE,  VK_RETURN,  VK_BACK,

    VK_LEFT,    VK_RIGHT,   VK_UP,      VK_DOWN,

    VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_SNAPSHOT, VK_DELETE,
};

CKeyMgr::CKeyMgr()
    : m_vMousePos{}
    , m_vMousePrevPos{}
    , m_vMouseDrag{}
    , m_MouseWheel(0)
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
    // Capaticy 를 미리 확보
    m_vecKeyData.reserve(KEY::KEY_END);

    for (UINT i = 0; i < KEY_END; ++i)
    {
        m_vecKeyData.push_back(FKeyData{(KEY)i, NONE, false});
    }
}

void CKeyMgr::tick()
{
    if (nullptr == GetFocus())
    {
        for (size_t i = 0; i < m_vecKeyData.size(); ++i)
        {
            if (TAP == m_vecKeyData[i].eState)
            {
                m_vecKeyData[i].eState = PRESSED;
            }
            else if (PRESSED == m_vecKeyData[i].eState)
            {
                m_vecKeyData[i].eState = RELEASED;
            }
            else if (RELEASED == m_vecKeyData[i].eState)
            {
                m_vecKeyData[i].eState = NONE;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < m_vecKeyData.size(); ++i)
        {
            if (GetAsyncKeyState(g_KeySync[m_vecKeyData[i].eKey]) & 0x8001)
            {
                // 이번 프레임에 해당 키가 눌려있다.
                if (m_vecKeyData[i].bPressed)
                {
                    // 이전에도 눌려있었다 ==> 계속 눌림 상태
                    m_vecKeyData[i].eState = PRESSED;
                }
                else
                {
                    // 이전에는 눌려있지 않았다 ==> 막 눌림 상태
                    m_vecKeyData[i].eState = TAP;
                }

                m_vecKeyData[i].bPressed = true;
            }
            else
            {
                // 이번 프레임에 해당키는 안 눌려 있다
                if (m_vecKeyData[i].bPressed)
                {
                    // 이전 프레임에는 눌려 있었다 ==> 막 뗌
                    m_vecKeyData[i].eState = RELEASED;
                }
                else
                {
                    m_vecKeyData[i].eState = NONE;
                }

                m_vecKeyData[i].bPressed = false;
            }
        }

        // 마우스 좌표 계산
        m_vMousePrevPos = m_vMousePos;

        POINT pt = {};
        GetCursorPos(&pt);
        ScreenToClient(CEngine::GetInst()->GetMainWind(), &pt);
        m_vMousePos = Vec2((float)pt.x, (float)pt.y);

        // 마우스 이동 방향
        m_vMouseDrag = m_vMousePos - m_vMousePrevPos;

        // 에디터 모드인 경우 Wheel 상태를 ImGui로 부터 입력받음
        if (CEditorMgr::GetInst()->IsEnabled())
        {
            float wheel = ImGui::GetIO().MouseWheel;
            if (wheel > 0)
                m_MouseWheel = 120;
            else if (wheel < 0)
                m_MouseWheel = -120;
        }
    }
}

void CKeyMgr::clear()
{
    SetMouseWheel(0);
}
