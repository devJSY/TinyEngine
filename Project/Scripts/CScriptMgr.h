#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVESCRIPT,
	ELEVATORSCRIPT,
	ENEMYHITBOXSCRIPT,
	FLYINGBOOKSCRIPT,
	GAMEMANAGERSCRIPT,
	GHOSTSCRIPT,
	LEVELCHANGESCRIPT,
	PLAYBUTTONSCRIPT,
	PLAYERCAMERASCRIPT,
	PLAYERDETECTSCRIPT,
	PLAYERHITBOXSCRIPT,
	PLAYERLIFEBARSCRIPT,
	PLAYERMANABARSCRIPT,
	PLAYERSCRIPT,
	PROJECTILE2DSCRIPT,
	PYROGHOSTSCRIPT,
	SPOOKSMENSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript* GetScript(const wstring& _strScriptName);
	static CScript* GetScript(UINT _iScriptType);
	static const wchar_t* GetScriptName(CScript* _pScript);
};
