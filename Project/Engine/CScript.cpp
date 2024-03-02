#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _ScriptType)
    : CComponent(COMPONENT_TYPE::SCRIPT)
    , m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx)
{
    CGameObject* pNewObj = _Prefab->Instantiate();

    pNewObj->Transform()->SetRelativePos(_vWorldPos);

    GamePlayStatic::SpawnGameObject(pNewObj, _LayerIdx);
}
