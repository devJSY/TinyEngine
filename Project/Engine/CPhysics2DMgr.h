#pragma once

class b2World;
class CGameObject;

class CPhysics2DMgr : public CSingleton<CPhysics2DMgr>
{
    SINGLE(CPhysics2DMgr);

private:
    b2World* m_PhysicsWorld;
    vector<CGameObject*> m_vecPhysicsObj;

public:
    void tick();

    void OnPhysics2DStart();
    void OnPhysics2DStop();
};
