#include "pch.h"
#include <Engine\\CScript.h>
#include "CElfilisLaser.h"

CElfilisLaser::CElfilisLaser()
    : CScript(ELFILISLASER)
    , m_Step(0)
{
}

CElfilisLaser::~CElfilisLaser()
{
}


void CElfilisLaser::tick()
{
}

void CElfilisLaser::SetStart()
{
    Animator()->Play(ANIMPREFIX("Start"), false);
}

void CElfilisLaser::SetWait()
{
    Animator()->Play(ANIMPREFIX("Loop"));
}

void CElfilisLaser::SetEnd()
{
    Animator()->Play(ANIMPREFIX("End"), false);
}

UINT CElfilisLaser::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisLaser::LoadFromLevelFile(FILE* _File)
{
    return 0;
}