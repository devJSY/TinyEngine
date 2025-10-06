#include "pch.h"
#include "CScriptMgr.h"

#include "CModelEditorCameraMoveScript.h"
#include "CCameraMoveScript.h"
#include "CEngineTestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CModelEditorCameraMoveScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CEngineTestScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CModelEditorCameraMoveScript" == _strScriptName)
		return new CModelEditorCameraMoveScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CEngineTestScript" == _strScriptName)
		return new CEngineTestScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return new CModelEditorCameraMoveScript;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::ENGINETESTSCRIPT:
		return new CEngineTestScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return L"CModelEditorCameraMoveScript";
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::ENGINETESTSCRIPT:
		return L"CEngineTestScript";
	}
	return nullptr;
}