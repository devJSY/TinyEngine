#pragma once
#include "CEntity.h"

enum class BLACKBOARD_DATA
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OBJECT,
};

struct tBlackboardData
{
    BLACKBOARD_DATA Type;
    void* pData;
};

class CBlackboard : public CEntity
{
private:
    map<wstring, tBlackboardData> m_mapBBData;

public:
    void AddBlackboardData(const wstring& _strKey, BLACKBOARD_DATA _Type, void* _Data);
    void* GetBlackboardData(const wstring& _strKey);

public:
    CLONE(CBlackboard);

public:
    CBlackboard();
    CBlackboard(const CBlackboard& origin);
    virtual ~CBlackboard();
};
