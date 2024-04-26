#pragma once

#include <physx\\PxPhysicsAPI.h>
#include <physx\\PxSimulationEventCallback.h>

#ifndef PX_RELEASE
    #define PX_RELEASE(x)                                                                                                                            \
        if (x)                                                                                                                                       \
        {                                                                                                                                            \
            x->release();                                                                                                                            \
            x = NULL;                                                                                                                                \
        }

#endif // PX_RELEASE

#ifndef PVD_HOST
    #define PVD_HOST "127.0.0.1"
#endif // PVD_HOST

// 충돌 콜백 클래스
class CCollisionCallback : public physx::PxSimulationEventCallback
{
    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

    // Not Use
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override{};
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override{};
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override{};
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override{};
};

class CGameObject;

class CPhysicsMgr : public CSingleton<CPhysicsMgr>
{
    SINGLE(CPhysicsMgr);

private:
    physx::PxDefaultAllocator m_Allocator;
    physx::PxDefaultErrorCallback m_ErrorCallback;
    physx::PxFoundation* m_Foundation;
    physx::PxPhysics* m_Physics;
    physx::PxDefaultCpuDispatcher* m_Dispatcher;
    physx::PxScene* m_Scene;
    physx::PxPvd* m_Pvd;

    vector<CGameObject*> m_vecPhysicsObj;
    CCollisionCallback m_CallbackInst;
    WORD m_Matrix[LAYER_MAX];

    float m_Accumulator;
    const float m_StepSize;

public:
    void tick();

    void OnPhysicsStart();
    void OnPhysicsStop();

public:
    void LayerCheck(UINT _LeftLayer, UINT _RightLayer, bool _bCheck = true);
    void LayerCheck(CLevel* _CurLevel, const wstring& _LeftLayer, const wstring& _RightLayer);

    void EnableAllLayer();
    void DisableAllLayer();

    WORD GetCollisionLayer(UINT idx) const { return m_Matrix[idx]; }
    void SetCollisionLayer(UINT idx, UINT row) { m_Matrix[idx] = row; }

private:
    void AddPhysicsObject(CGameObject* _GameObject);
    void RemovePhysicsObject(CGameObject* _GameObject);

    friend class CTaskMgr;
};
