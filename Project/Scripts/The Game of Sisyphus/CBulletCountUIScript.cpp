#include "pch.h"
#include "CBulletCountUIScript.h"

#include "CPlayerScript.h"

CBulletCountUIScript::CBulletCountUIScript()
    : CScript(BULLETCOUNTUISCRIPT)
{
}

CBulletCountUIScript::~CBulletCountUIScript()
{
}

void CBulletCountUIScript::begin()
{
    m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
}

void CBulletCountUIScript::tick()
{
    if (nullptr == m_Player || nullptr == TextRender())
        return;

    CPlayerScript* pPlayerScript = m_Player->GetScript<CPlayerScript>();

    if (nullptr != pPlayerScript)
    {
        wstring TextStr = L"Bullet Count : ";
        TextStr += std::to_wstring(pPlayerScript->GetBulletCount());
        TextRender()->SetText(TextStr);
    }
}
