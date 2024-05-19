#include "pch.h"
#include "CScriptMgr.h"

#include "CBulletCountUIScript.h"
#include "CBulletScript.h"
#include "CCameraMoveScript.h"
#include "CGoalScript.h"
#include "CPlayerCameraScript.h"
#include "CPlayerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBulletCountUIScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CGoalScript");
	_vec.push_back(L"CPlayerCameraScript");
	_vec.push_back(L"CPlayerScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBulletCountUIScript" == _strScriptName)
		return new CBulletCountUIScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CGoalScript" == _strScriptName)
		return new CGoalScript;
	if (L"CPlayerCameraScript" == _strScriptName)
		return new CPlayerCameraScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BULLETCOUNTUISCRIPT:
		return new CBulletCountUIScript;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::GOALSCRIPT:
		return new CGoalScript;
	case (UINT)SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return new CPlayerCameraScript;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BULLETCOUNTUISCRIPT:
		return L"CBulletCountUIScript";
	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::GOALSCRIPT:
		return L"CGoalScript";
	case SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return L"CPlayerCameraScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
	}
	return nullptr;
}