#pragma once

enum class SCRIPT_TYPE
{
    CAMERAMOVESCRIPT,
    PLAYERSCRIPT,
    END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
    static void GetScriptInfo(vector<wstring>& _vec);
    static CScript* GetScript(const wstring& _strScriptName);
    static CScript* GetScript(SCRIPT_TYPE _iScriptType);
    static const wchar_t* GetScriptName(CScript* _pScript);
};
