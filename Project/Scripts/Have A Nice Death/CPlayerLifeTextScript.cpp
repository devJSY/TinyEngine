#include "pch.h"
#include "CPlayerLifeTextScript.h"
#include "CPlayerScript.h"

CPlayerLifeTextScript::CPlayerLifeTextScript()
    : CScript(PLAYERLIFETEXTSCRIPT)
{
}

CPlayerLifeTextScript::~CPlayerLifeTextScript()
{
}

void CPlayerLifeTextScript::begin()
{
    if (nullptr == TextRender())
    {
        GetOwner()->AddComponent(new CTextRender);
    }
}

void CPlayerLifeTextScript::tick()
{
    CPlayerScript* pPlyaerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    int CurLife = pPlyaerScript->GetCurLife();
    int MaxLife = pPlyaerScript->GetMaxLife();

    wstring StrText = wstring();
    StrText += std::to_wstring(CurLife);
    StrText += L" / ";
    StrText += std::to_wstring(MaxLife);

    TextRender()->SetText(StrText);
}
