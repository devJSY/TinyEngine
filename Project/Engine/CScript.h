#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h"

#include "CGameObject.h"
#include "components.h"

class CScript : public CComponent
{
private:
    UINT m_iScriptType;

public:
    void Destroy();

public:
    virtual void finaltick() final{};

public:
    CScript();
    virtual ~CScript();
};
