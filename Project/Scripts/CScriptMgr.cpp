#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CModelEditorCameraMoveScript.h"
#include "CPlayerMgr.h"
#include "CKirbyBulletScript.h"
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
#include "CCanJuice.h"
#include "CMonsterTest.h"
#include "CMomentaryObjScript.h"
#include "CKirbyWeaponHitbox.h"
#include "CCameraController.h"
#include "CUIMoveUpDownScript.h"
#include "CUIAnimScript.h"
#include "CUIFlowScript.h"
#include "CUIStartSceneFlowScript.h"
#include "CUIGlowMoveScript.h"
#include "CChangeCamTrigger.h"
#include "CProgressCamTrigger.h"
#include "CTwoTargetCamTrigger.h"
#include "CElfilisFSM.h"
#include "CBossMgr.h"
#include "CElfilisArrowSetScript.h"
#include "CElfilisArrowScript.h"
#include "CElfilisSwordSlashScript.h"
#include "CElfilisStormScript.h"
#include "CElfilisAirArrow.h"
#include "CElfilisDimensionLaser.h"
#include "CElfilisLaser.h"
#include "CElfilisBigFSM.h"
#include "CChangeAlphaScript.h"
#include "CCrumbleScript.h"
#include "CSurpriseBoardScript.h"
#include "CSurpriseBoardAttackScript.h"
#include "CUIWipeScript.h"
#include "CWipeTriggerScript.h"
#include "CKirbyUnitScript.h"
#include "CSolarChargeScript.h"
#include "CSolarOnceScript.h"
#include "CElevatorScript.h"
#include "CPushOutScript.h"
#include "CPushOutTriggerScript.h"
#include "CPushOutColliderScript.h"
#include "CLadderScript.h"
#include "CKirbyCheckPointTrigger.h"
#include "CKirbyFallDetectTrigger.h"
#include "CFixedViewCamTrigger.h"
#include "CMorphoFSM.h"
#include "CMorphoUnit.h"
#include "CElfilisUnit.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CKirbyFSM");
	_vec.push_back(L"CKirbyMoveController");
	_vec.push_back(L"CModelEditorCameraMoveScript");
	_vec.push_back(L"CPlayerMgr");
	_vec.push_back(L"CKirbyBulletScript");
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
	_vec.push_back(L"CCanJuice");
	_vec.push_back(L"CMonsterTest");
	_vec.push_back(L"CMomentaryObjScript");
	_vec.push_back(L"CKirbyWeaponHitbox");
	_vec.push_back(L"CCameraController");
	_vec.push_back(L"CUIMoveUpDownScript");
	_vec.push_back(L"CUIAnimScript");
	_vec.push_back(L"CUIFlowScript");
	_vec.push_back(L"CUIStartSceneFlowScript");
	_vec.push_back(L"CUIGlowMoveScript");
	_vec.push_back(L"CChangeCamTrigger");
	_vec.push_back(L"CProgressCamTrigger");
	_vec.push_back(L"CTwoTargetCamTrigger");
	_vec.push_back(L"CElfilisFSM");
	_vec.push_back(L"CBossMgr");
	_vec.push_back(L"CElfilisArrowSetScript");
	_vec.push_back(L"CElfilisArrowScript");
	_vec.push_back(L"CElfilisSwordSlashScript");
	_vec.push_back(L"CElfilisStormScript");
	_vec.push_back(L"CElfilisAirArrow");
	_vec.push_back(L"CElfilisDimensionLaser");
	_vec.push_back(L"CElfilisLaser");
	_vec.push_back(L"CElfilisBigFSM");
	_vec.push_back(L"CChangeAlphaScript");
	_vec.push_back(L"CCrumbleScript");
	_vec.push_back(L"CSurpriseBoardScript");
	_vec.push_back(L"CSurpriseBoardAttackScript");
	_vec.push_back(L"CUIWipeScript");
	_vec.push_back(L"CWipeTriggerScript");
	_vec.push_back(L"CKirbyUnitScript");
	_vec.push_back(L"CSolarChargeScript");
	_vec.push_back(L"CSolarOnceScript");
	_vec.push_back(L"CElevatorScript");
	_vec.push_back(L"CPushOutScript");
	_vec.push_back(L"CPushOutTriggerScript");
	_vec.push_back(L"CPushOutColliderScript");
	_vec.push_back(L"CLadderScript");
	_vec.push_back(L"CKirbyCheckPointTrigger");
	_vec.push_back(L"CKirbyFallDetectTrigger");
	_vec.push_back(L"CFixedViewCamTrigger");
	_vec.push_back(L"CMorphoFSM");
	_vec.push_back(L"CMorphoUnit");
	_vec.push_back(L"CElfilisUnit");
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
	if (L"CCanJuice" == _strScriptName)
		return new CCanJuice;
	if (L"CMonsterTest" == _strScriptName)
		return new CMonsterTest;
	if (L"CMomentaryObjScript" == _strScriptName)
		return new CMomentaryObjScript;
	if (L"CKirbyWeaponHitbox" == _strScriptName)
		return new CKirbyWeaponHitbox;
	if (L"CCameraController" == _strScriptName)
		return new CCameraController;
	if (L"CUIMoveUpDownScript" == _strScriptName)
		return new CUIMoveUpDownScript;
	if (L"CUIAnimScript" == _strScriptName)
		return new CUIAnimScript;
	if (L"CUIFlowScript" == _strScriptName)
		return new CUIFlowScript;
	if (L"CUIStartSceneFlowScript" == _strScriptName)
		return new CUIStartSceneFlowScript;
	if (L"CUIGlowMoveScript" == _strScriptName)
		return new CUIGlowMoveScript;
	if (L"CChangeCamTrigger" == _strScriptName)
		return new CChangeCamTrigger;
	if (L"CProgressCamTrigger" == _strScriptName)
		return new CProgressCamTrigger;
	if (L"CTwoTargetCamTrigger" == _strScriptName)
		return new CTwoTargetCamTrigger;
	if (L"CElfilisFSM" == _strScriptName)
		return new CElfilisFSM;
	if (L"CBossMgr" == _strScriptName)
		return new CBossMgr;
	if (L"CElfilisArrowSetScript" == _strScriptName)
		return new CElfilisArrowSetScript;
	if (L"CElfilisArrowScript" == _strScriptName)
		return new CElfilisArrowScript;
	if (L"CElfilisSwordSlashScript" == _strScriptName)
		return new CElfilisSwordSlashScript;
	if (L"CElfilisStormScript" == _strScriptName)
		return new CElfilisStormScript;
	if (L"CElfilisAirArrow" == _strScriptName)
		return new CElfilisAirArrow;
	if (L"CElfilisDimensionLaser" == _strScriptName)
		return new CElfilisDimensionLaser;
	if (L"CElfilisLaser" == _strScriptName)
		return new CElfilisLaser;
	if (L"CElfilisBigFSM" == _strScriptName)
		return new CElfilisBigFSM;
	if (L"CChangeAlphaScript" == _strScriptName)
		return new CChangeAlphaScript;
	if (L"CCrumbleScript" == _strScriptName)
		return new CCrumbleScript;
	if (L"CSurpriseBoardScript" == _strScriptName)
		return new CSurpriseBoardScript;
	if (L"CSurpriseBoardAttackScript" == _strScriptName)
		return new CSurpriseBoardAttackScript;
	if (L"CUIWipeScript" == _strScriptName)
		return new CUIWipeScript;
	if (L"CWipeTriggerScript" == _strScriptName)
		return new CWipeTriggerScript;
	if (L"CKirbyUnitScript" == _strScriptName)
		return new CKirbyUnitScript;
	if (L"CSolarChargeScript" == _strScriptName)
		return new CSolarChargeScript;
	if (L"CSolarOnceScript" == _strScriptName)
		return new CSolarOnceScript;
	if (L"CElevatorScript" == _strScriptName)
		return new CElevatorScript;
	if (L"CPushOutScript" == _strScriptName)
		return new CPushOutScript;
	if (L"CPushOutTriggerScript" == _strScriptName)
		return new CPushOutTriggerScript;
	if (L"CPushOutColliderScript" == _strScriptName)
		return new CPushOutColliderScript;
	if (L"CLadderScript" == _strScriptName)
		return new CLadderScript;
	if (L"CKirbyCheckPointTrigger" == _strScriptName)
		return new CKirbyCheckPointTrigger;
	if (L"CKirbyFallDetectTrigger" == _strScriptName)
		return new CKirbyFallDetectTrigger;
	if (L"CFixedViewCamTrigger" == _strScriptName)
		return new CFixedViewCamTrigger;
	if (L"CMorphoFSM" == _strScriptName)
		return new CMorphoFSM;
	if (L"CMorphoUnit" == _strScriptName)
		return new CMorphoUnit;
	if (L"CElfilisUnit" == _strScriptName)
		return new CElfilisUnit;
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
	case (UINT)SCRIPT_TYPE::CANJUICE:
		return new CCanJuice;
	case (UINT)SCRIPT_TYPE::MONSTERTEST:
		return new CMonsterTest;
	case (UINT)SCRIPT_TYPE::MOMENTARYOBJSCRIPT:
		return new CMomentaryObjScript;
	case (UINT)SCRIPT_TYPE::KIRBYWEAPONHITBOX:
		return new CKirbyWeaponHitbox;
	case (UINT)SCRIPT_TYPE::CAMERACONTROLLER:
		return new CCameraController;
	case (UINT)SCRIPT_TYPE::UIMOVEUPDOWNSCRIPT:
		return new CUIMoveUpDownScript;
	case (UINT)SCRIPT_TYPE::UIANIMSCRIPT:
		return new CUIAnimScript;
	case (UINT)SCRIPT_TYPE::UIFLOWSCRIPT:
		return new CUIFlowScript;
	case (UINT)SCRIPT_TYPE::UISTARTSCENEFLOWSCRIPT:
		return new CUIStartSceneFlowScript;
	case (UINT)SCRIPT_TYPE::UIGLOWMOVESCRIPT:
		return new CUIGlowMoveScript;
	case (UINT)SCRIPT_TYPE::CHANGECAMTRIGGER:
		return new CChangeCamTrigger;
	case (UINT)SCRIPT_TYPE::PROGRESSCAMTRIGGER:
		return new CProgressCamTrigger;
	case (UINT)SCRIPT_TYPE::TWOTARGETCAMTRIGGER:
		return new CTwoTargetCamTrigger;
	case (UINT)SCRIPT_TYPE::ELFILISFSM:
		return new CElfilisFSM;
	case (UINT)SCRIPT_TYPE::BOSSMGR:
		return new CBossMgr;
	case (UINT)SCRIPT_TYPE::ELFILISARROWSETSCRIPT:
		return new CElfilisArrowSetScript;
	case (UINT)SCRIPT_TYPE::ELFILISARROWSCRIPT:
		return new CElfilisArrowScript;
	case (UINT)SCRIPT_TYPE::ELFILISSWORDSLASHSCRIPT:
		return new CElfilisSwordSlashScript;
	case (UINT)SCRIPT_TYPE::ELFILISSTORMSCRIPT:
		return new CElfilisStormScript;
	case (UINT)SCRIPT_TYPE::ELFILISAIRARROW:
		return new CElfilisAirArrow;
	case (UINT)SCRIPT_TYPE::ELFILISDIMENSIONLASER:
		return new CElfilisDimensionLaser;
	case (UINT)SCRIPT_TYPE::ELFILISLASER:
		return new CElfilisLaser;
	case (UINT)SCRIPT_TYPE::ELFILISBIGFSM:
		return new CElfilisBigFSM;
	case (UINT)SCRIPT_TYPE::CHANGEALPHASCRIPT:
		return new CChangeAlphaScript;
	case (UINT)SCRIPT_TYPE::CRUMBLESCRIPT:
		return new CCrumbleScript;
	case (UINT)SCRIPT_TYPE::SURPRISEBOARDSCRIPT:
		return new CSurpriseBoardScript;
	case (UINT)SCRIPT_TYPE::SURPRISEBOARDATTACKSCRIPT:
		return new CSurpriseBoardAttackScript;
	case (UINT)SCRIPT_TYPE::UIWIPESCRIPT:
		return new CUIWipeScript;
	case (UINT)SCRIPT_TYPE::WIPETRIGGERSCRIPT:
		return new CWipeTriggerScript;
	case (UINT)SCRIPT_TYPE::KIRBYUNITSCRIPT:
		return new CKirbyUnitScript;
	case (UINT)SCRIPT_TYPE::SOLARCHARGESCRIPT:
		return new CSolarChargeScript;
	case (UINT)SCRIPT_TYPE::SOLARONCESCRIPT:
		return new CSolarOnceScript;
	case (UINT)SCRIPT_TYPE::ELEVATORSCRIPT:
		return new CElevatorScript;
	case (UINT)SCRIPT_TYPE::PUSHOUTSCRIPT:
		return new CPushOutScript;
	case (UINT)SCRIPT_TYPE::PUSHOUTTRIGGERSCRIPT:
		return new CPushOutTriggerScript;
	case (UINT)SCRIPT_TYPE::PUSHOUTCOLLIDERSCRIPT:
		return new CPushOutColliderScript;
	case (UINT)SCRIPT_TYPE::LADDERSCRIPT:
		return new CLadderScript;
	case (UINT)SCRIPT_TYPE::KIRBYCHECKPOINTTRIGGER:
		return new CKirbyCheckPointTrigger;
	case (UINT)SCRIPT_TYPE::KIRBYFALLDETECTTRIGGER:
		return new CKirbyFallDetectTrigger;
	case (UINT)SCRIPT_TYPE::FIXEDVIEWCAMTRIGGER:
		return new CFixedViewCamTrigger;
	case (UINT)SCRIPT_TYPE::MORPHOFSM:
		return new CMorphoFSM;
	case (UINT)SCRIPT_TYPE::MORPHOUNIT:
		return new CMorphoUnit;
	case (UINT)SCRIPT_TYPE::ELFILISUNIT:
		return new CElfilisUnit;
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
	case SCRIPT_TYPE::CANJUICE:
		return L"CCanJuice";
	case SCRIPT_TYPE::MONSTERTEST:
		return L"CMonsterTest";
	case SCRIPT_TYPE::MOMENTARYOBJSCRIPT:
		return L"CMomentaryObjScript";
	case SCRIPT_TYPE::KIRBYWEAPONHITBOX:
		return L"CKirbyWeaponHitbox";
	case SCRIPT_TYPE::CAMERACONTROLLER:
		return L"CCameraController";
	case SCRIPT_TYPE::UIMOVEUPDOWNSCRIPT:
		return L"CUIMoveUpDownScript";
	case SCRIPT_TYPE::UIANIMSCRIPT:
		return L"CUIAnimScript";
	case SCRIPT_TYPE::UIFLOWSCRIPT:
		return L"CUIFlowScript";
	case SCRIPT_TYPE::UISTARTSCENEFLOWSCRIPT:
		return L"CUIStartSceneFlowScript";
	case SCRIPT_TYPE::UIGLOWMOVESCRIPT:
		return L"CUIGlowMoveScript";
	case SCRIPT_TYPE::CHANGECAMTRIGGER:
		return L"CChangeCamTrigger";
	case SCRIPT_TYPE::PROGRESSCAMTRIGGER:
		return L"CProgressCamTrigger";
	case SCRIPT_TYPE::TWOTARGETCAMTRIGGER:
		return L"CTwoTargetCamTrigger";
	case SCRIPT_TYPE::ELFILISFSM:
		return L"CElfilisFSM";
	case SCRIPT_TYPE::BOSSMGR:
		return L"CBossMgr";
	case SCRIPT_TYPE::ELFILISARROWSETSCRIPT:
		return L"CElfilisArrowSetScript";
	case SCRIPT_TYPE::ELFILISARROWSCRIPT:
		return L"CElfilisArrowScript";
	case SCRIPT_TYPE::ELFILISSWORDSLASHSCRIPT:
		return L"CElfilisSwordSlashScript";
	case SCRIPT_TYPE::ELFILISSTORMSCRIPT:
		return L"CElfilisStormScript";
	case SCRIPT_TYPE::ELFILISAIRARROW:
		return L"CElfilisAirArrow";
	case SCRIPT_TYPE::ELFILISDIMENSIONLASER:
		return L"CElfilisDimensionLaser";
	case SCRIPT_TYPE::ELFILISLASER:
		return L"CElfilisLaser";
	case SCRIPT_TYPE::ELFILISBIGFSM:
		return L"CElfilisBigFSM";
	case SCRIPT_TYPE::CHANGEALPHASCRIPT:
		return L"CChangeAlphaScript";
	case SCRIPT_TYPE::CRUMBLESCRIPT:
		return L"CCrumbleScript";
	case SCRIPT_TYPE::SURPRISEBOARDSCRIPT:
		return L"CSurpriseBoardScript";
	case SCRIPT_TYPE::SURPRISEBOARDATTACKSCRIPT:
		return L"CSurpriseBoardAttackScript";
	case SCRIPT_TYPE::UIWIPESCRIPT:
		return L"CUIWipeScript";
	case SCRIPT_TYPE::WIPETRIGGERSCRIPT:
		return L"CWipeTriggerScript";
	case SCRIPT_TYPE::KIRBYUNITSCRIPT:
		return L"CKirbyUnitScript";
	case SCRIPT_TYPE::SOLARCHARGESCRIPT:
		return L"CSolarChargeScript";
	case SCRIPT_TYPE::SOLARONCESCRIPT:
		return L"CSolarOnceScript";
	case SCRIPT_TYPE::ELEVATORSCRIPT:
		return L"CElevatorScript";
	case SCRIPT_TYPE::PUSHOUTSCRIPT:
		return L"CPushOutScript";
	case SCRIPT_TYPE::PUSHOUTTRIGGERSCRIPT:
		return L"CPushOutTriggerScript";
	case SCRIPT_TYPE::PUSHOUTCOLLIDERSCRIPT:
		return L"CPushOutColliderScript";
	case SCRIPT_TYPE::LADDERSCRIPT:
		return L"CLadderScript";
	case SCRIPT_TYPE::KIRBYCHECKPOINTTRIGGER:
		return L"CKirbyCheckPointTrigger";
	case SCRIPT_TYPE::KIRBYFALLDETECTTRIGGER:
		return L"CKirbyFallDetectTrigger";
	case SCRIPT_TYPE::FIXEDVIEWCAMTRIGGER:
		return L"CFixedViewCamTrigger";
	case SCRIPT_TYPE::MORPHOFSM:
		return L"CMorphoFSM";
	case SCRIPT_TYPE::MORPHOUNIT:
		return L"CMorphoUnit";
	case SCRIPT_TYPE::ELFILISUNIT:
		return L"CElfilisUnit";
	}
	return nullptr;
}