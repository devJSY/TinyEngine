#include "pch.h"
#include "CEngineTestScript.h"
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSoundMgr.h>

CEngineTestScript::CEngineTestScript()
    : CScript(ENGINETESTSCRIPT)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam1, "Radius");
    AddScriptParam(SCRIPT_PARAM::INT, &TestParam2, "ChildCount");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam3, "Degree");
}

CEngineTestScript::CEngineTestScript(const CEngineTestScript& origin)
    : CScript(origin)
    , TestParam1(origin.TestParam1)
    , TestParam2(origin.TestParam2)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam1, "Radius");
    AddScriptParam(SCRIPT_PARAM::INT, &TestParam2, "ChildCount");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &TestParam3, "Degree");
}

void CEngineTestScript::begin()
{
   
}

void CEngineTestScript::tick()
{
    makeHeart();
}

void CEngineTestScript::makeHeart()
{
    // 2차원 하트 방정식
    // x = 16 * sin^3Θ
    // y = 13 * cos(Θ) * -5cos(2Θ) * - 2cos(3Θ) - cos(4Θ)
    const int childCount = 100;
    const float Radius = 1.0f;
    static vector<CGameObject*> childList;
    if (childList.empty())
    {
        childList.reserve(childCount);
        for (int index = 0; index < childCount; ++index)
        {
            childList.push_back(new CGameObject);
            childList[index]->SetName(L"Child Object");
            childList[index]->AddComponent(new CTransform);
            childList[index]->AddComponent(new CMeshRender);

            childList[index]->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
            childList[index]->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material/2DTestMtrl.mtrl"), 0);

            GamePlayStatic::SpawnGameObject(childList[index], 0);
        }
    }

    for (int index = 0; index < childCount; ++index)
    {
        const float Theta = XM_2PI / childCount * index;
        Vec3 pos;
        const float sin = sinf(Theta);
        const float cos = cosf(Theta);
        const float cos2 = cosf(Theta * 2.f);
        const float cos3 = cosf(Theta * 3.f);
        const float cos4 = cosf(Theta * 4.f);

        pos.x = 16.f * sin * sin * sin;
        pos.y = 13.f * cos - 5 * cos2 - 2 * cos3 - cos4;
        pos.z = 0;

        childList[index]->Transform()->SetLocalPos(pos);
    }

    // Scale
    const float minScale = 1.f;
    const float maxScale = 2.f;
    static float elapsedTime = 0.f;
    elapsedTime += CTimeMgr::GetInst()->GetDeltaTime();
    const float duration = sinf(elapsedTime) + 1.f;
    const float currentScale = Lerp(minScale, maxScale, duration);

    // Rotate
    const float rotateRadian = TestParam3 * XM_PI / 180.f;

    for (CGameObject* child : childList)
    {
        Vec3 pos = child->Transform()->GetLocalPos();

        // Scale
        pos.x *= currentScale;
        pos.y *= currentScale;

        // Rotate
        Vec3 prevPos = pos;
        pos.x = cosf(rotateRadian) * prevPos.x - sinf(rotateRadian) * prevPos.y;
        pos.y = sinf(rotateRadian) * prevPos.x + cosf(rotateRadian) * prevPos.y;

        pos += Transform()->GetWorldPos();

        child->Transform()->SetLocalPos(pos);
    }
}

UINT CEngineTestScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    fwrite(&TestParam1, sizeof(float), 1, _File);
    fwrite(&TestParam2, sizeof(int), 1, _File);
    fwrite(&TestParam3, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CEngineTestScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    fread(&TestParam1, sizeof(float), 1, _File);
    fread(&TestParam2, sizeof(int), 1, _File);
    fread(&TestParam3, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
