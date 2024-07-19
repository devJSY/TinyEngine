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
#include "CBladeKnightScript.h"
#include "CDetectAreaScript.h"
#include "CBladeKnightSwordScript.h"
#include "CNoddyScript.h"
#include "CHotHeadScript.h"
#include "CKabuScript.h"
#include "CSirKibbleScript.h"
#include "CGhostGordoScript.h"
#include "CGordoLongScript.h"
#include "CNormalEnemyScript.h"
#include "CTackleEnemyScript.h"
#include "CCutterProjectileScript.h"
#include "CFireProjectileScript.h"
#include "CAttackAreaScript.h"
#include "CDeformObjectScript.h"
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
	_vec.push_back(L"CBladeKnightScript");
	_vec.push_back(L"CDetectAreaScript");
	_vec.push_back(L"CBladeKnightSwordScript");
	_vec.push_back(L"CNoddyScript");
	_vec.push_back(L"CHotHeadScript");
	_vec.push_back(L"CKabuScript");
	_vec.push_back(L"CSirKibbleScript");
	_vec.push_back(L"CGhostGordoScript");
	_vec.push_back(L"CGordoLongScript");
	_vec.push_back(L"CNormalEnemyScript");
	_vec.push_back(L"CTackleEnemyScript");
	_vec.push_back(L"CCutterProjectileScript");
	_vec.push_back(L"CFireProjectileScript");
	_vec.push_back(L"CAttackAreaScript");
	_vec.push_back(L"CDeformObjectScript");
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
	if (L"CBladeKnightScript" == _strScriptName)
		return new CBladeKnightScript;
	if (L"CDetectAreaScript" == _strScriptName)
		return new CDetectAreaScript;
	if (L"CBladeKnightSwordScript" == _strScriptName)
		return new CBladeKnightSwordScript;
	if (L"CNoddyScript" == _strScriptName)
		return new CNoddyScript;
	if (L"CHotHeadScript" == _strScriptName)
		return new CHotHeadScript;
	if (L"CKabuScript" == _strScriptName)
		return new CKabuScript;
	if (L"CSirKibbleScript" == _strScriptName)
		return new CSirKibbleScript;
	if (L"CGhostGordoScript" == _strScriptName)
		return new CGhostGordoScript;
	if (L"CGordoLongScript" == _strScriptName)
		return new CGordoLongScript;
	if (L"CNormalEnemyScript" == _strScriptName)
		return new CNormalEnemyScript;
	if (L"CTackleEnemyScript" == _strScriptName)
		return new CTackleEnemyScript;
	if (L"CCutterProjectileScript" == _strScriptName)
		return new CCutterProjectileScript;
	if (L"CFireProjectileScript" == _strScriptName)
		return new CFireProjectileScript;
	if (L"CAttackAreaScript" == _strScriptName)
		return new CAttackAreaScript;
	if (L"CDeformObjectScript" == _strScriptName)
		return new CDeformObjectScript;
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
	case (UINT)SCRIPT_TYPE::BLADEKNIGHTSCRIPT:
		return new CBladeKnightScript;
	case (UINT)SCRIPT_TYPE::DETECTAREASCRIPT:
		return new CDetectAreaScript;
	case (UINT)SCRIPT_TYPE::BLADEKNIGHTSWORDSCRIPT:
		return new CBladeKnightSwordScript;
	case (UINT)SCRIPT_TYPE::NODDYSCRIPT:
		return new CNoddyScript;
	case (UINT)SCRIPT_TYPE::HOTHEADSCRIPT:
		return new CHotHeadScript;
	case (UINT)SCRIPT_TYPE::KABUSCRIPT:
		return new CKabuScript;
	case (UINT)SCRIPT_TYPE::SIRKIBBLESCRIPT:
		return new CSirKibbleScript;
	case (UINT)SCRIPT_TYPE::GHOSTGORDOSCRIPT:
		return new CGhostGordoScript;
	case (UINT)SCRIPT_TYPE::GORDOLONGSCRIPT:
		return new CGordoLongScript;
	case (UINT)SCRIPT_TYPE::NORMALENEMYSCRIPT:
		return new CNormalEnemyScript;
	case (UINT)SCRIPT_TYPE::TACKLEENEMYSCRIPT:
		return new CTackleEnemyScript;
	case (UINT)SCRIPT_TYPE::CUTTERPROJECTILESCRIPT:
		return new CCutterProjectileScript;
	case (UINT)SCRIPT_TYPE::FIREPROJECTILESCRIPT:
		return new CFireProjectileScript;
	case (UINT)SCRIPT_TYPE::ATTACKAREASCRIPT:
		return new CAttackAreaScript;
	case (UINT)SCRIPT_TYPE::DEFORMOBJECTSCRIPT:
		return new CDeformObjectScript;
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
	case SCRIPT_TYPE::BLADEKNIGHTSCRIPT:
		return L"CBladeKnightScript";
	case SCRIPT_TYPE::DETECTAREASCRIPT:
		return L"CDetectAreaScript";
	case SCRIPT_TYPE::BLADEKNIGHTSWORDSCRIPT:
		return L"CBladeKnightSwordScript";
	case SCRIPT_TYPE::NODDYSCRIPT:
		return L"CNoddyScript";
	case SCRIPT_TYPE::HOTHEADSCRIPT:
		return L"CHotHeadScript";
	case SCRIPT_TYPE::KABUSCRIPT:
		return L"CKabuScript";
	case SCRIPT_TYPE::SIRKIBBLESCRIPT:
		return L"CSirKibbleScript";
	case SCRIPT_TYPE::GHOSTGORDOSCRIPT:
		return L"CGhostGordoScript";
	case SCRIPT_TYPE::GORDOLONGSCRIPT:
		return L"CGordoLongScript";
	case SCRIPT_TYPE::NORMALENEMYSCRIPT:
		return L"CNormalEnemyScript";
	case SCRIPT_TYPE::TACKLEENEMYSCRIPT:
		return L"CTackleEnemyScript";
	case SCRIPT_TYPE::CUTTERPROJECTILESCRIPT:
		return L"CCutterProjectileScript";
	case SCRIPT_TYPE::FIREPROJECTILESCRIPT:
		return L"CFireProjectileScript";
	case SCRIPT_TYPE::ATTACKAREASCRIPT:
		return L"CAttackAreaScript";
	case SCRIPT_TYPE::DEFORMOBJECTSCRIPT:
		return L"CDeformObjectScript";
	case SCRIPT_TYPE::KIRBYHATBLADE:
		return L"CKirbyHatBlade";
	case SCRIPT_TYPE::KIRBYCUTTERBULLET:
		return L"CKirbyCutterBullet";
	}
	return nullptr;
}