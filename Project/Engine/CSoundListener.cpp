#include "pch.h"
#include "CSoundListener.h"
#include "CTransform.h"

CSoundListener::CSoundListener()
    : CComponent(COMPONENT_TYPE::SOUNDLISTENER)
{
}

CSoundListener::~CSoundListener()
{
}

void CSoundListener::finaltick()
{
    Vec3 WorldPos = Transform()->GetWorldPos();
    Vec3 WorldFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 WorldUp = Transform()->GetWorldDir(DIR_TYPE::UP);

    FMOD_VECTOR listenerPos = {WorldPos.x, WorldPos.y, WorldPos.z};
    FMOD_VECTOR listenerVel = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR forward = {WorldFront.x, WorldFront.y, WorldFront.z};
    FMOD_VECTOR up = {WorldUp.x, WorldUp.y, WorldUp.z};

    CSound::g_pFMOD->set3DListenerAttributes(0, &listenerPos, &listenerVel, &forward, &up);
}

UINT CSoundListener::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CSoundListener::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}
