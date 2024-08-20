#include "pch.h"
#include "CElfilisD_ResistSuccess.h"
#include "CElfilisFSM.h"

CElfilisD_ResistSuccess::CElfilisD_ResistSuccess()
{
}

CElfilisD_ResistSuccess::~CElfilisD_ResistSuccess()
{
}

void CElfilisD_ResistSuccess::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisD_ResistSuccess::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ResistSuccess"), false, false, 1.5f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Roar"), false, false, 1.5f, 0.3f);
    }
    break;
    }
}

void CElfilisD_ResistSuccess::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisD_ResistSuccess::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisD_ResistSuccess::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundMove);
    }
}