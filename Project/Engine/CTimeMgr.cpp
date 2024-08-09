#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"

CTimeMgr::CTimeMgr()
    : m_Frequency{}
    , m_PrevCount{}
    , m_CurCount{}
    , m_DeltaTime(0.f)
    , m_EngineDeltaTime(0.f)
    , m_iCall(0)
    , m_iFPS(0)
    , m_fAccTime(0.f)
    , m_bLock(true)
    , m_TimeScale(1.f)
    , m_TimeScaleDuration(0.f)
    , m_TimeScaling(false)
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

    m_EngineDeltaTime = m_DeltaTime = float(m_CurCount.QuadPart - m_PrevCount.QuadPart) / float(m_Frequency.QuadPart);

    if (m_bLock)
        m_DeltaTime = 0.f;

    m_PrevCount = m_CurCount;

    // 시간 누적 ==> 1초마다 if 구문 실행
    m_fAccTime += m_EngineDeltaTime;
    if (1.f <= m_fAccTime)
    {
        // wchar_t szText[50] = {};
        // swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
        // SetWindowText(CEngine::GetInst()->GetMainWind(), szText);
        m_iFPS = m_iCall;
        m_iCall = 0;
        m_fAccTime = 0.f;
    }

    ++m_iCall;

    // TimeScale 적용
    if (m_TimeScaling)
    {
        m_TimeScaleDuration -= m_EngineDeltaTime;

        // 지속시간이 종료된경우 TimeScale 원상복귀
        if (m_TimeScaleDuration < 0.f)
        {
            m_TimeScaling = false;
            m_TimeScale = 1.f;
        }
    }

    m_DeltaTime *= m_TimeScale;

    g_Global.g_DT = m_DeltaTime;
    g_Global.g_Time += m_DeltaTime;
    g_Global.g_EngineDT = m_EngineDeltaTime;
    g_Global.g_EngineTime += m_EngineDeltaTime;
}

void CTimeMgr::SetTimeScale(float _Duration, float _TimeScale)
{
    m_TimeScaleDuration = _Duration;
    m_TimeScale = _TimeScale;
    m_TimeScaling = true;
}
