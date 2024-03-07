#pragma once
#include "CAsset.h"

typedef void (*SAVE_TYPE)(CGameObject*, FILE*);
typedef CGameObject* (*LOAD_TYPE)(FILE*);

class CPrefab : public CAsset
{
public:
    static SAVE_TYPE GAMEOBJECT_SAVE;
    static LOAD_TYPE GAMEOBJECT_LOAD;

private:
    CGameObject* m_ProtoObj;

public:
    void SetGameObject(CGameObject* _Object);
    CGameObject* Instantiate();

public:
    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

    CLONE_DISABLE(CPrefab);

public:
    CPrefab(bool _bEngineAsset = false);
    CPrefab(CGameObject* _Proto, bool _bEngineAsset = false);
    virtual ~CPrefab();
};
