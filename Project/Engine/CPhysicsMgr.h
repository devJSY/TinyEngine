#pragma once

#include <physx\\PxPhysicsAPI.h>
#include <physx\\PxSimulationEventCallback.h>

class CGameObject;

// RayCast
struct RaycastHit
{
    Vec3 Centroid;              // Raycast 중심 위치
    float Distance;             // Centroid 에서 부터 Ray를 발사하여 충돌한 지점까지의 거리
    Vec3 Normal;                // 충돌 표면의 노말벡터
    Vec3 Point;                 // 충돌 지점의 위치
    CGameObject* pCollisionObj; // 충돌한 콜라이더 오브젝트
};

struct ControllerColliderHit
{
    class CCollider* Collider;
    class CCharacterController* Controller;
    Vec3 MoveDirection;
    float MoveLength;
    Vec3 Normal;
    Vec3 Point;
};

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

// 캐릭터 컨트롤러 콜백 클래스
class CCTCCollisionCallback : public physx::PxUserControllerHitReport
{
    virtual void onShapeHit(const physx::PxControllerShapeHit& hit) override;

    virtual void onControllerHit(const physx::PxControllersHit& hit) override{};
    virtual void onObstacleHit(const physx::PxControllerObstacleHit& hit) override{};
};

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

class CPhysicsMgr : public CSingleton<CPhysicsMgr>
{
    SINGLE(CPhysicsMgr);

private:
    // PhysX
    physx::PxDefaultAllocator m_Allocator;
    physx::PxDefaultErrorCallback m_ErrorCallback;
    physx::PxFoundation* m_Foundation;
    physx::PxPhysics* m_Physics;
    physx::PxDefaultCpuDispatcher* m_Dispatcher;
    physx::PxScene* m_Scene;
    physx::PxPvd* m_Pvd;
    physx::PxControllerManager* m_ControllerMgr;

private:
    vector<CGameObject*> m_vecPhysicsObj;
    CCollisionCallback m_CallbackInst;
    CCTCCollisionCallback m_CCTCallbackInst;
    WORD m_Matrix[LAYER_MAX];

    float m_Accumulator;    // DT 누적량
    const float m_StepSize; // 시뮬레이션 타임 스탭

    Vec3 m_Gravity;
    float m_PPM; // PixelPerMeter

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

    RaycastHit RayCast(Vec3 _Origin, Vec3 _Direction, float _Distance, WORD _LayerMask = 0xFFFF);
    RaycastHit RayCast(Vec3 _Origin, Vec3 _Direction, float _Distance, const vector<wstring>& _LayerNames);

public:
    Vec3 GetGravity() const { return m_Gravity; }
    void SetGravity(Vec3 _Gravity) { m_Gravity = _Gravity; }

    float GetPPM() const { return m_PPM; }
    void SetPPM(float _PPM) { m_PPM = _PPM; }

private:
    void AddPhysicsObject(CGameObject* _GameObject);
    void AddCharacterControllerObject(CGameObject* _GameObject);
    void RemovePhysicsObject(CGameObject* _GameObject);

    friend class CTaskMgr;
};
