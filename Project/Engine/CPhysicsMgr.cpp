#include "pch.h"
#include "CPhysicsMgr.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"

using namespace physx;

static physx::PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
                                                      PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags,
                                                      const void* constantBlock, PxU32 constantBlockSize)
{
    // 트리거 플래그 등록
    if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_CCD;
        return PxFilterFlag::eDEFAULT;
    }

    // 충돌 시작, 충돌 중, 충돌 끝 플래그 등록
    if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
        pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
                    PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_TOUCH_CCD;

    return PxFilterFlag::eDEFAULT;
}

void CCollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    CCollider* pColliderA = (CCollider*)pairs->shapes[0]->userData;
    CCollider* pColliderB = (CCollider*)pairs->shapes[1]->userData;

    if (pairs->events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND))
    {
        pColliderA->OnCollisionEnter(pColliderB);
        pColliderB->OnCollisionEnter(pColliderA);
    }

    if (pairs->events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST))
    {
        pColliderA->OnCollisionExit(pColliderB);
        pColliderB->OnCollisionExit(pColliderA);
    }

    // 충돌 카운트가 남아있는 경우 Stay 상태
    if (pColliderA->m_CollisionCount > 0)
        pColliderA->OnCollisionStay(pColliderB);
    if (pColliderB->m_CollisionCount > 0)
        pColliderB->OnCollisionStay(pColliderA);
}

void CCollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    CCollider* pColliderA = (CCollider*)pairs->triggerShape->userData;
    CCollider* pColliderB = (CCollider*)pairs->otherShape->userData;

    if (PxPairFlag::eNOTIFY_TOUCH_FOUND & pairs->status)
    {
        pColliderA->OnTriggerEnter(pColliderB);
        pColliderB->OnTriggerEnter(pColliderA);

        // Trigger List 에 추가
        CPhysicsMgr::GetInst()->m_listTrigger.push_back(make_pair(pColliderA, pColliderB));
    }

    if (PxPairFlag::eNOTIFY_TOUCH_LOST & pairs->status)
    {
        pColliderA->OnTriggerExit(pColliderB);
        pColliderB->OnTriggerExit(pColliderA);
    }
}

void CCTCCollisionCallback::onShapeHit(const physx::PxControllerShapeHit& hit)
{
    float PPM = CPhysicsMgr::GetInst()->GetPPM();

    ControllerColliderHit HitResult;
    HitResult.Collider = (CCollider*)hit.shape->userData;
    HitResult.Controller = (CCharacterController*)hit.controller->getUserData();
    HitResult.MoveDirection = hit.dir;
    HitResult.MoveLength = hit.length * PPM;
    HitResult.Normal = hit.worldNormal;
    HitResult.Point = Vec3((float)hit.worldPos.x, (float)hit.worldPos.y, (float)hit.worldPos.z);
    HitResult.Point *= PPM;

    HitResult.Controller->OnControllerColliderHit(HitResult);
    HitResult.Collider->OnControllerColliderHit(HitResult);
}

CPhysicsMgr::CPhysicsMgr()
    : m_Allocator()
    , m_ErrorCallback()
    , m_Foundation(NULL)
    , m_Physics(NULL)
    , m_Dispatcher(NULL)
    , m_Scene(NULL)
    , m_Pvd(NULL)
    , m_ControllerMgr(NULL)
    , m_vecPhysicsObj{}
    , m_CallbackInst()
    , m_CCTCallbackInst()
    , m_Matrix{}
    , m_Accumulator(0.f)
    , m_StepSize(0.02f) // 1.f / 50.f
    , m_Gravity(Vec3(0.f, -9.81f, 0.f))
    , m_PPM(1.f)
    , m_listTrigger{}
{
    EnableAllLayer();
}

CPhysicsMgr::~CPhysicsMgr()
{
    OnPhysicsStop();
}

