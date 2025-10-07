#include "pch.h"
#include "CLevelSaveLoad.h"

#include "CPhysics2DMgr.h"
#include "CPhysicsMgr.h"
#include <Scripts/CScriptMgr.h>

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CScript.h"

wstring CLevelSaveLoad::Level_extension = L".tLevel";
const UINT CLevelSaveLoad::MemoryBlockSize = 8192;

void CLevelSaveLoad::SaveLevel(CLevel* _Level, const wstring& _LevelFileName)
{
    if (nullptr == _Level)
    {
        assert(nullptr);
        return;
    }

    if (_Level->GetState() != LEVEL_STATE::STOP)
        return;

    // Save �� ������ Ȯ���ڷ� �����ϵ��� ����
    wstring strLevelPath = CPathMgr::GetContentPath();
    strLevelPath += L"Levels/";
    strLevelPath += _LevelFileName;

    std::filesystem::path filePath = strLevelPath;

    // Ȯ���ڰ� �Էµ��� ���� ���
    if ("" == filePath.extension())
    {
        strLevelPath += Level_extension;
    }
    // Ȯ���ڰ� �߸� �Էµ� ���
    else if (Level_extension != filePath.extension())
    {
        strLevelPath = filePath.replace_extension(Level_extension);
    }

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

    if (nullptr == pFile)
        return;

    // ���� �̸�
    _Level->SetName(_LevelFileName);
    SaveWStringToFile(_Level->GetName(), pFile);

    // ������ ���̾� ����
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
    // Layer �� �̸� ����
    SaveWStringToFile(_Layer->GetName(), _File);

    // Layer �� �����ϰ� �ִ� GameObject ���� ����
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
    // GameObject �� �̸��� ����
    SaveWStringToFile(_Obj->GetName(), _File);

    // ���̾� �ε��� ����
    int layerIdx = _Obj->GetLayerIdx();
    fwrite(&layerIdx, sizeof(int), 1, _File);

    // Active ����
    bool Active = _Obj->m_bActive;
    fwrite(&Active, sizeof(bool), 1, _File);

    vector<pair<UINT, CComponent*>> vecComps;

    // ������Ʈ ������ ����
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
        if (nullptr == pCom)
            continue;

        vecComps.push_back({i, pCom});
    }

    // ������Ʈ ���� ����
    size_t CompCount = vecComps.size();
    fwrite(&CompCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < CompCount; i++)
    {
        // ������Ʈ�� �̸� ����
        SaveWStringToFile(ToWstring(COMPONENT_TYPE_STRING[vecComps[i].first]), _File);

        // �ش� ������Ʈ�� ������ ������ ����
        UINT MemoryByte = vecComps[i].second->SaveToLevelFile(_File);
        PaddingMemoryBlock(MemoryByte, _File, false);
    }

    // ��ũ��Ʈ ���� ����
    const vector<CScript*>& vecScripts = _Obj->GetScripts();
    size_t ScriptCount = vecScripts.size();

    // ��ũ��Ʈ ���� ����
    fwrite(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        SaveWStringToFile(CScriptMgr::GetScriptName(vecScripts[i]), _File);
        UINT MemoryByte = vecScripts[i]->SaveToLevelFile(_File);
        PaddingMemoryBlock(MemoryByte, _File, false);
    }

    // BoneSocket ����
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

    // �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����
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
    // Level �� �ҷ��� ��� ����
    wstring strLevelPath = CPathMgr::GetContentPath();
    strLevelPath += L"Levels/";
    strLevelPath += _LevelFileName;

    std::filesystem::path filePath = strLevelPath;

    // Ȯ���ڰ� �Էµ��� ���� ���
    if ("" == filePath.extension())
    {
        strLevelPath += Level_extension;
    }
    // Ȯ���ڰ� �߸� �Էµ� ���
    else if (Level_extension != filePath.extension())
    {
        strLevelPath = filePath.replace_extension(Level_extension);
    }

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strLevelPath.c_str(), L"rb");

    if (nullptr == pFile)
    {
        MessageBox(nullptr, L"���� ��ΰ� �ùٸ��� �ʰų�, ������ �������� ����", L"���� �ε� ����", MB_OK);
        return nullptr;
    }

    // ������ �̸� �ε�
    CLevel* pLevel = new CLevel;
    wstring strLevelName;
    LoadWStringFromFile(strLevelName, pFile);
    pLevel->SetName(strLevelName);

    // Layer �ε�
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
    // Layer �� �̸� �ε�
    wstring strLayerName;
    LoadWStringFromFile(strLayerName, _File);
    _Layer->SetName(strLayerName);

    // Layer �� �����ϴ� GameObject ���� �ҷ��´�.
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

    // GameObject �� �̸��� �ε�
    wstring strName;
    LoadWStringFromFile(strName, _File);
    pObject->SetName(strName);

    // ���̾� �ε��� ����
    int layerIdx = 0;
    fread(&layerIdx, sizeof(int), 1, _File);
    pObject->m_iLayerIdx = layerIdx;

    // Active
    bool Active = true;
    fread(&Active, sizeof(bool), 1, _File);
    pObject->m_bActive = Active;

    // ������Ʈ ���� �б�
    size_t CompCount = 0;
    fread(&CompCount, sizeof(size_t), 1, _File);

    // ������Ʈ ������ �ҷ�����
    for (size_t i = 0; i < CompCount; i++)
    {
        wstring CompName;
        LoadWStringFromFile(CompName, _File);
        string CompNameStr = ToString(CompName);
        COMPONENT_TYPE type = COMPONENT_TYPE::END;

        // ���ڿ��� ��ġ�ϴ� Ÿ���� Ž��
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
        case COMPONENT_TYPE::SOUNDLISTENER:
            pComponent = new CSoundListener;
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
            assert(nullptr); // Ÿ�Կ� �´� ������Ʈ�� ���������ʾҴ�
            break;
        }

        // �ش� ������Ʈ�� ������ �����͸� �ε�
        pObject->AddComponent(pComponent);

        UINT MemoryByte = pComponent->LoadFromLevelFile(_File);
        PaddingMemoryBlock(MemoryByte, _File, true);
    }

    // ��ũ��Ʈ ���� �б�
    size_t ScriptCount = 0;
    fread(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        wstring strScriptName;
        LoadWStringFromFile(strScriptName, _File);

        CScript* pScript = CScriptMgr::GetScript(strScriptName);
        pObject->AddComponent(pScript);
        UINT MemoryByte = pScript->LoadFromLevelFile(_File);
        PaddingMemoryBlock(MemoryByte, _File, true);
    }

    // BoneSocket �ε�
    int BoneIndex = -1;
    wstring BoneSocketName;
    fread(&BoneIndex, sizeof(int), 1, _File);
    LoadWStringFromFile(BoneSocketName, _File);

    if (nullptr != _ParentObj && -1 != BoneIndex)
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

    // �ڽ� ������Ʈ�� ������ �ڽ� ������Ʈ ���� ����
    size_t childcount = 0;
    fread(&childcount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < childcount; ++i)
    {
        pObject->AddChild(LoadGameObject(pObject, _File));
    }

    return pObject;
}

void CLevelSaveLoad::PaddingMemoryBlock(UINT MemoryByte, FILE* _File, bool _bRead)
{
    // ������Ʈ/��ũ��Ʈ�� �޸𸮰� ������ �޸� ��Ϻ��� ū ���
    if (MemoryByte > MemoryBlockSize)
    {
        MessageBox(nullptr, L"Memory Overflow!!", L"MemoryBlock Padding ����", MB_ICONHAND);
        return;
    }

    // ����� �����Ѱ�� Padding X
    if (MemoryByte == MemoryBlockSize)
        return;

    // 0������ �е�
    UINT PaddingByte = MemoryBlockSize - MemoryByte;
    vector<char> padding(PaddingByte, 0);

    if (_bRead)
    {
        fread(padding.data(), padding.size(), 1, _File);
    }
    else
    {
        fwrite(padding.data(), padding.size(), 1, _File);
    }
}
