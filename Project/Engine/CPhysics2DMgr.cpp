#include "pch.h"
#include "CPhysics2DMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"

#include <box2d\\b2_world.h>
#include <box2d\\b2_body.h>
#include <box2d\\b2_fixture.h>
#include <box2d\\b2_polygon_shape.h>
#include <box2d\\b2_circle_shape.h>

#include <box2d\\b2_contact.h>

void CollisionCallback::BeginContact(b2Contact* contact)
{
    CCollider2D* pColliderA = (CCollider2D*)contact->GetFixtureA()->GetUserData().pointer;
    CCollider2D* pColliderB = (CCollider2D*)contact->GetFixtureB()->GetUserData().pointer;

    if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
    {
        pColliderA->OnTriggerEnter(pColliderB);
        pColliderB->OnTriggerEnter(pColliderA);
    }
    else
    {
        pColliderA->OnCollisionEnter(pColliderB);
        pColliderB->OnCollisionEnter(pColliderA);
    }
}

void CollisionCallback::EndContact(b2Contact* contact)
{
    CCollider2D* pColliderA = (CCollider2D*)contact->GetFixtureA()->GetUserData().pointer;
    CCollider2D* pColliderB = (CCollider2D*)contact->GetFixtureB()->GetUserData().pointer;

    if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
    {
        pColliderA->OnTriggerExit(pColliderB);
        pColliderB->OnTriggerExit(pColliderA);
    }
    else
    {
        pColliderA->OnCollisionExit(pColliderB);
        pColliderB->OnCollisionExit(pColliderA);
    }
}

static b2BodyType Rigidbody2DTypeTob2BodyType(BODY_TYPE bodyType)
{
    switch (bodyType)
    {
    case BODY_TYPE::Dynamic:
        return b2_dynamicBody;
    case BODY_TYPE::Kinematic:
        return b2_kinematicBody;
    case BODY_TYPE::Static:
        return b2_staticBody;
    }

    LOG(Error, "Unknown body type");
    return b2_dynamicBody;
}

CPhysics2DMgr::CPhysics2DMgr()
    : m_PhysicsWorld(nullptr)
    , m_CallbackInst()
    , m_vecPhysicsObj{}
    , m_Matrix{}
    , m_PPM(100.f)
{
    EnableAllLayer();
}

CPhysics2DMgr::~CPhysics2DMgr()
{
    if (nullptr != m_PhysicsWorld)
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }
}

void CPhysics2DMgr::tick()
{
    if (nullptr == m_PhysicsWorld)
        return;

    const int32_t velocityIterations = 6; // 속도를 얼마나 강하게 수정해야하는지
    const int32_t positionIterations = 2; // 위치를 얼마나 강하게 수정해야 하는지

    // 시뮬레이션
    m_PhysicsWorld->Step(DT, velocityIterations, positionIterations);

    // 시뮬레이션 이후 충돌 카운트가 남아있다면 충돌 Stay 상태
    b2Contact* contact = m_PhysicsWorld->GetContactList();
    while (contact)
    {
        CCollider2D* pColliderA = (CCollider2D*)contact->GetFixtureA()->GetUserData().pointer;
        CCollider2D* pColliderB = (CCollider2D*)contact->GetFixtureB()->GetUserData().pointer;

        if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
        {
            if (pColliderA->m_TriggerCount > 0)
                pColliderA->OnTriggerStay(pColliderB);
            if (pColliderB->m_TriggerCount > 0)
                pColliderB->OnTriggerStay(pColliderA);
        }
        else
        {
            if (pColliderA->m_CollisionCount > 0)
                pColliderA->OnCollisionStay(pColliderB);
            if (pColliderB->m_CollisionCount > 0)
                pColliderB->OnCollisionStay(pColliderA);
        }

        contact = contact->GetNext();
    }

    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CTransform* pTr = m_vecPhysicsObj[i]->Transform();
        CRigidbody2D* rb2d = m_vecPhysicsObj[i]->Rigidbody2D();

        // 시뮬레이션 결과값으로 트랜스폼 업데이트
        if (nullptr != rb2d)
        {
            b2Body* body = (b2Body*)rb2d->m_RuntimeBody;
            const auto& position = body->GetPosition();
            Vec3 pos = pTr->GetRelativePos();
            Vec3 rot = pTr->GetRelativeRotation();

            pos.x += (position.x * m_PPM) - pTr->GetWorldPos().x;
            pos.y += (position.y * m_PPM) - pTr->GetWorldPos().y;

            rot.z += body->GetAngle() - pTr->GetWorldRotation().z;

            pTr->SetRelativePos(pos);
            pTr->SetRelativeRotation(rot);
        }
    }
}

