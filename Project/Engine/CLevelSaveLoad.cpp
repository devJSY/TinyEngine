#include "pch.h"
#include "CLevelSaveLoad.h"

#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CLevelMgr.h"
#include "CEditorMgr.h"

#include "CLevel.h"

#include "CScript.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "components.h"

wstring CLevelSaveLoad::Level_extension = L".tLevel";

int CLevelSaveLoad::SaveLevel(const wstring& _FileName, CLevel* _Level)
{
    // if (_Level->GetState() != LEVEL_STATE::STOP)
    //     return E_FAIL;

    wstring strPath = CPathMgr::GetContentPath();
    strPath += L"Levels\\";
    strPath += _FileName;

    std::filesystem::path filePath = strPath;

    if ("" == filePath.extension())
    {
        // 확장자가 입력되지 않은 경운
        strPath += Level_extension;
    }
    else if (Level_extension != filePath.extension())
    {
        // 확장자가 잘못 입력된 경우
        strPath = filePath.replace_extension(Level_extension);
    }

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // 레벨 이름 저장
    SaveWString(_Level->GetName(), pFile);

    // 레벨의 레이어들을 저장
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        CLayer* pLayer = _Level->GetLayer(i);

        // 레이어 이름 저장
        SaveWString(pLayer->GetName(), pFile);

        // 레이어의 게임오브젝트들 저장
        const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

        // 오브젝트 개수 저장
        size_t objCount = vecParent.size();
        fwrite(&objCount, sizeof(size_t), 1, pFile);

        // 각 게임오브젝트
        for (size_t i = 0; i < objCount; ++i)
        {
            SaveGameObject(vecParent[i], pFile);
        }
    }

    // Collision Layer
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        UINT row = CCollisionMgr::GetInst()->GetCollisionLayer(i);
        fwrite(&row, sizeof(UINT), 1, pFile);
    }

    fclose(pFile);

    return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
    // 이름
    SaveWString(_Object->GetName(), _File);

    // 컴포넌트
    for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (i == (UINT)COMPONENT_TYPE::END)
        {
            // 컴포넌트 타입 저장
            fwrite(&i, sizeof(UINT), 1, _File);
            break;
        }

        CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
        if (nullptr == Com)
            continue;

        // 컴포넌트 타입 저장
        fwrite(&i, sizeof(UINT), 1, _File);

        // 컴포넌트 정보 저장
        Com->SaveToLevelFile(_File);
    }

    // 스크립트
    const vector<CScript*>& vecScript = _Object->GetScripts();
    size_t ScriptCount = vecScript.size();
    fwrite(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
        SaveWString(ScriptName, _File);
        vecScript[i]->SaveToLevelFile(_File);
    }

    // 자식 오브젝트
    const vector<CGameObject*>& vecChild = _Object->GetChildObject();
    size_t ChildCount = vecChild.size();
    fwrite(&ChildCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _File);
    }

    return 0;
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _FileName)
{
    wstring strPath = CPathMgr::GetContentPath();
    strPath += L"Levels\\";
    strPath += _FileName;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"rb");

    if (nullptr == pFile)
        return nullptr;

    CLevel* NewLevel = new CLevel;

    // 레벨 이름
    wstring strLevelName;
    LoadWString(strLevelName, pFile);
    NewLevel->SetName(strLevelName);

    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        CLayer* pLayer = NewLevel->GetLayer(i);

        // 레이어 이름
        wstring LayerName;
        LoadWString(LayerName, pFile);
        pLayer->SetName(LayerName);

        // 게임 오브젝트 개수
        size_t objCount = 0;
        fread(&objCount, sizeof(size_t), 1, pFile);

        // 각 게임오브젝트
        for (size_t j = 0; j < objCount; ++j)
        {
            CGameObject* pNewObj = LoadGameObject(pFile);
            NewLevel->AddObject(pNewObj, i, false);
        }
    }

    // Collision Layer
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        UINT row = -1;
        fread(&row, sizeof(UINT), 1, pFile);
        CCollisionMgr::GetInst()->SetCollisionLayer(i, row);
    }

    fclose(pFile);

    /*NewLevel->ChangeState(LEVEL_STATE::STOP);*/

    return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
    CGameObject* pObject = new CGameObject;

    // 이름
    wstring Name;
    LoadWString(Name, _File);
    pObject->SetName(Name);

    // 컴포넌트
    while (true)
    {
        UINT ComponentType = 0;
        fread(&ComponentType, sizeof(UINT), 1, _File);

        // 컴포넌트 정보의 끝을 확인
        if ((UINT)COMPONENT_TYPE::END == ComponentType)
            break;

        CComponent* Component = nullptr;

        switch ((COMPONENT_TYPE)ComponentType)
        {
        case COMPONENT_TYPE::TRANSFORM:
            Component = new CTransform;
            break;
        case COMPONENT_TYPE::COLLIDER2D:
            Component = new CCollider2D;
            break;
        case COMPONENT_TYPE::COLLIDER3D:
            break;
        case COMPONENT_TYPE::ANIMATOR2D:
            Component = new CAnimator2D;
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
            break;
        case COMPONENT_TYPE::LIGHT2D:
            Component = new CLight2D();
            break;
        case COMPONENT_TYPE::LIGHT3D:
            Component = new CLight3D();
            break;
        case COMPONENT_TYPE::CAMERA:
            Component = new CCamera;
            break;
        case COMPONENT_TYPE::MESHRENDER:
            Component = new CMeshRender;
            break;
        case COMPONENT_TYPE::TILEMAP:
            Component = new CTileMap;
            break;
        case COMPONENT_TYPE::PARTICLESYSTEM:
            Component = new CParticleSystem;
            break;
        case COMPONENT_TYPE::SKYBOX:
            break;
        case COMPONENT_TYPE::DECAL:
            break;
        case COMPONENT_TYPE::LANDSCAPE:
            break;
        }

        Component->LoadFromLevelFile(_File);
        pObject->AddComponent(Component);
    }

    // 스크립트
    size_t ScriptCount = 0;
    fread(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        wstring ScriptName;
        LoadWString(ScriptName, _File);
        CScript* pScript = CScriptMgr::GetScript(ScriptName);
        pObject->AddComponent(pScript);
        pScript->LoadFromLevelFile(_File);
    }

    // 자식 오브젝트
    size_t ChildCount = 0;
    fread(&ChildCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ChildCount; ++i)
    {
        CGameObject* ChildObject = LoadGameObject(_File);
        pObject->AddChild(ChildObject);
    }

    return pObject;
}
