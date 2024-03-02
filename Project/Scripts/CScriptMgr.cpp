#include "pch.h"
#include "CScriptMgr.h"

#include "CBulletScript.h"
#include "CCameraMoveScript.h"
#include "CPlayerScript.h"
#include "CShockWaveScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CShockWaveScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CShockWaveScript" == _strScriptName)
		return new CShockWaveScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
	case (UINT)SCRIPT_TYPE::SHOCKWAVESCRIPT:
		return new CShockWaveScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
	case SCRIPT_TYPE::SHOCKWAVESCRIPT:
		return L"CShockWaveScript";
	}
	return nullptr;
}