void CPhysicsMgr::tick()
{
    if (nullptr == m_Scene || m_vecPhysicsObj.empty())
        return;

    // 시뮬레이션 안정성을 위해 StepSize 단위로 시뮬레이션
    m_Accumulator += DT;
    while (m_Accumulator >= m_StepSize)
    {
        m_Accumulator -= m_StepSize;

        // 시뮬레이션
        m_Scene->simulate(m_StepSize);
        m_Scene->fetchResults(true);

        // TriggerStay Event
        std::list<std::pair<CCollider*, CCollider*>>::iterator iter = m_listTrigger.begin();
        while (iter != m_listTrigger.end())
        {
            if (0 >= iter->first->m_TriggerCount || 0 >= iter->second->m_TriggerCount)
            {
                iter = m_listTrigger.erase(iter);
                continue;
            }

            iter->first->OnTriggerStay(iter->second);
            iter->second->OnTriggerStay(iter->first);
            ++iter;
        }

        // 시뮬레이션 결과로 트랜스폼 업데이트
        PxU32 nbActors = m_Scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
        std::vector<PxRigidActor*> actors(nbActors);
        m_Scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

        for (PxU32 i = 0; i < nbActors; i++)
        {
            if (!actors[i]->is<PxRigidDynamic>())
                continue;

            // RigidDynamic이 캐릭터 컨트롤러인 경우
            if (nullptr == actors[i]->userData)
                continue;

            CGameObject* obj = (CGameObject*)actors[i]->userData;
            CTransform* pTr = obj->Transform();

            const PxMat44 ActorPose(actors[i]->getGlobalPose());
            Matrix SimulatedMat = Matrix(ActorPose.front());

            // 시뮬레이션 Matrix SRT 분해
            Vec3 Translation, Rotation, Scale;
            ImGuizmo::DecomposeMatrixToComponents(*SimulatedMat.m, Translation, Rotation, Scale);

            // PPM 적용
            Translation *= m_PPM;
            Rotation.ToRadian();

            // 변화량 추출
            Vec3 vPosOffset = pTr->GetWorldPos() - Translation;
            Vec3 vRotOffset = pTr->GetWorldRotation() - Rotation;

            // 변화량만큼 Relative 에 적용
            pTr->SetLocalPos(pTr->GetLocalPos() - vPosOffset);
            pTr->SetLocalRotation(pTr->GetLocalRotation() - vRotOffset);
        }

        // Character Controller
        PxU32 nbControllers = m_ControllerMgr->getNbControllers();
        for (PxU32 i = 0; i < nbControllers; i++)
        {
            PxController* pPxController = m_ControllerMgr->getController(i);
            CCharacterController* pCharacterController = (CCharacterController*)pPxController->getUserData();
            CTransform* pTr = pCharacterController->Transform();

            PxVec3d PxPos = pPxController->getPosition();
            Vec3 vPosOffset = Vec3((float)PxPos.x, (float)PxPos.y, (float)PxPos.z);
            vPosOffset -= pCharacterController->m_Center / m_PPM;
            vPosOffset *= m_PPM;
            vPosOffset = pTr->GetWorldPos() - vPosOffset;

            pTr->SetLocalPos(pTr->GetLocalPos() - vPosOffset);
        }
    }
}

void CPhysicsMgr::OnPhysicsStart()
{
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

    m_Pvd = PxCreatePvd(*m_Foundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);

    PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
    sceneDesc.gravity = m_Gravity;
    m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_Dispatcher;
    sceneDesc.filterShader = contactReportFilterShader;  // 필터 등록
    sceneDesc.simulationEventCallback = &m_CallbackInst; // 충돌 콜백 등록
    sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;         // CCD Activate

    m_Scene = m_Physics->createScene(sceneDesc);

    PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    m_ControllerMgr = PxCreateControllerManager(*m_Scene);

    // 레벨의 모든 오브젝트를 순회하여 Scene 에 추가할 오브젝트를 등록
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        const vector<CGameObject*>& vecParentObj = pCurLevel->GetLayer(i)->GetParentObjects();

        for (const auto& ParentObj : vecParentObj)
        {
            list<CGameObject*> queue;
            queue.push_back(ParentObj);

            while (!queue.empty())
            {
                CGameObject* pObject = queue.front();
                queue.pop_front();

                const vector<CGameObject*>& vecChildObj = pObject->GetChildObject();

                for (size_t i = 0; i < vecChildObj.size(); ++i)
                {
                    queue.push_back(vecChildObj[i]);
                }

                AddPhysicsObject(pObject);
            }
        }
    }
}

