#include "pch.h"
#include "CLevelSaveLoad.h"

#include "CPhysics2DMgr.h"
#include "CPhysicsMgr.h"
#include <Scripts\\CScriptMgr.h>

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CScript.h"

wstring CLevelSaveLoad::Level_extension = L".tLevel";
const UINT CLevelSaveLoad::MemoryBlockSize = 256;

void CLevelSaveLoad::SaveLevel(CLevel* _Level, const wstring& _LevelFileName)
{
    assert(_Level);

    if (_Level->GetState() != LEVEL_STATE::STOP)
        return;

    // Save 시 지정된 확장자로 저장하도록 설정
    wstring strLevelPath = CPathMgr::GetContentPath();
    strLevelPath += L"Levels\\";
    strLevelPath += _LevelFileName;

    std::filesystem::path filePath = strLevelPath;

    // 확장자가 입력되지 않은 경우
    if ("" == filePath.extension())
    {
        strLevelPath += Level_extension;
    }
    // 확장자가 잘못 입력된 경우
    else if (Level_extension != filePath.extension())
    {
        strLevelPath = filePath.replace_extension(Level_extension);
    }

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

    if (nullptr == pFile)
        return;

    // 레벨 이름
    _Level->SetName(_LevelFileName);
    SaveWStringToFile(_Level->GetName(), pFile);

    // 레벨의 레이어 저장
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        SaveLayer(_Level->GetLayer(i), pFile);
    }

    // CPhysics2DMgr
    {
        for (UINT i = 0; i < LAYER_MAX; ++i)
        {
            WORD ColLayer = CPhysics2DMgr::GetInst()->GetCollisionLayer(i);
            fwrite(&ColLayer, sizeof(WORD), 1, pFile);
        }

        Vec2 Gravity = CPhysics2DMgr::GetInst()->GetGravity();
        fwrite(&Gravity, sizeof(Vec2), 1, pFile);

        float PPM = CPhysics2DMgr::GetInst()->GetPPM();
        fwrite(&PPM, sizeof(float), 1, pFile);
    }

    // CPhysicsMgr
    {
        for (UINT i = 0; i < LAYER_MAX; ++i)
        {
            WORD ColLayer = CPhysicsMgr::GetInst()->GetCollisionLayer(i);
            fwrite(&ColLayer, sizeof(WORD), 1, pFile);
        }

        Vec3 Gravity = CPhysicsMgr::GetInst()->GetGravity();
        fwrite(&Gravity, sizeof(Vec3), 1, pFile);

        float PPM = CPhysicsMgr::GetInst()->GetPPM();
        fwrite(&PPM, sizeof(float), 1, pFile);
    }

    fclose(pFile);
}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, FILE* _File)
{
    // Layer 의 이름 저장
    SaveWStringToFile(_Layer->GetName(), _File);

    // Layer 가 보유하고 있는 GameObject 들을 저장
    const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();

    size_t ObjCount = vecObject.size();
    fwrite(&ObjCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < vecObject.size(); ++i)
    {
        SaveGameObject(vecObject[i], _File);
    }
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, FILE* _File)
{
    // GameObject 의 이름을 저장
    SaveWStringToFile(_Obj->GetName(), _File);

    // 레이어 인덱스 저장
    int layerIdx = _Obj->GetLayerIdx();
    fwrite(&layerIdx, sizeof(int), 1, _File);

    // Active 저장
    bool Active = _Obj->m_bActive;
    fwrite(&Active, sizeof(bool), 1, _File);

    vector<pair<UINT, CComponent*>> vecComps;

    // 컴포넌트 정보를 저장
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
        if (nullptr == pCom)
            continue;

        vecComps.push_back({i, pCom});
    }

    // 컴포넌트 개수 저장
    size_t CompCount = vecComps.size();
    fwrite(&CompCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < CompCount; i++)
    {
        // 컴포넌트의 이름 저장
        SaveWStringToFile(ToWstring(COMPONENT_TYPE_STRING[vecComps[i].first]), _File);

        // 해당 컴포넌트가 저장할 데이터 저장
        vecComps[i].second->SaveToLevelFile(_File);
    }

    // 스크립트 정보 저장
    const vector<CScript*>& vecScripts = _Obj->GetScripts();
    size_t ScriptCount = vecScripts.size();

    // 스크립트 개수 저장
    fwrite(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        SaveWStringToFile(CScriptMgr::GetScriptName(vecScripts[i]), _File);
        UINT MemoryByte = vecScripts[i]->SaveToLevelFile(_File);

        // 해당 스크립트의 메모리가 지정된 메모리 블록보다 큰 경우
        if (MemoryByte > MemoryBlockSize)
        {
            assert(nullptr);
        }

        // 0값으로 패딩
        UINT PaddingByte = MemoryBlockSize - MemoryByte;
        vector<char> padding(PaddingByte, 0);
        fwrite(padding.data(), padding.size(), 1, _File);
    }

    // BoneSocket 저장
    int BoneIndex = -1;
    wstring BoneSocketName;
    const tBoneSocket* pBoneSocket = _Obj->GetBoneSocket();
    if (nullptr != pBoneSocket)
    {
        BoneIndex = pBoneSocket->BoneIndex;
        BoneSocketName = pBoneSocket->SoketName;
    }

    fwrite(&BoneIndex, sizeof(int), 1, _File);
    SaveWStringToFile(BoneSocketName, _File);

    // 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
    const vector<CGameObject*>& vecChild = _Obj->GetChildObject();
    size_t childcount = vecChild.size();
    fwrite(&childcount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < childcount; ++i)
    {
        SaveGameObject(vecChild[i], _File);
    }
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _LevelFileName)
{
    // Level 을 불러올 경로 설정
    wstring strLevelPath = CPathMgr::GetContentPath();
    strLevelPath += L"Levels\\";
    strLevelPath += _LevelFileName;

    std::filesystem::path filePath = strLevelPath;

    // 확장자가 입력되지 않은 경우
    if ("" == filePath.extension())
    {
        strLevelPath += Level_extension;
    }
    // 확장자가 잘못 입력된 경우
    else if (Level_extension != filePath.extension())
    {
        strLevelPath = filePath.replace_extension(Level_extension);
    }

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strLevelPath.c_str(), L"rb");

    if (nullptr == pFile)
    {
        MessageBox(nullptr, L"레벨 경로가 올바르지 않거나, 레벨이 존재하지 않음", L"레벨 로드 실패", MB_OK);
        return nullptr;
    }

    // 레벨의 이름 로드
    CLevel* pLevel = new CLevel;
    wstring strLevelName;
    LoadWStringFromFile(strLevelName, pFile);
    pLevel->SetName(strLevelName);

    // Layer 로드
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        LoadLayer(pLevel->GetLayer(i), pFile);
    }

    // CPhysics2DMgr
    {
        for (UINT i = 0; i < LAYER_MAX; ++i)
        {
            WORD ColLayer = 0;
            fread(&ColLayer, sizeof(WORD), 1, pFile);
            CPhysics2DMgr::GetInst()->SetCollisionLayer(i, ColLayer);
        }

        Vec2 Gravity = Vec2();
        fread(&Gravity, sizeof(Vec2), 1, pFile);
        CPhysics2DMgr::GetInst()->SetGravity(Gravity);

        float PPM = 0.f;
        fread(&PPM, sizeof(float), 1, pFile);
        CPhysics2DMgr::GetInst()->SetPPM(PPM);
    }

    // CPhysicsMgr
    {
        for (UINT i = 0; i < LAYER_MAX; ++i)
        {
            WORD ColLayer = 0;
            fread(&ColLayer, sizeof(WORD), 1, pFile);
            CPhysicsMgr::GetInst()->SetCollisionLayer(i, ColLayer);
        }

        Vec3 Gravity = Vec3();
        fread(&Gravity, sizeof(Vec3), 1, pFile);
        CPhysicsMgr::GetInst()->SetGravity(Gravity);

        float PPM = 0.f;
        fread(&PPM, sizeof(float), 1, pFile);
        CPhysicsMgr::GetInst()->SetPPM(PPM);
    }

    fclose(pFile);

    return pLevel;
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, FILE* _File)
{
    // Layer 의 이름 로드
    wstring strLayerName;
    LoadWStringFromFile(strLayerName, _File);
    _Layer->SetName(strLayerName);

    // Layer 가 보유하던 GameObject 들을 불러온다.
    size_t ObjCount = 0;
    fread(&ObjCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ObjCount; ++i)
    {
        _Layer->AddObject(LoadGameObject(nullptr, _File), false);
    }
}

