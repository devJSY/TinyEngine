#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CAnim::CAnim()
    : m_Animator(nullptr)
    , m_CurFrmIdx(0)
    , m_bFinish(false)
    , m_fAccTime(0.f)
    , m_bUseBackGround(false)
{
}

CAnim::~CAnim()
{
}

void CAnim::finaltick()
{
    if (m_bFinish)
        return;

    m_fAccTime += DT;

    if (m_vecFrm[m_CurFrmIdx].Duration < m_fAccTime)
    {
        ++m_CurFrmIdx;
        if (m_vecFrm.size() <= m_CurFrmIdx)
        {
            m_CurFrmIdx = 0;
            m_bFinish = true;
        }
        m_fAccTime = 0.f;
    }
}

void CAnim::UpdateData()
{
    // 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
    tAnimData2D data = {};
    data.UseAnim2D = 1;
    data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
    data.vSliceSize = m_vecFrm[m_CurFrmIdx].vSlice;
    data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;
    data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
    data.UseBackGround = m_bUseBackGround;

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
    data.UseBackGround = 0;

    pCB->SetData(&data);
    pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset,
                   Vec2 _vBackground, int _FrmCount, float _FPS, bool _UseBackGround)
{
    m_Animator = _Animator;
    m_AtlasTex = _Atlas;
    m_bUseBackGround = _UseBackGround;

    Vec2 AtlasSize = Vec2((float)_Atlas->GetWidth(), (float)_Atlas->GetHeight());

    for (int i = 0; i < _FrmCount; ++i)
    {
        tAnimFrm frm = {};

        frm.vSlice = Vec2(_vSliceSize.x, _vSliceSize.y) / AtlasSize;

        frm.vLeftTop = Vec2(_vLeftTop.x, _vLeftTop.y) / AtlasSize;
        frm.vLeftTop.x += frm.vSlice.x * i;

        frm.vOffset = Vec2(_vOffset.x, _vOffset.y) / AtlasSize;
        frm.vBackground = Vec2(_vBackground.x, _vBackground.y) / AtlasSize;
        frm.Duration = 1.f / _FPS;

        m_vecFrm.push_back(frm);
    }
}

void CAnim::SaveToLevelFile(FILE* _File)
{
}

void CAnim::LoadFromLevelFile(FILE* _File)
{
}