void CPhysics2DMgr::OnPhysics2DStart()
{
    m_PhysicsWorld = new b2World({0.0f, -9.8f});
    m_PhysicsWorld->SetContactListener(&m_CallbackInst);

    // 레벨의 모든 오브젝트를 순회하여 World 에 추가할 오브젝트를 등록
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

void CPhysics2DMgr::OnPhysics2DStop()
{
    for (size_t i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CRigidbody2D* rb2d = m_vecPhysicsObj[i]->Rigidbody2D();
        CBoxCollider2D* bc2d = m_vecPhysicsObj[i]->BoxCollider2D();
        CCircleCollider2D* cc2d = m_vecPhysicsObj[i]->CircleCollider2D();

        if (nullptr != rb2d)
            rb2d->m_RuntimeBody = nullptr;
        if (nullptr != bc2d)
            bc2d->m_RuntimeFixture = nullptr;
        if (nullptr != cc2d)
            cc2d->m_RuntimeFixture = nullptr;
    }

    if (nullptr != m_PhysicsWorld)
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    m_vecPhysicsObj.clear();
}

void CPhysics2DMgr::AddPhysicsObject(CGameObject* _GameObject)
{
    if (nullptr == m_PhysicsWorld)
        return;

    CTransform* pTr = _GameObject->Transform();
    CRigidbody2D* rb2d = _GameObject->Rigidbody2D();
    CBoxCollider2D* bc2d = _GameObject->BoxCollider2D();
    CCircleCollider2D* cc2d = _GameObject->CircleCollider2D();

    if (nullptr == rb2d && nullptr == bc2d && nullptr == cc2d)
        return;

    b2BodyDef bodyDef;
    bodyDef.position.Set(pTr->GetWorldPos().x / m_PPM, pTr->GetWorldPos().y / m_PPM);
    bodyDef.angle = pTr->GetWorldRotation().z;
    b2Body* body = nullptr;

    // Rigidbody 2D
    if (nullptr != rb2d)
    {
        bodyDef.type = Rigidbody2DTypeTob2BodyType(rb2d->m_BodyType);
        bodyDef.linearDamping = rb2d->m_LinearDrag;
        bodyDef.angularDamping = rb2d->m_AngularDrag;
        bodyDef.gravityScale = rb2d->m_GravityScale;
        bodyDef.enabled = rb2d->m_bSimulated;

        body = m_PhysicsWorld->CreateBody(&bodyDef);
        rb2d->m_RuntimeBody = body;

        body->SetFixedRotation(rb2d->m_bFreezeRotation);
    }
    else
    {
        body = m_PhysicsWorld->CreateBody(&bodyDef);
    }

    // Box Collider 2D
    if (nullptr != bc2d)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(bc2d->m_Size.x * pTr->GetWorldScale().x / m_PPM, bc2d->m_Size.y * pTr->GetWorldScale().y / m_PPM, bc2d->m_Offset / m_PPM,
                          0.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(bc2d);
        Ptr<CPhysics2DMaterial> Mtrl = bc2d->GetMaterial();
        if (nullptr != Mtrl)
        {
            fixtureDef.friction = Mtrl->m_Friction;
            fixtureDef.restitution = Mtrl->m_Bounciness;
        }
        fixtureDef.density = 1.f;
        fixtureDef.isSensor = nullptr == rb2d ? true : bc2d->m_bTrigger;

        fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
        fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

        bc2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
    }

    // Circle Collider 2D
    if (nullptr != cc2d)
    {
        b2CircleShape circleShape;
        circleShape.m_p.Set(cc2d->m_Offset.x / m_PPM, cc2d->m_Offset.y / m_PPM);
        circleShape.m_radius = pTr->GetWorldScale().x * cc2d->m_Radius / m_PPM;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(cc2d);
        Ptr<CPhysics2DMaterial> Mtrl = cc2d->GetMaterial();
        if (nullptr != Mtrl)
        {
            fixtureDef.friction = Mtrl->m_Friction;
            fixtureDef.restitution = Mtrl->m_Bounciness;
        }
        fixtureDef.density = 1.f;
        fixtureDef.isSensor = nullptr == rb2d ? true : cc2d->m_bTrigger;

        fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
        fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

        cc2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
    }

    // 질량 설정
    if (nullptr != rb2d && !rb2d->m_bAutoMass)
    {
        b2MassData MassData = body->GetMassData();
        MassData.mass = rb2d->m_Mass;
        body->SetMassData(&MassData);
    }

    m_vecPhysicsObj.push_back(_GameObject);
}

void CPhysics2DMgr::RemovePhysicsObject(CGameObject* _GameObject)
{
    if (nullptr == m_PhysicsWorld)
        return;

    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        if (m_vecPhysicsObj[i] != _GameObject)
            continue;

        CRigidbody2D* rb2d = _GameObject->Rigidbody2D();
        CBoxCollider2D* bc2d = _GameObject->BoxCollider2D();
        CCircleCollider2D* cc2d = _GameObject->CircleCollider2D();

        b2Body* body = nullptr;

        if (nullptr != rb2d)
            body = (b2Body*)rb2d->m_RuntimeBody;
        else if (nullptr != bc2d)
            body = ((b2Fixture*)bc2d->m_RuntimeFixture)->GetBody();
        else if (nullptr != cc2d)
            body = ((b2Fixture*)cc2d->m_RuntimeFixture)->GetBody();

        assert(body);

        m_PhysicsWorld->DestroyBody(body);
        m_vecPhysicsObj.erase(m_vecPhysicsObj.begin() + i);

        if (nullptr != rb2d)
            rb2d->m_RuntimeBody = nullptr;
        if (nullptr != bc2d)
            bc2d->m_RuntimeFixture = nullptr;
        if (nullptr != cc2d)
            cc2d->m_RuntimeFixture = nullptr;

        break;
    }
}

