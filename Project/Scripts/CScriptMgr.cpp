#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CTestFSM.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CAnimationTestScript.h"
#include "CModelEditorCameraMoveScript.h"
#include "CSpringArmCameraScript.h"
#include "CButtonManagerScript.h"
#include "CUIManagerScript.h"
#include "CButtonScript.h"
#include "CUITexManagerScript.h"
#include "CUITexScript.h"
#include "CUIAnimManagerScript.h"
#include "CLevelChangeButtonScript.h"
#include "CStageChangeButtonScript.h"
#include "CExitButton.h"
#include "CUIHPScript.h"
#include "CUIUnitManagerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CTestFSM");
	_vec.push_back(L"CKirbyFSM");
	_vec.push_back(L"CKirbyMoveController");
	_vec.push_back(L"CAnimationTestScript");
	_vec.push_back(L"CModelEditorCameraMoveScript");
	_vec.push_back(L"CSpringArmCameraScript");
	_vec.push_back(L"CButtonManagerScript");
	_vec.push_back(L"CUIManagerScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CUITexManagerScript");
	_vec.push_back(L"CUITexScript");
	_vec.push_back(L"CUIAnimManagerScript");
	_vec.push_back(L"CLevelChangeButtonScript");
	_vec.push_back(L"CStageChangeButtonScript");
	_vec.push_back(L"CExitButton");
	_vec.push_back(L"CUIHPScript");
	_vec.push_back(L"CUIUnitManagerScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CTestFSM" == _strScriptName)
		return new CTestFSM;
	if (L"CKirbyFSM" == _strScriptName)
		return new CKirbyFSM;
	if (L"CKirbyMoveController" == _strScriptName)
		return new CKirbyMoveController;
	if (L"CAnimationTestScript" == _strScriptName)
		return new CAnimationTestScript;
	if (L"CModelEditorCameraMoveScript" == _strScriptName)
		return new CModelEditorCameraMoveScript;
	if (L"CSpringArmCameraScript" == _strScriptName)
		return new CSpringArmCameraScript;
	if (L"CButtonManagerScript" == _strScriptName)
		return new CButtonManagerScript;
	if (L"CUIManagerScript" == _strScriptName)
		return new CUIManagerScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CUITexManagerScript" == _strScriptName)
		return new CUITexManagerScript;
	if (L"CUITexScript" == _strScriptName)
		return new CUITexScript;
	if (L"CUIAnimManagerScript" == _strScriptName)
		return new CUIAnimManagerScript;
	if (L"CLevelChangeButtonScript" == _strScriptName)
		return new CLevelChangeButtonScript;
	if (L"CStageChangeButtonScript" == _strScriptName)
		return new CStageChangeButtonScript;
	if (L"CExitButton" == _strScriptName)
		return new CExitButton;
	if (L"CUIHPScript" == _strScriptName)
		return new CUIHPScript;
	if (L"CUIUnitManagerScript" == _strScriptName)
		return new CUIUnitManagerScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::TESTFSM:
		return new CTestFSM;
	case (UINT)SCRIPT_TYPE::KIRBYFSM:
		return new CKirbyFSM;
	case (UINT)SCRIPT_TYPE::KIRBYMOVECONTROLLER:
		return new CKirbyMoveController;
	case (UINT)SCRIPT_TYPE::ANIMATIONTESTSCRIPT:
		return new CAnimationTestScript;
	case (UINT)SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return new CModelEditorCameraMoveScript;
	case (UINT)SCRIPT_TYPE::SPRINGARMCAMERASCRIPT:
		return new CSpringArmCameraScript;
	case (UINT)SCRIPT_TYPE::BUTTONMANAGERSCRIPT:
		return new CButtonManagerScript;
	case (UINT)SCRIPT_TYPE::UIMANAGERSCRIPT:
		return new CUIManagerScript;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
	case (UINT)SCRIPT_TYPE::UITEXMANAGERSCRIPT:
		return new CUITexManagerScript;
	case (UINT)SCRIPT_TYPE::UITEXSCRIPT:
		return new CUITexScript;
	case (UINT)SCRIPT_TYPE::UIANIMMANAGERSCRIPT:
		return new CUIAnimManagerScript;
	case (UINT)SCRIPT_TYPE::LEVELCHANGEBUTTONSCRIPT:
		return new CLevelChangeButtonScript;
	case (UINT)SCRIPT_TYPE::STAGECHANGEBUTTONSCRIPT:
		return new CStageChangeButtonScript;
	case (UINT)SCRIPT_TYPE::EXITBUTTON:
		return new CExitButton;
	case (UINT)SCRIPT_TYPE::UIHPSCRIPT:
		return new CUIHPScript;
	case (UINT)SCRIPT_TYPE::UIUNITMANAGERSCRIPT:
		return new CUIUnitManagerScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::TESTFSM:
		return L"CTestFSM";
	case SCRIPT_TYPE::KIRBYFSM:
		return L"CKirbyFSM";
	case SCRIPT_TYPE::KIRBYMOVECONTROLLER:
		return L"CKirbyMoveController";
	case SCRIPT_TYPE::ANIMATIONTESTSCRIPT:
		return L"CAnimationTestScript";
	case SCRIPT_TYPE::MODELEDITORCAMERAMOVESCRIPT:
		return L"CModelEditorCameraMoveScript";
	case SCRIPT_TYPE::SPRINGARMCAMERASCRIPT:
		return L"CSpringArmCameraScript";
	case SCRIPT_TYPE::BUTTONMANAGERSCRIPT:
		return L"CButtonManagerScript";
	case SCRIPT_TYPE::UIMANAGERSCRIPT:
		return L"CUIManagerScript";
	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
	case SCRIPT_TYPE::UITEXMANAGERSCRIPT:
		return L"CUITexManagerScript";
	case SCRIPT_TYPE::UITEXSCRIPT:
		return L"CUITexScript";
	case SCRIPT_TYPE::UIANIMMANAGERSCRIPT:
		return L"CUIAnimManagerScript";
	case SCRIPT_TYPE::LEVELCHANGEBUTTONSCRIPT:
		return L"CLevelChangeButtonScript";
	case SCRIPT_TYPE::STAGECHANGEBUTTONSCRIPT:
		return L"CStageChangeButtonScript";
	case SCRIPT_TYPE::EXITBUTTON:
		return L"CExitButton";
	case SCRIPT_TYPE::UIHPSCRIPT:
		return L"CUIHPScript";
	case SCRIPT_TYPE::UIUNITMANAGERSCRIPT:
		return L"CUIUnitManagerScript";
	}
	return nullptr;
}