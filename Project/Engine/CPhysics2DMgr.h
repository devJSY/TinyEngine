#pragma once

class b2World;
class CGameObject;

class CPhysics2DMgr : public CSingleton<CPhysics2DMgr>
{
    SINGLE(CPhysics2DMgr);

private:
    b2World* m_PhysicsWorld;
    vector<CGameObject*> m_vecPhysicsObj;

    unsigned short m_Matrix[LAYER_MAX];

public:
    void tick();

    void OnPhysics2DStart();
    void OnPhysics2DStop();

public:
    void LayerCheck(UINT _LeftLayer, UINT _RightLayer, bool _bCheck = true);
    void LayerCheck(CLevel* _CurLevel, const wstring& _LeftLayer, const wstring& _RightLayer);

    void EnableAllLayer();
    void DisableAllLayer();

    unsigned short GetCollisionLayer(UINT idx) const { return m_Matrix[idx]; }
    void SetCollisionLayer(UINT idx, UINT row) { m_Matrix[idx] = row; }
};
