#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CAnimationTestScript.h"
#include "CModelEditorCameraMoveScript.h"
#include "CPlayerMgr.h"
#include "CKirbyBulletScript.h"
#include "CTestFSM.h"
#include "CKirbyUnitScript.h"
#include "CSpringArmCameraScript.h"
#include "CButtonScript.h"
#include "CUIManagerScript.h"
#include "CButtonManagerScript.h"
#include "CKirbyVacuumCollider.h"
#include "CMonsterUnitScript.h"
#include "CKirbyCopyAbilityScript.h"
#include "CKirbyCopyObjScript.h"
#include "CKirbyBodyCollider.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CKirbyFSM");
	_vec.push_back(L"CKirbyMoveController");
	_vec.push_back(L"CAnimationTestScript");
	_vec.push_back(L"CModelEditorCameraMoveScript");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CKirbyBulletScript");
	_vec.push_back(L"CTestFSM");
	_vec.push_back(L"CKirbyUnitScript");
	_vec.push_back(L"CSpringArmCameraScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CUIManagerScript");
	_vec.push_back(L"CButtonManagerScript");
	_vec.push_back(L"CKirbyVacuumCollider");
	_vec.push_back(L"CMonsterUnitScript");
	_vec.push_back(L"CKirbyCopyAbilityScript");
	_vec.push_back(L"CKirbyCopyObjScript");
	_vec.push_back(L"CKirbyBodyCollider");
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
	if (L"CModelEditorCameraMoveScript" == _strScriptName)
		return new CModelEditorCameraMoveScript;
	if (L"CPlayerMgr" == _strScriptName)
		return new CPlayerMgr;
	if (L"CKirbyBulletScript" == _strScriptName)
		return new CKirbyBulletScript;
	if (L"CTestFSM" == _strScriptName)
		return new CTestFSM;
	if (L"CKirbyUnitScript" == _strScriptName)
		return new CKirbyUnitScript;
	if (L"CSpringArmCameraScript" == _strScriptName)
		return new CSpringArmCameraScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CUIManagerScript" == _strScriptName)
		return new CUIManagerScript;
	if (L"CButtonManagerScript" == _strScriptName)
		return new CButtonManagerScript;
	if (L"CKirbyVacuumCollider" == _strScriptName)
		return new CKirbyVacuumCollider;
	if (L"CMonsterUnitScript" == _strScriptName)
		return new CMonsterUnitScript;
	if (L"CKirbyCopyAbilityScript" == _strScriptName)
		return new CKirbyCopyAbilityScript;
	if (L"CKirbyCopyObjScript" == _strScriptName)
		return new CKirbyCopyObjScript;
	if (L"CKirbyBodyCollider" == _strScriptName)
		return new CKirbyBodyCollider;
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
	case (UINT)SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return new CModelEditorCameraMoveScript;
	case (UINT)SCRIPT_TYPE::PLAYERMGR:
		return new CPlayerMgr;
	case (UINT)SCRIPT_TYPE::KIRBYBULLETSCRIPT:
		return new CKirbyBulletScript;
	case (UINT)SCRIPT_TYPE::TESTFSM:
		return new CTestFSM;
	case (UINT)SCRIPT_TYPE::KIRBYUNITSCRIPT:
		return new CKirbyUnitScript;
	case (UINT)SCRIPT_TYPE::SPRINGARMCAMERASCRIPT:
		return new CSpringArmCameraScript;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
	case (UINT)SCRIPT_TYPE::UIMANAGERSCRIPT:
		return new CUIManagerScript;
	case (UINT)SCRIPT_TYPE::BUTTONMANAGERSCRIPT:
		return new CButtonManagerScript;
	case (UINT)SCRIPT_TYPE::UIMANAGERSCRIPT:
		return new CUIManagerScript;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
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
	case SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return L"CModelEditorCameraMoveScript";
	case SCRIPT_TYPE::PLAYERMGR:
		return L"CPlayerMgr";
	case SCRIPT_TYPE::KIRBYBULLETSCRIPT:
		return L"CKirbyBulletScript";
	case SCRIPT_TYPE::TESTFSM:
		return L"CTestFSM";
	case SCRIPT_TYPE::KIRBYUNITSCRIPT:
		return L"CKirbyUnitScript";
	case SCRIPT_TYPE::SPRINGARMCAMERASCRIPT:
		return L"CSpringArmCameraScript";
	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
	case SCRIPT_TYPE::UIMANAGERSCRIPT:
		return L"CUIManagerScript";
	case SCRIPT_TYPE::BUTTONMANAGERSCRIPT:
		return L"CButtonManagerScript";
	case SCRIPT_TYPE::UIMANAGERSCRIPT:
		return L"CUIManagerScript";
	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
	}
	return nullptr;
}