#include "pch.h"
#include "CTestIdle.h"

CTestIdle::CTestIdle()
{
}

CTestIdle::~CTestIdle()
{
}

void CTestIdle::tick()
{

	if (KEY_PRESSED(KEY::UP))
	{
		ChangeState(L"FORWARD");
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		ChangeState(L"RIGHT");
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		ChangeState(L"LEFT");
	}
}

void CTestIdle::Enter()
{
}

void CTestIdle::Exit()
{
}