void CPhysicsMgr::OnPhysicsStop()
{
    PX_RELEASE(m_ControllerMgr);
    PX_RELEASE(m_Scene);
    PX_RELEASE(m_Dispatcher);
    PX_RELEASE(m_Physics);
    if (m_Pvd)
    {
        PxPvdTransport* transport = m_Pvd->getTransport();
        m_Pvd->release();
        m_Pvd = NULL;
        PX_RELEASE(transport);
    }
    PX_RELEASE(m_Foundation);

    // Physics Object가 보유하고있던 데이터 초기화
    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CRigidbody* pRigidbody = m_vecPhysicsObj[i]->Rigidbody();
        CBoxCollider* pBoxCol = m_vecPhysicsObj[i]->BoxCollider();
        CSphereCollider* pSphereCol = m_vecPhysicsObj[i]->SphereCollider();
        CCapsuleCollider* pCapsuleCol = m_vecPhysicsObj[i]->CapsuleCollider();

        if (nullptr != pRigidbody)
            pRigidbody->m_RuntimeBody = nullptr;
        if (nullptr != pBoxCol)
            pBoxCol->m_RuntimeShape = nullptr;
        if (nullptr != pSphereCol)
            pSphereCol->m_RuntimeShape = nullptr;
        if (nullptr != pCapsuleCol)
            pCapsuleCol->m_RuntimeShape = nullptr;
    }

    m_vecPhysicsObj.clear();
    m_listTrigger.clear();
}

RaycastHit CPhysicsMgr::RayCast(Vec3 _Origin, Vec3 _Direction, float _Distance, WORD _LayerMask)
{
    RaycastHit Hit;
    Hit.Centroid = _Origin / m_PPM;
    Hit.Distance = 0.f;
    Hit.Normal = Vec3();
    Hit.Point = Vec3();
    Hit.pCollisionObj = nullptr;

    bool IsRunning = nullptr != m_Scene;
    if (!IsRunning)
        OnPhysicsStart();

    PxQueryFilterData filterData = PxQueryFilterData();
    filterData.data.word0 = _LayerMask; // 검사할 레이어 체크

    PxRaycastBuffer HitResult;
    bool status = m_Scene->raycast(Hit.Centroid, _Direction.Normalize(), _Distance / m_PPM, HitResult, PxHitFlag::eDEFAULT, filterData);
    if (status)
    {
        PxShape* shape = (PxShape*)HitResult.block.shape;

        Hit.Centroid *= m_PPM;
        Hit.Point = HitResult.block.position * m_PPM;
        Hit.Distance = (Hit.Point - Hit.Centroid).Length();
        Hit.Normal = HitResult.block.normal;
        Hit.pCollisionObj = ((CCollider*)shape->userData)->GetOwner();
    }

    if (!IsRunning)
        OnPhysicsStop();

    return Hit;
}

RaycastHit CPhysicsMgr::RayCast(Vec3 _Origin, Vec3 _Direction, float _Distance, const vector<wstring>& _LayerNames)
{
    WORD LayerMask = 0;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (const wstring& LayerName : _LayerNames)
    {
        CLayer* pCurLayer = pCurLevel->GetLayer(LayerName);

        // 해당이름의 레이어가 존재하지 않는경우
        if (nullptr == pCurLayer)
            continue;

        int LayerIdx = pCurLayer->GetLayerIdx();
        LayerMask |= (1 << LayerIdx);
    }

    if (0 == LayerMask)
        return RaycastHit();

    return RayCast(_Origin, _Direction, _Distance, LayerMask);
}

