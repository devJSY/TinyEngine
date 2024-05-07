#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CConflictTest.h"
#include "CPhysicsTestScript.h"
#include "CProjectileScript.h"
#include "CShooterScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CConflictTest");
	_vec.push_back(L"CPhysicsTestScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CShooterScript");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CConflictTest" == _strScriptName)
		return new CConflictTest;
	if (L"CPhysicsTestScript" == _strScriptName)
		return new CPhysicsTestScript;
	if (L"CProjectileScript" == _strScriptName)
		return new CProjectileScript;
	if (L"CShooterScript" == _strScriptName)
		return new CShooterScript;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
	case (UINT)SCRIPT_TYPE::CONFLICTTEST:
		return new CConflictTest;
	case (UINT)SCRIPT_TYPE::PHYSICSTESTSCRIPT:
		return new CPhysicsTestScript;
	case (UINT)SCRIPT_TYPE::PROJECTILESCRIPT:
		return new CProjectileScript;
	case (UINT)SCRIPT_TYPE::SHOOTERSCRIPT:
		return new CShooterScript;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
	case SCRIPT_TYPE::CONFLICTTEST:
		return L"CConflictTest";
	case SCRIPT_TYPE::PHYSICSTESTSCRIPT:
		return L"CPhysicsTestScript";
	case SCRIPT_TYPE::PROJECTILESCRIPT:
		return L"CProjectileScript";
	case SCRIPT_TYPE::SHOOTERSCRIPT:
		return L"CShooterScript";
	}
	return nullptr;
}