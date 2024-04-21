#pragma once
#include "CEntity.h"

#include "CTexture.h"

struct tAnimFrm
{
    Vec2 vLeftTop;
    Vec2 vSlice;
    Vec2 vOffset;
    Vec2 vBackground;
    float Duration;
};

class CAnimator2D;

class CAnim : public CEntity
{
private:
    CAnimator2D* m_Animator;

    vector<tAnimFrm> m_vecFrm;
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
    const vector<tAnimFrm>& GetVecFrm() const { return m_vecFrm; }
    int GetCurFrmIdx() const { return m_CurFrmIdx; }
    bool IsUseBackGround() const { return m_bUseBackGround; }

public:
    bool SaveAnim(const wstring& _FilePath);
    bool LoadAnim(const wstring& _FilePath);

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);

    CLONE(CAnim);

public:
    CAnim();
    CAnim(const CAnim& origin);
    virtual ~CAnim();

    friend class CAnimator2D;
    friend class CSpriteEditor;
    friend class COutliner;
};