void CPhysicsMgr::AddPhysicsObject(CGameObject* _GameObject)
{
    if (nullptr == m_Scene || nullptr == _GameObject)
        return;

    // 활성화 여부 체크
    if (!_GameObject->IsActive())
        return;

    AddCharacterControllerObject(_GameObject);

    CTransform* pTr = _GameObject->Transform();
    CRigidbody* pRigidbody = _GameObject->Rigidbody();
    CBoxCollider* pBoxCol = _GameObject->BoxCollider();
    CSphereCollider* pSphereCol = _GameObject->SphereCollider();
    CCapsuleCollider* pCapsuleCol = _GameObject->CapsuleCollider();
    CMeshCollider* pMeshCol = _GameObject->MeshCollider();

    // Physics 관련 컴포넌트가 존재하지않는 경우
    if (nullptr == pRigidbody && nullptr == pBoxCol && nullptr == pSphereCol && nullptr == pCapsuleCol && nullptr == pMeshCol)
        return;

    Vec3 WorldPos = pTr->GetWorldPos();
    Quat WorldQuat = pTr->GetWorldQuaternion();
    Vec3 WorldScale = pTr->GetWorldScale();

    WorldPos /= m_PPM;
    WorldScale /= m_PPM;

    // World Space 기준 위치, 회전상태 적용
    PxTransform PxTr = PxTransform(WorldPos.x, WorldPos.y, WorldPos.z, PxQuat(WorldQuat.x, WorldQuat.y, WorldQuat.z, WorldQuat.w));

    PxRigidActor* RigidActor = nullptr;

    // RigidBody
    if (nullptr != pRigidbody)
    {
        PxRigidDynamic* dynamicRigid = m_Physics->createRigidDynamic(PxTr);

        dynamicRigid->setMass(pRigidbody->m_Mass);
        dynamicRigid->setLinearDamping(pRigidbody->m_Drag);
        dynamicRigid->setAngularDamping(pRigidbody->m_AngularDrag);
        dynamicRigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !pRigidbody->m_bGravity);
        dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, pRigidbody->m_bKinematic);

        switch (pRigidbody->m_CollisionDetectionType)
        {
        case CollisionDetection_TYPE::Discrete: {
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, false);
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
        }
        break;
        case CollisionDetection_TYPE::Continuous: {
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, false);
        }
        break;
        case CollisionDetection_TYPE::ContinuousSpecutive: {
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
            dynamicRigid->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
        }
        break;
        }

        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, pRigidbody->m_FreezePosition[0]);
        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, pRigidbody->m_FreezePosition[1]);
        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, pRigidbody->m_FreezePosition[2]);
        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, pRigidbody->m_FreezeRotation[0]);
        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, pRigidbody->m_FreezeRotation[1]);
        dynamicRigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, pRigidbody->m_FreezeRotation[2]);

        RigidActor = dynamicRigid;
        pRigidbody->m_RuntimeBody = dynamicRigid;
    }
    // 리지드바디 없이 콜라이더만 보유한 경우
    else
    {
        PxRigidStatic* staticRigid = m_Physics->createRigidStatic(PxTr);
        RigidActor = staticRigid;
    }

    int LayerIdx = _GameObject->GetLayerIdx();
    PxFilterData filterData;
    filterData.word0 = (1 << LayerIdx);    // 해당 오브젝트의 레이어 번호
    filterData.word1 = m_Matrix[LayerIdx]; // 필터링을 적용할 테이블

    Ptr<CPhysicMaterial> pDefaultMtrl = CAssetMgr::GetInst()->FindAsset<CPhysicMaterial>(L"Default Material");
    PxMaterial* DefaultPxMtrl =
        m_Physics->createMaterial(pDefaultMtrl->GetStaticFriction(), pDefaultMtrl->GetDynamicFriction(), pDefaultMtrl->GetBounciness());

    // Box Collider
    if (nullptr != pBoxCol && pBoxCol->m_bEnabled)
    {
        // Material 생성
        PxMaterial* pPxMtrl = DefaultPxMtrl;
        if (nullptr != pBoxCol->m_Mtrl)
        {
            pPxMtrl = m_Physics->createMaterial(pBoxCol->m_Mtrl->GetStaticFriction(), pBoxCol->m_Mtrl->GetDynamicFriction(),
                                                pBoxCol->m_Mtrl->GetBounciness());
        }

        // Shape 생성
        PxShape* shape = PxRigidActorExt::createExclusiveShape(*RigidActor, PxBoxGeometry(WorldScale * pBoxCol->m_Size), *pPxMtrl);

        // 콜라이더 Enable / Disable
        shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

        // 트리거
        if (pBoxCol->m_bTrigger)
        {
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }

        // 콜라이더의 상대 위치 적용
        PxTransform LocalPos = shape->getLocalPose();
        LocalPos.p = pBoxCol->GetCenter();
        LocalPos.p /= m_PPM;
        shape->setLocalPose(LocalPos);

        // 필터링 데이터 적용
        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);

        // UserData 등록
        shape->userData = (void*)pBoxCol;
        pBoxCol->m_RuntimeShape = shape;
    }

    // Sphere Collider
    if (nullptr != pSphereCol && pSphereCol->m_bEnabled)
    {
        // Material 생성
        PxMaterial* pPxMtrl = DefaultPxMtrl;
        if (nullptr != pSphereCol->m_Mtrl)
        {
            pPxMtrl = m_Physics->createMaterial(pSphereCol->m_Mtrl->GetStaticFriction(), pSphereCol->m_Mtrl->GetDynamicFriction(),
                                                pSphereCol->m_Mtrl->GetBounciness());
        }

        // Shape 생성
        PxShape* shape = PxRigidActorExt::createExclusiveShape(*RigidActor, PxSphereGeometry(WorldScale.x * pSphereCol->m_Radius * 2.f), *pPxMtrl);

        // 콜라이더 Enable / Disable
        shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

        // 트리거
        if (pSphereCol->IsTrigger())
        {
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }

        // 콜라이더의 상대 위치 적용
        PxTransform LocalPos = shape->getLocalPose();
        LocalPos.p = pSphereCol->GetCenter();
        LocalPos.p /= m_PPM;
        shape->setLocalPose(LocalPos);

        // 필터링 데이터 적용
        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);

        // UserData 등록
        shape->userData = (void*)pSphereCol;
        pSphereCol->m_RuntimeShape = shape;
    }

    // Capsule Collider
    if (nullptr != pCapsuleCol && pCapsuleCol->m_bEnabled)
    {
        PxMaterial* pPxMtrl = DefaultPxMtrl;
        if (nullptr != pCapsuleCol->m_Mtrl)
        {
            pPxMtrl = m_Physics->createMaterial(pCapsuleCol->m_Mtrl->GetStaticFriction(), pCapsuleCol->m_Mtrl->GetDynamicFriction(),
                                                pCapsuleCol->m_Mtrl->GetBounciness());
        }

        // Capsule 기준 축 기준으로 회전 적용
        PxQuat PxRelativeQuat = PxQuat(PxHalfPi, PxVec3(0, 0, 1));
        float RadiusScale = 1.f;
        float HeightScale = 1.f;

        switch (pCapsuleCol->m_Direction)
        {
        case AXIS_TYPE::X: {
            PxRelativeQuat *= PxQuat(PxHalfPi, PxVec3(0, 0, 1));

            RadiusScale = WorldScale.y > WorldScale.z ? WorldScale.y : WorldScale.z;
            HeightScale = WorldScale.x;
        }
        break;
        case AXIS_TYPE::Y: {
            RadiusScale = WorldScale.x > WorldScale.z ? WorldScale.x : WorldScale.z;
            HeightScale = WorldScale.y;
        }
        break;
        case AXIS_TYPE::Z: {
            PxRelativeQuat = PxQuat(PxHalfPi, PxVec3(0, 1, 0));

            RadiusScale = WorldScale.x > WorldScale.y ? WorldScale.x : WorldScale.y;
            HeightScale = WorldScale.z;
        }
        break;
        }

        // Shape 생성
        PxShape* shape = PxRigidActorExt::createExclusiveShape(
            *RigidActor, PxCapsuleGeometry(RadiusScale * pCapsuleCol->m_Radius, HeightScale * (pCapsuleCol->m_Height / 2.f - pCapsuleCol->m_Radius)),
            *pPxMtrl);

        // 콜리이더 Enable / Disable
        shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

        // 트리거
        if (pCapsuleCol->IsTrigger())
        {
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }

        // 콜라이더의 상대 위치 적용
        PxTransform LocalPos = shape->getLocalPose();
        LocalPos.p = pCapsuleCol->GetCenter();
        LocalPos.p /= m_PPM;
        LocalPos.q = LocalPos.q * PxRelativeQuat;
        shape->setLocalPose(LocalPos);

        // 필터링 데이터 적용
        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);

        // UserData 등록
        shape->userData = (void*)pCapsuleCol;
        pCapsuleCol->m_RuntimeShape = shape;
    }

    // Mesh Collider
    if (nullptr != pMeshCol && nullptr != pMeshCol->GetMesh() && pMeshCol->m_bEnabled)
    {
        PxMaterial* pPxMtrl = DefaultPxMtrl;
        if (nullptr != pMeshCol->m_Mtrl)
        {
            pPxMtrl = m_Physics->createMaterial(pMeshCol->m_Mtrl->GetStaticFriction(), pMeshCol->m_Mtrl->GetDynamicFriction(),
                                                pMeshCol->m_Mtrl->GetBounciness());
        }

        Ptr<CMesh> pMesh = pMeshCol->GetMesh();

        PxConvexMeshDesc convexDesc;
        convexDesc.points.count = pMesh->GetVtxCount();
        convexDesc.points.stride = sizeof(Vtx);
        convexDesc.points.data = pMesh->GetVtxSysMem();

        if (pMeshCol->m_bConvex)
            convexDesc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;

        // PxConvexMesh 생성
        PxTolerancesScale scale;
        PxCookingParams params(scale);
        PxDefaultMemoryOutputStream buf;
        PxConvexMeshCookingResult::Enum result;

        // 유효한 메쉬인 경우에만 생성
        if (PxCookConvexMesh(params, convexDesc, buf, &result))
        {
            PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
            PxConvexMesh* convexMesh = m_Physics->createConvexMesh(input);

            PxShape* shape = PxRigidActorExt::createExclusiveShape(*RigidActor, PxConvexMeshGeometry(convexMesh, PxMeshScale(WorldScale)), *pPxMtrl);

            // 콜라이더 Enable / Disable
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

            // 트리거
            if (pMeshCol->m_bTrigger)
            {
                shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
                shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
            }

            // 필터링 데이터 적용
            shape->setSimulationFilterData(filterData);
            shape->setQueryFilterData(filterData);

            // UserData 등록
            shape->userData = (void*)pMeshCol;
            pMeshCol->m_RuntimeShape = shape;
        }
    }

    // 설정한 질량 업데이트
    if (nullptr != pRigidbody)
    {
        PxRigidBody* body = (PxRigidBody*)pRigidbody->m_RuntimeBody;
        PxRigidBodyExt::updateMassAndInertia(*body, body->getMass());
    }

    // Physics Object 추가
    RigidActor->userData = (void*)_GameObject;
    m_Scene->addActor(*RigidActor);
    m_vecPhysicsObj.push_back(_GameObject);
}

