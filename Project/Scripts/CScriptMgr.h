#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVESCRIPT,
	KIRBYFSM,
	KIRBYMOVECONTROLLER,
	MODELEDITORCAMERAMOVESCRIPT,
	PLAYERMGR,
	KIRBYBULLETSCRIPT,
	BUTTONSCRIPT,
	UIMANAGERSCRIPT,
	BUTTONMANAGERSCRIPT,
	KIRBYVACUUMCOLLIDER,
	KIRBYCOPYABILITYSCRIPT,
	KIRBYCOPYOBJSCRIPT,
	KIRBYBODYCOLLIDER,
	UITEXMANAGERSCRIPT,
	UITEXSCRIPT,
	UIANIMMANAGERSCRIPT,
	LEVELCHANGEBUTTONSCRIPT,
	STAGECHANGEBUTTONSCRIPT,
	EXITBUTTON,
	UIHPSCRIPT,
	UIUNITMANAGERSCRIPT,
	ENGINETESTSCRIPT,
	BLADEKNIGHTSCRIPT,
	DETECTAREASCRIPT,
	BLADEKNIGHTSWORDSCRIPT,
	NODDYSCRIPT,
	HOTHEADSCRIPT,
	KABUSCRIPT,
	SIRKIBBLESCRIPT,
	GHOSTGORDOSCRIPT,
	GORDOLONGSCRIPT,
	NORMALENEMYSCRIPT,
	TACKLEENEMYSCRIPT,
	CUTTERPROJECTILESCRIPT,
	FIREPROJECTILESCRIPT,
	ATTACKAREASCRIPT,
	DEFORMOBJECTSCRIPT,
	KIRBYHATBLADE,
	KIRBYCUTTERBULLET,
	CANJUICE,
	MONSTERTEST,
	MOMENTARYOBJSCRIPT,
	KIRBYWEAPONHITBOX,
	CAMERACONTROLLER,
	UIMOVEUPDOWNSCRIPT,
	UIANIMSCRIPT,
	UIFLOWSCRIPT,
	UISTARTSCENEFLOWSCRIPT,
	UIGLOWMOVESCRIPT,
	CHANGECAMTRIGGER,
	PROGRESSCAMTRIGGER,
	TWOTARGETCAMTRIGGER,
	CRUMBLESCRIPT,
	SURPRISEBOARDSCRIPT,
	SURPRISEBOARDATTACKSCRIPT,
	UIWIPESCRIPT,
	WIPETRIGGERSCRIPT,
	KIRBYUNITSCRIPT,
	SOLARCHARGESCRIPT,
	SOLARONCESCRIPT,
	ELEVATORSCRIPT,
	PUSHOUTSCRIPT,
	PUSHOUTTRIGGERSCRIPT,
	PUSHOUTCOLLIDERSCRIPT,
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
