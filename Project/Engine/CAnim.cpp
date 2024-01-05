#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CAnim::CAnim()
    : m_Animator(nullptr)
    , m_CurFrmIdx(0)
    , m_bFinish(false)
    , m_AccTime(0.f)
{
}

CAnim::~CAnim()
{
}

void CAnim::finaltick()
{
    m_AccTime += DT;

    if (m_vecFrm[m_CurFrmIdx].Duration < m_AccTime)
    {
        ++m_CurFrmIdx;
        if (m_vecFrm.size() <= m_CurFrmIdx)
        {
            m_CurFrmIdx = 0;
        }
        m_AccTime = 0.f;
    }
}

void CAnim::UpdateData()
{
    // 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 1;
    data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
    data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
    data.vSlizeSize = m_vecFrm[m_CurFrmIdx].vSlice;

    pCB->SetData(&data);
    pCB->UpdateData();

    // 아틀라스 텍스쳐 t10 에 바인딩
    m_AtlasTex->UpdateData(10);
}

void CAnim::Clear()
{
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 0;

    pCB->SetData(&data);
    pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset,
                   int _FrmCount, float _FPS)
{
    m_Animator = _Animator;
    m_AtlasTex = _Atlas;

    for (int i = 0; i < _FrmCount; ++i)
    {
        tAnimFrm frm = {};

        frm.vSlice = Vec2(_vSliceSize.x / (float)_Atlas->GetWidth(), _vSliceSize.y / (float)_Atlas->GetHeight());

        frm.vLeftTop =
            Vec2(_vLeftTop.x / (float)_Atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_Atlas->GetHeight());

        frm.vOffset = Vec2(_vOffset.x / (float)_Atlas->GetWidth(), _vOffset.y / (float)_Atlas->GetHeight());
        frm.Duration = 1.f / _FPS;

        m_vecFrm.push_back(frm);
    }
}