void CPhysicsMgr::AddCharacterControllerObject(CGameObject* _GameObject)
{
    CCharacterController* pCharacterController = _GameObject->CharacterController();

    if (nullptr == pCharacterController || !pCharacterController->m_bEnabled)
        return;

    CTransform* pTr = _GameObject->Transform();

    Vec3 WorldScale = pTr->GetWorldScale();

    WorldScale /= m_PPM;

    Ptr<CPhysicMaterial> pDefaultMtrl = CAssetMgr::GetInst()->FindAsset<CPhysicMaterial>(L"Default Material");
    PxMaterial* DefaultPxMtrl =
        m_Physics->createMaterial(pDefaultMtrl->GetStaticFriction(), pDefaultMtrl->GetDynamicFriction(), pDefaultMtrl->GetBounciness());

    float RadiusScale = WorldScale.x > WorldScale.z ? WorldScale.x : WorldScale.z;
    float HeightScale = WorldScale.y;

    PxCapsuleControllerDesc desc;
    desc.slopeLimit = max(cosf(DirectX::XMConvertToRadians(pCharacterController->m_SlopeLimit)), 0.f);
    desc.stepOffset = pCharacterController->m_StepOffset;
    desc.contactOffset = pCharacterController->m_SkinWidth;
    desc.radius = pCharacterController->m_Radius * RadiusScale;
    desc.height = (pCharacterController->m_Height - (pCharacterController->m_Radius * 2.f)) * HeightScale;
    desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
    desc.material = DefaultPxMtrl;

    Vec3 WorldPos = pTr->GetWorldPos();
    WorldPos += pCharacterController->m_Center;
    WorldPos /= m_PPM;
    desc.position = PxVec3d(WorldPos.x, WorldPos.y, WorldPos.z);

    desc.reportCallback = &m_CCTCallbackInst;
    desc.userData = (void*)pCharacterController;

    if (desc.isValid())
    {
        PxController* PxCharacterController = m_ControllerMgr->createController(desc);
        pCharacterController->m_RuntimeShape = PxCharacterController;

        // Shape 에 UserData 등록
        PxRigidDynamic* PxActor = PxCharacterController->getActor();
        PxU32 nbShapes = PxActor->getNbShapes();
        std::vector<PxShape*> vecShapes(nbShapes);
        PxActor->getShapes(vecShapes.data(), nbShapes);

        for (UINT i = 0; i < vecShapes.size(); i++)
        {
            vecShapes[i]->userData = (void*)pCharacterController;
        }
    }
}

