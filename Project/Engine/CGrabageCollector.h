#pragma once
#include "singleton.h"

class CEntity;

class CGrabageCollector : public CSingleton<CGrabageCollector>
{
    SINGLE(CGrabageCollector);

private:
    vector<CEntity*> m_vecEntity;
    float m_AccTime;

public:
    void tick();

    void Add(CEntity* _Entity) { m_vecEntity.push_back(_Entity); }
    void Clear();
};
