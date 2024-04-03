#include "pch.h"
#include "CGameManagerScript.h"

#include "CLevelChangeScript.h"

CGameManagerScript* CGameManagerScript::m_Inst = nullptr;

CGameManagerScript::CGameManagerScript()
    : CScript(GAMEMANAGERSCRIPT)
    , m_Player(nullptr)
    , m_ExitElevator(nullptr)
{
    CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");

    // 레벨에 플레이어가 존재하는 경우 삭제
    // 플레이어는 GameManager에서만 관리
    if (nullptr != pPlayer)
        GamePlayStatic::DestroyGameObject(pPlayer);

    // 프리팹으로 부터 플레이어 로딩
    Ptr<CPrefab> pPlayerPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Player.pref", L"prefab\\Player.pref");
    m_Player = pPlayerPref->Instantiate();
}

CGameManagerScript::~CGameManagerScript()
{
    // 레벨 변경시 현재 레벨에서 관리하던 오브젝트 해제
    Release();
}

CGameObject* CGameManagerScript::GetExitElevator()
{
    if (nullptr == m_ExitElevator)
    {
        m_ExitElevator = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"ExitElevator");
    }

    return m_ExitElevator;
}

void CGameManagerScript::ChangeLevel(const std::string& _LevelName)
{
    CLevelChangeScript* LevelChangeScript = GetOwner()->GetScript<CLevelChangeScript>();
    if (nullptr == LevelChangeScript)
    {
        LevelChangeScript = new CLevelChangeScript;
        GetOwner()->AddComponent(LevelChangeScript);
    }

    LevelChangeScript->ChangeLevel(_LevelName);
}

void CGameManagerScript::begin()
{
    // 게임매니저 오브젝트가 1개이상인 경우
    vector<CGameObject*> pGMObjs = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectsOfType<CGameManagerScript>();
    if (1 != pGMObjs.size())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
        return;
    }
}

void CGameManagerScript::tick()
{
}

CGameManagerScript* CGameManagerScript::GetInset()
{
    if (nullptr == m_Inst)
    {
        CGameObject* pGMObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectOfType<CGameManagerScript>();

        if (nullptr != pGMObj)
        {
            CGameManagerScript* ScriptInst = pGMObj->GetScript<CGameManagerScript>();

            // GM오브젝트는 존재하는데 스크립트가 존재하지않는 경우
            if (nullptr == ScriptInst)
            {
                ScriptInst = new CGameManagerScript;
            }

            m_Inst = ScriptInst;
        }
        else // GM오브젝트가 존재하지않는 경우
        {
            CGameObject* pNewGMObj = new CGameObject;
            pNewGMObj->SetName(L"GameManager");
            pNewGMObj->AddComponent(new CTransform);

            CGameManagerScript* ScriptInst = new CGameManagerScript;
            pNewGMObj->AddComponent(ScriptInst);
            m_Inst = ScriptInst;

            int GMLayerIdx = 0;
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

            for (int i = 0; i < LAYER_MAX; i++)
            {
                if (L"GameManager" == pCurLevel->GetLayer(i)->GetName())
                {
                    GMLayerIdx = i;
                    break;
                }
            }

            GamePlayStatic::SpawnGameObject(pNewGMObj, GMLayerIdx);
        }
    }

    return m_Inst;
}

void CGameManagerScript::Release()
{
    m_Inst = nullptr;

    // Player 가 레벨에 존재하고있지 않은 경우에만 GameManager에서 Player 메모리 해제
    CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
    if (nullptr == pObj)
    {
        if (nullptr != m_Player)
        {
            delete m_Player;
        }
    }

    m_Player = nullptr;
}