CGameObject* CLevelSaveLoad::LoadGameObject(CGameObject* _ParentObj, FILE* _File)
{
    CGameObject* pObject = new CGameObject;

    // GameObject 의 이름을 로드
    wstring strName;
    LoadWStringFromFile(strName, _File);
    pObject->SetName(strName);

    // 레이어 인덱스 설정
    int layerIdx = 0;
    fread(&layerIdx, sizeof(int), 1, _File);
    pObject->m_iLayerIdx = layerIdx;

    // Active
    bool Active = true;
    fread(&Active, sizeof(bool), 1, _File);
    pObject->m_bActive = Active;

    // 컴포넌트 개수 읽기
    size_t CompCount = 0;
    fread(&CompCount, sizeof(size_t), 1, _File);

    // 컴포넌트 정보를 불러오기
    for (size_t i = 0; i < CompCount; i++)
    {
        wstring CompName;
        LoadWStringFromFile(CompName, _File);
        string CompNameStr = ToString(CompName);
        COMPONENT_TYPE type = COMPONENT_TYPE::END;

        // 문자열과 일치하는 타입을 탐색
        for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
        {
            if (CompNameStr == COMPONENT_TYPE_STRING[i])
            {
                type = (COMPONENT_TYPE)i;
                break;
            }
        }

        CComponent* pComponent = nullptr;

        switch (type)
        {
        case COMPONENT_TYPE::TRANSFORM:
            pComponent = new CTransform;
            break;
        case COMPONENT_TYPE::ANIMATOR2D:
            pComponent = new CAnimator2D;
            break;
        case COMPONENT_TYPE::ANIMATOR:
            pComponent = new CAnimator;
            break;
        case COMPONENT_TYPE::LIGHT2D:
            pComponent = new CLight2D;
            break;
        case COMPONENT_TYPE::LIGHT:
            pComponent = new CLight;
            break;
        case COMPONENT_TYPE::CAMERA:
            pComponent = new CCamera;
            break;
        case COMPONENT_TYPE::RIGIDBODY2D:
            pComponent = new CRigidbody2D;
            break;
        case COMPONENT_TYPE::BOXCOLLIDER2D:
            pComponent = new CBoxCollider2D;
            break;
        case COMPONENT_TYPE::CIRCLECOLLIDER2D:
            pComponent = new CCircleCollider2D;
            break;
        case COMPONENT_TYPE::POLYGONCOLLIDER2D:
            pComponent = new CPolygonCollider2D;
            break;
        case COMPONENT_TYPE::EDGECOLLIDER2D:
            pComponent = new CEdgeCollider2D;
            break;
        case COMPONENT_TYPE::RIGIDBODY:
            pComponent = new CRigidbody;
            break;
        case COMPONENT_TYPE::BOXCOLLIDER:
            pComponent = new CBoxCollider;
            break;
        case COMPONENT_TYPE::SPHERECOLLIDER:
            pComponent = new CSphereCollider;
            break;
        case COMPONENT_TYPE::CAPSULECOLLIDER:
            pComponent = new CCapsuleCollider;
            break;
        case COMPONENT_TYPE::MESHCOLLIDER:
            pComponent = new CMeshCollider;
            break;
        case COMPONENT_TYPE::CHARACTERCONTROLLER:
            pComponent = new CCharacterController;
            break;
        case COMPONENT_TYPE::MESHRENDER:
            pComponent = new CMeshRender;
            break;
        case COMPONENT_TYPE::TILEMAP:
            pComponent = new CTileMap;
            break;
        case COMPONENT_TYPE::PARTICLESYSTEM:
            pComponent = new CParticleSystem;
            break;
        case COMPONENT_TYPE::SKYBOX:
            pComponent = new CSkyBox;
            break;
        case COMPONENT_TYPE::DECAL:
            pComponent = new CDecal;
            break;
        case COMPONENT_TYPE::LANDSCAPE:
            pComponent = new CLandScape;
            break;
        case COMPONENT_TYPE::TEXTRENDER:
            pComponent = new CTextRender;
            break;
        default:
            assert(nullptr); // 타입에 맞는 컴포넌트가 존재하지않았다
            break;
        }

        // 해당 컴포넌트가 저장한 데이터를 로드
        pObject->AddComponent(pComponent);

        pComponent->LoadFromLevelFile(_File);
    }

    // 컴포넌트 매쉬 설정
    if (nullptr != pObject->MeshRender())
    {
        if (nullptr != pObject->Animator())
        {
            pObject->Animator()->SetSkeletalMesh(pObject->MeshRender()->GetMesh());
        }

        if (nullptr != pObject->MeshCollider())
        {
            pObject->MeshCollider()->SetMesh(pObject->MeshRender()->GetMesh());
        }
    }

    // 스크립트 개수 읽기
    size_t ScriptCount = 0;
    fread(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        wstring strScriptName;
        LoadWStringFromFile(strScriptName, _File);

        CScript* pScript = CScriptMgr::GetScript(strScriptName);
        pObject->AddComponent(pScript);
        UINT MemoryByte = pScript->LoadFromLevelFile(_File);

        // 해당 스크립트의 메모리가 지정된 메모리 블록보다 큰 경우
        if (MemoryByte > MemoryBlockSize)
        {
            assert(nullptr);
        }

        // 0값으로 패딩
        UINT PaddingByte = MemoryBlockSize - MemoryByte;
        vector<char> padding(PaddingByte, 0);
        fread(padding.data(), padding.size(), 1, _File);
    }

    // BoneSocket 로드
    int BoneIndex = -1;
    wstring BoneSocketName;
    fread(&BoneIndex, sizeof(int), 1, _File);
    LoadWStringFromFile(BoneSocketName, _File);

    if (-1 != BoneIndex)
    {
        const tMTBone& Bone = _ParentObj->Animator()->GetSkeletalMesh()->GetBones()->at(BoneIndex);
        for (tBoneSocket* BoneSocket : Bone.vecBoneSocket)
        {
            if (BoneSocket->SoketName == BoneSocketName)
            {
                pObject->SetBoneSocket(BoneSocket);
                break;
            }
        }
    }

    // 자식 오브젝트가 있으면 자식 오브젝트 정보 저장
    size_t childcount = 0;
    fread(&childcount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < childcount; ++i)
    {
        pObject->AddChild(LoadGameObject(pObject, _File));
    }

    return pObject;
}
