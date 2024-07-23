#pragma once

class CLevel;
class CLayer;
class CGameObject;

class CLevelSaveLoad
{
public:
    static void SaveLevel(CLevel* _Level, const wstring& _LevelFileName);
    static void SaveLayer(CLayer* _Layer, FILE* _File);
    static void SaveGameObject(CGameObject* _Obj, FILE* _File);

    static CLevel* LoadLevel(const wstring& _LevelFileName);
    static void LoadLayer(CLayer* _Layer, FILE* _File);
    static CGameObject* LoadGameObject(CGameObject* _ParentObj, FILE* _File);

public:
    static const wstring& GetLevelExtension() { return Level_extension; }

private:
    static wstring Level_extension;
    const static UINT MemoryBlockSize;
};
