#pragma once
#include "CEntity.h"

#include "CTexture.h"

struct tAnim2DFrm
{
    Vec2 vLeftTop;
    Vec2 vSlice;
    Vec2 vOffset;
    Vec2 vBackground;
    float Duration;
};

class CAnimator2D;

class CAnim2D : public CEntity
{
private:
    CAnimator2D* m_Animator;

    vector<tAnim2DFrm> m_vecFrm;
    int m_CurFrmIdx;
    bool m_bFinish;

    Ptr<CTexture> m_AtlasTex;

    float m_fAccTime;

    bool m_bUseBackGround;

public:
    void finaltick();
    void UpdateData();
    bool IsFinish() const { return m_bFinish; }
    void Reset()
    {
        m_bFinish = false;
        m_CurFrmIdx = 0;
        m_fAccTime = 0.f;
    }
    static void Clear();
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount,
                float _FPS, bool _UseBackGround);

public:
    Ptr<CTexture> GetAtlasTex() const { return m_AtlasTex; }
    const vector<tAnim2DFrm>& GetVecFrm() const { return m_vecFrm; }
    int GetCurFrmIdx() const { return m_CurFrmIdx; }
    bool IsUseBackGround() const { return m_bUseBackGround; }

public:
    bool SaveAnim(const wstring& _FilePath);
    bool LoadAnim(const wstring& _FilePath);

    UINT SaveToLevelFile(FILE* _File);
    UINT LoadFromLevelFile(FILE* _File);

    CLONE(CAnim2D);

public:
    CAnim2D();
    CAnim2D(const CAnim2D& origin);
    virtual ~CAnim2D();

    friend class CAnimator2D;
    friend class CSpriteEditor;
    friend class COutliner;
};
