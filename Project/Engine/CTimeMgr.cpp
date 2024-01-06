#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"

CTimeMgr::CTimeMgr()
    : m_Frequency{}
    , m_PrevCount{}
    , m_CurCount{}
    , m_DeltaTime(0.f)
    , m_iCall(0)
    , m_iFPS(0)
    , m_fAccTime(0.f)
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
    // 초당 빈도
    QueryPerformanceFrequency(&m_Frequency);

    QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::tick()
{
    QueryPerformanceCounter(&m_CurCount);

    m_DeltaTime = float(m_CurCount.QuadPart - m_PrevCount.QuadPart) / float(m_Frequency.QuadPart);

    m_PrevCount = m_CurCount;

    // DT 보정
    if ((1.f / 60.f) < m_DeltaTime)
        m_DeltaTime = (1.f / 60.f);

    // 시간 누적 ==> 1초마다 if 구문 실행
    m_fAccTime += m_DeltaTime;
    if (1.f <= m_fAccTime)
    {
        //wchar_t szText[50] = {};
        //swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
        //SetWindowText(CEngine::GetInst()->GetMainWind(), szText);
        m_iFPS = m_iCall;
        m_iCall = 0;
        m_fAccTime = 0.f;
    }

    ++m_iCall;
}