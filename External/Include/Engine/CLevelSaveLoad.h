#pragma once

class CLevel;
class CLayer;
class CGameObject;

class CLevelSaveLoad
{
public:
    static int SaveLevel(const wstring& _FileName, CLevel* _Level);
    static int SaveGameObject(CGameObject* _Object, FILE* _File);

    static CLevel* LoadLevel(const wstring& _FileName);
    static CGameObject* LoadGameObject(FILE* _File);

    static const wstring& GetLevelExtension() { return Level_extension; }

private:
    static wstring Level_extension;
};