void CPhysicsMgr::RemovePhysicsObject(CGameObject* _GameObject)
{
    if (nullptr == m_Scene || nullptr == _GameObject)
        return;

    CCharacterController* pCharacterController = _GameObject->CharacterController();
    if (nullptr != pCharacterController && nullptr != pCharacterController->m_RuntimeShape)
    {
        ((PxController*)pCharacterController->m_RuntimeShape)->release();
        pCharacterController->m_RuntimeShape = nullptr;
    }

    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        if (m_vecPhysicsObj[i] != _GameObject)
            continue;

        CRigidbody* pRigidbody = m_vecPhysicsObj[i]->Rigidbody();
        CBoxCollider* pBoxCol = m_vecPhysicsObj[i]->BoxCollider();
        CSphereCollider* pSphereCol = m_vecPhysicsObj[i]->SphereCollider();
        CCapsuleCollider* pCapsuleCol = m_vecPhysicsObj[i]->CapsuleCollider();
        CMeshCollider* pMeshCol = m_vecPhysicsObj[i]->MeshCollider();

        PxRigidActor* RigidActor = nullptr;

        // 리지드바디, 콜라이더가 보유한 Actor 탐색
        if (nullptr != pRigidbody)
            RigidActor = (PxRigidActor*)pRigidbody->m_RuntimeBody;
        else if (nullptr != pBoxCol && nullptr != pBoxCol->m_RuntimeShape)
            RigidActor = ((PxShape*)pBoxCol->m_RuntimeShape)->getActor();
        else if (nullptr != pSphereCol && nullptr != pSphereCol->m_RuntimeShape)
            RigidActor = ((PxShape*)pSphereCol->m_RuntimeShape)->getActor();
        else if (nullptr != pCapsuleCol && nullptr != pCapsuleCol->m_RuntimeShape)
            RigidActor = ((PxShape*)pCapsuleCol->m_RuntimeShape)->getActor();
        else if (nullptr != pMeshCol && nullptr != pMeshCol->m_RuntimeShape)
            RigidActor = ((PxShape*)pMeshCol->m_RuntimeShape)->getActor();

        // PhysX Scene에서 제거
        if (nullptr != RigidActor)
        {
            m_Scene->removeActor(*RigidActor);
        }

        // vecPhysicsObj에서 제거
        m_vecPhysicsObj.erase(m_vecPhysicsObj.begin() + i);

        // Trigger List 에서 제거
        std::list<std::pair<CCollider*, CCollider*>>::iterator iter = m_listTrigger.begin();
        while (iter != m_listTrigger.end())
        {
            if (iter->first == pCharacterController || iter->second == pCharacterController || iter->first == pBoxCol || iter->second == pBoxCol ||
                iter->first == pSphereCol || iter->second == pSphereCol || iter->first == pCapsuleCol || iter->second == pCapsuleCol ||
                iter->first == pMeshCol || iter->second == pMeshCol)
            {
                iter = m_listTrigger.erase(iter);
                continue;
            }

            ++iter;
        }

        if (nullptr != pRigidbody)
            pRigidbody->m_RuntimeBody = nullptr;
        if (nullptr != pBoxCol)
            pBoxCol->m_RuntimeShape = nullptr;
        if (nullptr != pSphereCol)
            pSphereCol->m_RuntimeShape = nullptr;
        if (nullptr != pCapsuleCol)
            pCapsuleCol->m_RuntimeShape = nullptr;
        if (nullptr != pMeshCol)
            pMeshCol->m_RuntimeShape = nullptr;

        break;
    }
}

void CPhysicsMgr::LayerCheck(UINT _LeftLayer, UINT _RightLayer, bool _bCheck)
{
    UINT iRow = (UINT)_LeftLayer;
    UINT iCol = (UINT)_RightLayer;

    if (_bCheck)
    {
        m_Matrix[iRow] |= (1 << iCol);
        m_Matrix[iCol] |= (1 << iRow);
    }
    else
    {
        m_Matrix[iRow] &= ~(1 << iCol);
        m_Matrix[iCol] &= ~(1 << iRow);
    }
}

void CPhysicsMgr::LayerCheck(CLevel* _CurLevel, const wstring& _LeftLayer, const wstring& _RightLayer)
{
    CLayer* pLeftLayer = _CurLevel->GetLayer(_LeftLayer);
    CLayer* pRightLayer = _CurLevel->GetLayer(_RightLayer);

    // 이름에 해당하는 Layer 가 존재하지 않으면
    assert(pLeftLayer && pRightLayer);

    LayerCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}

void CPhysicsMgr::EnableAllLayer()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_Matrix[i] = 0xFFFF;
    }
}

void CPhysicsMgr::DisableAllLayer()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_Matrix[i] = 0;
    }
}
