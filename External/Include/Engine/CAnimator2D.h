#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim;

class CAnimator2D : public CComponent
{
private:
    map<wstring, CAnim*> m_mapAnim;
    CAnim* m_CurAnim;
    bool m_bRepeat;

public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetPos,
                Vec2 _Background, int _FrmCount, float _FPS, bool _UseBackGround = false);

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat);

public:
    const map<wstring, CAnim*>& GetmapAnim() const { return m_mapAnim; }
    CAnim* GetCurAnim() const { return m_CurAnim; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CAnimator2D();
    virtual ~CAnimator2D();
};
