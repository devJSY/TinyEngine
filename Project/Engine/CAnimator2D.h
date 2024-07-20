#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim2D;

class CAnimator2D : public CComponent
{
private:
    map<wstring, CAnim2D*> m_mapAnim;
    CAnim2D* m_CurAnim;
    bool m_bRepeat;

public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetPos, Vec2 _Background, int _FrmCount,
                float _FPS, bool _UseBackGround = false);

    CAnim2D* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);
    bool IsFinish() const;

private:
    bool DeleteAnim(const wstring& _strAnimName);

public:
    const map<wstring, CAnim2D*>& GetmapAnim() const { return m_mapAnim; }
    CAnim2D* GetCurAnim() const { return m_CurAnim; }

public:
    void SaveAnimations(const wstring& _strRelativePath);
    void LoadAnimation(const wstring& _strRelativePath);

    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& origin);
    virtual ~CAnimator2D();

    friend class COutliner;
};
