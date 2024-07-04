#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CMesh;
class CStructuredBuffer;

class CAnimator : public CComponent
{
private:
    Ptr<CMesh> m_SkeletalMesh;

    map<wstring, int> m_mapClip; // Key, Clip Index

    int m_CurClipIdx; // 클립 인덱스
    vector<double> m_vecClipUpdateTime;

    bool m_bPlay;      // 애니메이션 재생 여부
    bool m_bRepeat;    // 반복 재생 여부
    float m_PlaySpeed; // 재생 속도

    double m_FrameRate; // Animation FPS
    double m_CurTime;

    int m_FrameIdx;     // 클립의 현재 프레임
    int m_NextFrameIdx; // 클립의 다음 프레임
    float m_Ratio;      // 프레임 사이 비율

    CStructuredBuffer* m_BoneFinalMatBuffer; // 특정 프레임의 최종 행렬
    bool m_bFinalMatUpdate;                  // 최종행렬 연산 수행여부

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    void finaltick_ModelEditor();

    void ClearData();

public:
    int FindClipIndex(const wstring& _strClipName);
    void Play(const wstring& _strClipName, bool _bRepeat = true, float _PlaySpeed = 2.f);
    bool IsFinish() const;

    bool IsVaild();

public:
    Ptr<CMesh> GetSkeletalMesh() const { return m_SkeletalMesh; }
    void SetSkeletalMesh(Ptr<CMesh> _SkeletalMesh);

    int GetCurClipIdx() const { return m_CurClipIdx; }
    void SetCurClipIdx(int _Idx) { m_CurClipIdx = _Idx; }

    const vector<double>& GetClipTime() const { return m_vecClipUpdateTime; }

    bool IsPlaying() const { return m_bPlay; }
    void SetPlay(bool _bPlay) { m_bPlay = _bPlay; }

    bool IsRepeat() const { return m_bRepeat; }
    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }

    float GetPlaySpeed() const { return m_PlaySpeed; }
    void SetPlaySpeed(float _Speed) { m_PlaySpeed = _Speed; }

    double GetFrameRate() const { return m_FrameRate; }

    int GetCurFrameIdx() const { return m_FrameIdx; }
    void SetFrameIdx(int _FrameIdx);

    CStructuredBuffer* GetFinalBoneMat() const { return m_BoneFinalMatBuffer; }
    UINT GetBoneCount() const;

private:
    void CheckBoneFinalMatBuffer();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CAnimator);

public:
    CAnimator();
    CAnimator(const CAnimator& _origin);
    virtual ~CAnimator();
};