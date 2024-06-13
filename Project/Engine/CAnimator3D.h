#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
    const vector<tMTBone>* m_pVecBones;
    const vector<tMTAnimClip>* m_pVecClip;

    vector<float> m_vecClipUpdateTime;
    vector<Matrix> m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
    int m_iFrameCount;                // 30
    double m_dCurTime;
    int m_iCurClip; // 클립 인덱스

    int m_iFrameIdx;     // 클립의 현재 프레임
    int m_iNextFrameIdx; // 클립의 다음 프레임
    float m_fRatio;      // 프레임 사이 비율

    CStructuredBuffer* m_pBoneFinalMatBuffer; // 특정 프레임의 최종 행렬
    bool m_bFinalMatUpdate;                   // 최종행렬 연산 수행여부

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetBones(const vector<tMTBone>* _vecBones)
    {
        m_pVecBones = _vecBones;
        m_vecFinalBoneMat.resize(m_pVecBones->size());
    }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

    CStructuredBuffer* GetFinalBoneMat() const { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() const { return (UINT)m_pVecBones->size(); }
    void ClearData();

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    virtual ~CAnimator3D();
};