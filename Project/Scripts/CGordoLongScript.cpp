#include "pch.h"
#include "CGordoLongScript.h"

CGordoLongScript::CGordoLongScript()
    : CMonsterUnitScript(GORDOLONGSCRIPT)
{
}

CGordoLongScript::~CGordoLongScript()
{
}

void CGordoLongScript::begin()
{
    Animator()->Play(ANIMPREFIX(L"Wait"));
}

void CGordoLongScript::tick()
{
    float Angle = Transform()->GetLocalRotation().y;
    Angle += DT * GetCurInfo().RotationSpeed;
    Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Angle);
    Transform()->SetWorldRotation(Quaternion);
}

void CGordoLongScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CGordoLongScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}