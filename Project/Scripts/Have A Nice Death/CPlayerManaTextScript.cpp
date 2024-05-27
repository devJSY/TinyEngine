#include "pch.h"
#include "CPlayerManaTextScript.h"
#include "CPlayerScript.h"

CPlayerManaTextScript::CPlayerManaTextScript()
    : CScript(PLAYERMANATEXTSCRIPT)
{
}

CPlayerManaTextScript::~CPlayerManaTextScript()
{
}

void CPlayerManaTextScript::begin()
{
    if (nullptr == TextRender())
    {
        GetOwner()->AddComponent(new CTextRender);
    }
}

void CPlayerManaTextScript::tick()
{
    CPlayerScript* pPlyaerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr == pPlyaerScript)
        return;

    int CurMana = pPlyaerScript->GetCurMana();
    int MaxMana = pPlyaerScript->GetMaxMana();

    wstring StrText = wstring();
    StrText += std::to_wstring(CurMana);
    StrText += L" / ";
    StrText += std::to_wstring(MaxMana);

    TextRender()->SetText(StrText);
}
