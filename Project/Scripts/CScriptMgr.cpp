#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CAnimationTestScript.h"
#include "CPlayerMgr.h"
#include "CKirbyBulletScript.h"
#include "CTestFSM.h"
#include "CKirbyScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CKirbyFSM");
	_vec.push_back(L"CKirbyMoveController");
	_vec.push_back(L"CAnimationTestScript");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CKirbyBulletScript");
	_vec.push_back(L"CTestFSM");
	_vec.push_back(L"CKirbyScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CKirbyFSM" == _strScriptName)
		return new CKirbyFSM;
	if (L"CKirbyMoveController" == _strScriptName)
		return new CKirbyMoveController;
	if (L"CAnimationTestScript" == _strScriptName)
		return new CAnimationTestScript;
	if (L"CPlayerMgr" == _strScriptName)
		return new CPlayerMgr;
	if (L"CKirbyBulletScript" == _strScriptName)
		return new CKirbyBulletScript;
	if (L"CTestFSM" == _strScriptName)
		return new CTestFSM;
	if (L"CKirbyScript" == _strScriptName)
		return new CKirbyScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::KIRBYFSM:
		return new CKirbyFSM;
	case (UINT)SCRIPT_TYPE::KIRBYMOVECONTROLLER:
		return new CKirbyMoveController;
	case (UINT)SCRIPT_TYPE::ANIMATIONTESTSCRIPT:
		return new CAnimationTestScript;
	case (UINT)SCRIPT_TYPE::PLAYERMGR:
		return new CPlayerMgr;
	case (UINT)SCRIPT_TYPE::KIRBYBULLETSCRIPT:
		return new CKirbyBulletScript;
	case (UINT)SCRIPT_TYPE::TESTFSM:
		return new CTestFSM;
	case (UINT)SCRIPT_TYPE::KIRBYSCRIPT:
		return new CKirbyScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::KIRBYFSM:
		return L"CKirbyFSM";
	case SCRIPT_TYPE::KIRBYMOVECONTROLLER:
		return L"CKirbyMoveController";
	case SCRIPT_TYPE::ANIMATIONTESTSCRIPT:
		return L"CAnimationTestScript";
	case SCRIPT_TYPE::PLAYERMGR:
		return L"CPlayerMgr";
	case SCRIPT_TYPE::KIRBYBULLETSCRIPT:
		return L"CKirbyBulletScript";
	case SCRIPT_TYPE::TESTFSM:
		return L"CTestFSM";
	case SCRIPT_TYPE::KIRBYSCRIPT:
		return L"CKirbyScript";
	}
	return nullptr;
}