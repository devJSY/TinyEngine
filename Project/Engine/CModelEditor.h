#pragma once
#include "CEditor.h"

class CGameObjectEx;

class CModelEditor : public CEditor
{
private:
    CGameObjectEx* m_ModelObj;
    int m_SelectedBoneIdx;
    vector<Matrix> m_FinalBoneMat;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawViewport();
    void DrawDetails();
    void DrawSkeletonTree();
    void SkeletonRe(const vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag);
    void DrawAnimation();

private:
    void ReleaseModel();

public:
    CModelEditor();
    virtual ~CModelEditor();
};
