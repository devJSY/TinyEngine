#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CPlayerScript.h"
#include "CShockWaveScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
    _vec.push_back(L"CCameraMoveScript");
    _vec.push_back(L"CPlayerScript");
    _vec.push_back(L"CShockWaveScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
    if (L"CCameraMoveScript" == _strScriptName)
        return new CCameraMoveScript;
    if (L"CPlayerScript" == _strScriptName)
        return new CPlayerScript;
    if (L"CShockWaveScript" == _strScriptName)
        return new CShockWaveScript;
    return nullptr;
}

CScript* CScriptMgr::GetScript(SCRIPT_TYPE _iScriptType)
{
    switch (_iScriptType)
    {
    case SCRIPT_TYPE::CAMERAMOVESCRIPT:
        return new CCameraMoveScript;
        break;
    case SCRIPT_TYPE::PLAYERSCRIPT:
        return new CPlayerScript;
    case SCRIPT_TYPE::SHOCKWAVESCRIPT:
        return new CShockWaveScript;
        break;
    }
    return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
    switch (_pScript->GetScriptType())
    {
    case SCRIPT_TYPE::CAMERAMOVESCRIPT:
        return L"CCameraMoveScript";
        break;
    case SCRIPT_TYPE::PLAYERSCRIPT:
        return L"CPlayerScript";
    case SCRIPT_TYPE::SHOCKWAVESCRIPT:
        return L"CShockWaveScript";
        break;
    }
    return nullptr;
}