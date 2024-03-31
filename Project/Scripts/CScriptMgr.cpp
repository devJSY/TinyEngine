#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CElevatorScript.h"
#include "CEnemyHitBoxScript.h"
#include "CFlyingBookScript.h"
#include "CGameManagerScript.h"
#include "CGhostScript.h"
#include "CLevelChangeScript.h"
#include "CPlayButtonScript.h"
#include "CPlayerCameraScript.h"
#include "CPlayerDetectScript.h"
#include "CPlayerHitBoxScript.h"
#include "CPlayerLifeBarScript.h"
#include "CPlayerManaBarScript.h"
#include "CPlayerScript.h"
#include "CProjectile2DScript.h"
#include "CPyroGhostScript.h"
#include "CSkeletonHandScript.h"
#include "CSpooksmenScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CElevatorScript");
	_vec.push_back(L"CEnemyHitBoxScript");
	_vec.push_back(L"CFlyingBookScript");
	_vec.push_back(L"CGameManagerScript");
	_vec.push_back(L"CGhostScript");
	_vec.push_back(L"CLevelChangeScript");
	_vec.push_back(L"CPlayButtonScript");
	_vec.push_back(L"CPlayerCameraScript");
	_vec.push_back(L"CPlayerDetectScript");
	_vec.push_back(L"CPlayerHitBoxScript");
	_vec.push_back(L"CPlayerLifeBarScript");
	_vec.push_back(L"CPlayerManaBarScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectile2DScript");
	_vec.push_back(L"CPyroGhostScript");
	_vec.push_back(L"CSkeletonHandScript");
	_vec.push_back(L"CSpooksmenScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CElevatorScript" == _strScriptName)
		return new CElevatorScript;
	if (L"CEnemyHitBoxScript" == _strScriptName)
		return new CEnemyHitBoxScript;
	if (L"CFlyingBookScript" == _strScriptName)
		return new CFlyingBookScript;
	if (L"CGameManagerScript" == _strScriptName)
		return new CGameManagerScript;
	if (L"CGhostScript" == _strScriptName)
		return new CGhostScript;
	if (L"CLevelChangeScript" == _strScriptName)
		return new CLevelChangeScript;
	if (L"CPlayButtonScript" == _strScriptName)
		return new CPlayButtonScript;
	if (L"CPlayerCameraScript" == _strScriptName)
		return new CPlayerCameraScript;
	if (L"CPlayerDetectScript" == _strScriptName)
		return new CPlayerDetectScript;
	if (L"CPlayerHitBoxScript" == _strScriptName)
		return new CPlayerHitBoxScript;
	if (L"CPlayerLifeBarScript" == _strScriptName)
		return new CPlayerLifeBarScript;
	if (L"CPlayerManaBarScript" == _strScriptName)
		return new CPlayerManaBarScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectile2DScript" == _strScriptName)
		return new CProjectile2DScript;
	if (L"CPyroGhostScript" == _strScriptName)
		return new CPyroGhostScript;
	if (L"CSkeletonHandScript" == _strScriptName)
		return new CSkeletonHandScript;
	if (L"CSpooksmenScript" == _strScriptName)
		return new CSpooksmenScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::ELEVATORSCRIPT:
		return new CElevatorScript;
	case (UINT)SCRIPT_TYPE::ENEMYHITBOXSCRIPT:
		return new CEnemyHitBoxScript;
	case (UINT)SCRIPT_TYPE::FLYINGBOOKSCRIPT:
		return new CFlyingBookScript;
	case (UINT)SCRIPT_TYPE::GAMEMANAGERSCRIPT:
		return new CGameManagerScript;
	case (UINT)SCRIPT_TYPE::GHOSTSCRIPT:
		return new CGhostScript;
	case (UINT)SCRIPT_TYPE::LEVELCHANGESCRIPT:
		return new CLevelChangeScript;
	case (UINT)SCRIPT_TYPE::PLAYBUTTONSCRIPT:
		return new CPlayButtonScript;
	case (UINT)SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return new CPlayerCameraScript;
	case (UINT)SCRIPT_TYPE::PLAYERDETECTSCRIPT:
		return new CPlayerDetectScript;
	case (UINT)SCRIPT_TYPE::PLAYERHITBOXSCRIPT:
		return new CPlayerHitBoxScript;
	case (UINT)SCRIPT_TYPE::PLAYERLIFEBARSCRIPT:
		return new CPlayerLifeBarScript;
	case (UINT)SCRIPT_TYPE::PLAYERMANABARSCRIPT:
		return new CPlayerManaBarScript;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
	case (UINT)SCRIPT_TYPE::PROJECTILE2DSCRIPT:
		return new CProjectile2DScript;
	case (UINT)SCRIPT_TYPE::PYROGHOSTSCRIPT:
		return new CPyroGhostScript;
	case (UINT)SCRIPT_TYPE::SKELETONHANDSCRIPT:
		return new CSkeletonHandScript;
	case (UINT)SCRIPT_TYPE::SPOOKSMENSCRIPT:
		return new CSpooksmenScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::ELEVATORSCRIPT:
		return L"CElevatorScript";
	case SCRIPT_TYPE::ENEMYHITBOXSCRIPT:
		return L"CEnemyHitBoxScript";
	case SCRIPT_TYPE::FLYINGBOOKSCRIPT:
		return L"CFlyingBookScript";
	case SCRIPT_TYPE::GAMEMANAGERSCRIPT:
		return L"CGameManagerScript";
	case SCRIPT_TYPE::GHOSTSCRIPT:
		return L"CGhostScript";
	case SCRIPT_TYPE::LEVELCHANGESCRIPT:
		return L"CLevelChangeScript";
	case SCRIPT_TYPE::PLAYBUTTONSCRIPT:
		return L"CPlayButtonScript";
	case SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return L"CPlayerCameraScript";
	case SCRIPT_TYPE::PLAYERDETECTSCRIPT:
		return L"CPlayerDetectScript";
	case SCRIPT_TYPE::PLAYERHITBOXSCRIPT:
		return L"CPlayerHitBoxScript";
	case SCRIPT_TYPE::PLAYERLIFEBARSCRIPT:
		return L"CPlayerLifeBarScript";
	case SCRIPT_TYPE::PLAYERMANABARSCRIPT:
		return L"CPlayerManaBarScript";
	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
	case SCRIPT_TYPE::PROJECTILE2DSCRIPT:
		return L"CProjectile2DScript";
	case SCRIPT_TYPE::PYROGHOSTSCRIPT:
		return L"CPyroGhostScript";
	case SCRIPT_TYPE::SKELETONHANDSCRIPT:
		return L"CSkeletonHandScript";
	case SCRIPT_TYPE::SPOOKSMENSCRIPT:
		return L"CSpooksmenScript";
	}
	return nullptr;
}