#include "pch.h"
#include "CKirbySoundListener.h"

CKirbySoundListener::CKirbySoundListener()
    : CScript(KIRBYSOUNDLISTENER)
{
}

CKirbySoundListener::CKirbySoundListener(const CKirbySoundListener& _Origin)
    : CScript(KIRBYSOUNDLISTENER)
{
}

CKirbySoundListener::~CKirbySoundListener()
{
}

void CKirbySoundListener::begin()
{
    m_MainCamera = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");
}

void CKirbySoundListener::tick()
{
    if (m_MainCamera == nullptr)
        return;

    Vec3 Dir = m_MainCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Dir.y = 0.f;
    Dir.Normalize();

    Transform()->SetDirection(Dir);
}

UINT CKirbySoundListener::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CKirbySoundListener::LoadFromLevelFile(FILE* _File)
{
    return 0;
}