void CPhysics2DMgr::LayerCheck(UINT _LeftLayer, UINT _RightLayer, bool _bCheck)
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

void CPhysics2DMgr::LayerCheck(CLevel* _CurLevel, const wstring& _LeftLayer, const wstring& _RightLayer)
{
    CLayer* pLeftLayer = _CurLevel->GetLayer(_LeftLayer);
    CLayer* pRightLayer = _CurLevel->GetLayer(_RightLayer);

    // 이름에 해당하는 Layer 가 존재하지 않으면
    assert(pLeftLayer && pRightLayer);

    LayerCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}

void CPhysics2DMgr::EnableAllLayer()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_Matrix[i] = 0xFFFF;
    }
}

void CPhysics2DMgr::DisableAllLayer()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_Matrix[i] = 0;
    }
}

CGameObject* CPhysics2DMgr::CollisionCheck(Vec2 _Point)
{
    bool IsRunning = nullptr != m_PhysicsWorld;
    CGameObject* pCollisionObj = nullptr;

    if (!IsRunning)
        OnPhysics2DStart();

    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CBoxCollider2D* bc2d = m_vecPhysicsObj[i]->BoxCollider2D();
        CCircleCollider2D* cc2d = m_vecPhysicsObj[i]->CircleCollider2D();

        b2Fixture* fixture = nullptr;

        if (nullptr != bc2d && bc2d->IsCollision(_Point))
        {
            pCollisionObj = m_vecPhysicsObj[i];
            break;
        }

        if (nullptr != cc2d && cc2d->IsCollision(_Point))
        {
            pCollisionObj = m_vecPhysicsObj[i];
            break;
        }
    }

    if (!IsRunning)
        OnPhysics2DStop();

    return pCollisionObj;
}

RaycastHit2D CPhysics2DMgr::RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, unsigned short _LayerMask)
{
    RaycastHit2D Hit;
    Hit.Centroid = _Origin;
    Hit.Distance = 0.f;
    Hit.Fraction = 1.f;
    Hit.Normal = Vec2();
    Hit.Point = Vec2();
    Hit.pCollisionObj = nullptr;

    bool IsRunning = nullptr != m_PhysicsWorld;
    if (!IsRunning)
        OnPhysics2DStart();

    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        // 충돌 체크 레이어 설정이 되어있는 경우에만 검사
        if (!(_LayerMask & (1 << m_vecPhysicsObj[i]->GetLayerIdx())))
            continue;

        CBoxCollider2D* bc2d = m_vecPhysicsObj[i]->BoxCollider2D();
        CCircleCollider2D* cc2d = m_vecPhysicsObj[i]->CircleCollider2D();

        b2Fixture* fixture = nullptr;

        if (nullptr != bc2d && bc2d->RayCast(_Origin, _Dirction, _Distance, Hit))
        {
            break;
        }

        if (nullptr != cc2d && cc2d->RayCast(_Origin, _Dirction, _Distance, Hit))
        {
            break;
        }
    }

    if (!IsRunning)
        OnPhysics2DStop();

    return Hit;
}

RaycastHit2D CPhysics2DMgr::RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, const wstring& _LayerName)
{
    CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_LayerName);

    // 해당이름의 레이어가 존재하지 않는경우
    if (nullptr == pCurLayer)
        return RaycastHit2D();

    int LayerIdx = pCurLayer->GetLayerIdx();
    unsigned short LayerMask = 0;

    LayerMask |= (1 << LayerIdx);

    return RayCast(_Origin, _Dirction, _Distance, LayerMask);
}