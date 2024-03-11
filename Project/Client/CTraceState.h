#pragma once
#include <Engine\\CState.h>

class CTraceState : public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CTraceState();
    virtual ~CTraceState();
};
