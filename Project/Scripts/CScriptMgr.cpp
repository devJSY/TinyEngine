#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
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
#include "CUITexManagerScript.h"
#include "CUITexScript.h"
#include "CUIAnimManagerScript.h"
#include "CLevelChangeButtonScript.h"
#include "CStageChangeButtonScript.h"
#include "CExitButton.h"
#include "CUIHPScript.h"
#include "CUIUnitManagerScript.h"
#include "CEngineTestScript.h"
#include "CNormalEnemyFSM.h"
#include "CMonsterMgr.h"
#include "CMonsterMoveController.h"
#include "CPlayerTestScript.h"
#include "CMonsterMoveTest.h"
#include "CKirbyHatBlade.h"
#include "CKirbyCutterBullet.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CKirbyFSM");
	_vec.push_back(L"CKirbyMoveController");
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
	_vec.push_back(L"CUITexManagerScript");
	_vec.push_back(L"CUITexScript");
	_vec.push_back(L"CUIAnimManagerScript");
	_vec.push_back(L"CLevelChangeButtonScript");
	_vec.push_back(L"CStageChangeButtonScript");
	_vec.push_back(L"CExitButton");
	_vec.push_back(L"CUIHPScript");
	_vec.push_back(L"CUIUnitManagerScript");
	_vec.push_back(L"CEngineTestScript");
	_vec.push_back(L"CNormalEnemyFSM");
	_vec.push_back(L"CMonsterMgr");
	_vec.push_back(L"CMonsterMoveController");
	_vec.push_back(L"CPlayerTestScript");
	_vec.push_back(L"CMonsterMoveTest");
	_vec.push_back(L"CKirbyHatBlade");
	_vec.push_back(L"CKirbyCutterBullet");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CKirbyFSM" == _strScriptName)
		return new CKirbyFSM;
	if (L"CKirbyMoveController" == _strScriptName)
		return new CKirbyMoveController;
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
	if (L"CEngineTestScript" == _strScriptName)
		return new CEngineTestScript;
	if (L"CNormalEnemyFSM" == _strScriptName)
		return new CNormalEnemyFSM;
	if (L"CMonsterMgr" == _strScriptName)
		return new CMonsterMgr;
	if (L"CMonsterMoveController" == _strScriptName)
		return new CMonsterMoveController;
	if (L"CPlayerTestScript" == _strScriptName)
		return new CPlayerTestScript;
	if (L"CMonsterMoveTest" == _strScriptName)
		return new CMonsterMoveTest;
	if (L"CKirbyHatBlade" == _strScriptName)
		return new CKirbyHatBlade;
	if (L"CKirbyCutterBullet" == _strScriptName)
		return new CKirbyCutterBullet;
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
	case (UINT)SCRIPT_TYPE::KIRBYVACUUMCOLLIDER:
		return new CKirbyVacuumCollider;
	case (UINT)SCRIPT_TYPE::MONSTERUNITSCRIPT:
		return new CMonsterUnitScript;
	case (UINT)SCRIPT_TYPE::KIRBYCOPYABILITYSCRIPT:
		return new CKirbyCopyAbilityScript;
	case (UINT)SCRIPT_TYPE::KIRBYCOPYOBJSCRIPT:
		return new CKirbyCopyObjScript;
	case (UINT)SCRIPT_TYPE::KIRBYBODYCOLLIDER:
		return new CKirbyBodyCollider;
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
	case (UINT)SCRIPT_TYPE::ENGINETESTSCRIPT:
		return new CEngineTestScript;
	case (UINT)SCRIPT_TYPE::NORMALENEMYFSM:
		return new CNormalEnemyFSM;
	case (UINT)SCRIPT_TYPE::MONSTERMGR:
		return new CMonsterMgr;
	case (UINT)SCRIPT_TYPE::MONSTERMOVECONTROLLER:
		return new CMonsterMoveController;
	case (UINT)SCRIPT_TYPE::PLAYERTESTSCRIPT:
		return new CPlayerTestScript;
	case (UINT)SCRIPT_TYPE::MONSTERMOVETEST:
		return new CMonsterMoveTest;
	case (UINT)SCRIPT_TYPE::KIRBYHATBLADE:
		return new CKirbyHatBlade;
	case (UINT)SCRIPT_TYPE::KIRBYCUTTERBULLET:
		return new CKirbyCutterBullet;
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
	case SCRIPT_TYPE::KIRBYVACUUMCOLLIDER:
		return L"CKirbyVacuumCollider";
	case SCRIPT_TYPE::MONSTERUNITSCRIPT:
		return L"CMonsterUnitScript";
	case SCRIPT_TYPE::KIRBYCOPYABILITYSCRIPT:
		return L"CKirbyCopyAbilityScript";
	case SCRIPT_TYPE::KIRBYCOPYOBJSCRIPT:
		return L"CKirbyCopyObjScript";
	case SCRIPT_TYPE::KIRBYBODYCOLLIDER:
		return L"CKirbyBodyCollider";
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
	case SCRIPT_TYPE::ENGINETESTSCRIPT:
		return L"CEngineTestScript";
	case SCRIPT_TYPE::NORMALENEMYFSM:
		return L"CNormalEnemyFSM";
	case SCRIPT_TYPE::MONSTERMGR:
		return L"CMonsterMgr";
	case SCRIPT_TYPE::MONSTERMOVECONTROLLER:
		return L"CMonsterMoveController";
	case SCRIPT_TYPE::PLAYERTESTSCRIPT:
		return L"CPlayerTestScript";
	case SCRIPT_TYPE::MONSTERMOVETEST:
		return L"CMonsterMoveTest";
	case SCRIPT_TYPE::KIRBYHATBLADE:
		return L"CKirbyHatBlade";
	case SCRIPT_TYPE::KIRBYCUTTERBULLET:
		return L"CKirbyCutterBullet";
	}
	return nullptr;
}