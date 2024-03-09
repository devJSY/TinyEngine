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

    pColliderA->OnCollisionEnter(pColliderB);
    pColliderB->OnCollisionEnter(pColliderA);

    if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
    {
        pColliderA->OnTriggerEnter(pColliderB);
        pColliderB->OnTriggerEnter(pColliderA);
    }
}

void CollisionCallback::EndContact(b2Contact* contact)
{
    CCollider2D* pColliderA = (CCollider2D*)contact->GetFixtureA()->GetUserData().pointer;
    CCollider2D* pColliderB = (CCollider2D*)contact->GetFixtureB()->GetUserData().pointer;

    pColliderA->OnCollisionExit(pColliderB);
    pColliderB->OnCollisionExit(pColliderA);

    if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
    {
        pColliderA->OnTriggerExit(pColliderB);
        pColliderB->OnTriggerExit(pColliderA);
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

    // 레벨 Play 모드 에서만 계산
    if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
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

        if (pColliderA->m_CollisionCount > 0)
        {
            pColliderA->OnCollisionStay(pColliderB);

            if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
                pColliderA->OnTriggerStay(pColliderB);
        }

        if (pColliderB->m_CollisionCount > 0)
        {
            pColliderB->OnCollisionStay(pColliderA);

            if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
                pColliderB->OnTriggerStay(pColliderA);
        }

        contact = contact->GetNext();
    }

    // 시뮬레이션 이후 트랜스폼 업데이트
    for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CTransform* pTr = m_vecPhysicsObj[i]->Transform();
        CRigidbody2D* rb2d = m_vecPhysicsObj[i]->Rigidbody2D();

        b2Body* body = (b2Body*)rb2d->m_RuntimeBody;
        const auto& position = body->GetPosition();
        pTr->SetRelativePos(Vec3(position.x, position.y, pTr->GetRelativePos().z));
        pTr->SetRelativeRotation(Vec3(pTr->GetRelativeRotation().x, pTr->GetRelativeRotation().y, body->GetAngle()));
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

                AddGameObject(pObject);
            }
        }
    }
}

void CPhysics2DMgr::OnPhysics2DStop()
{
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;

    m_vecPhysicsObj.clear();
}

void CPhysics2DMgr::AddGameObject(CGameObject* _GameObject)
{
    CRigidbody2D* rb2d = _GameObject->Rigidbody2D();
    if (nullptr == m_PhysicsWorld || nullptr == rb2d)
        return;

    CTransform* pTr = _GameObject->Transform();

    b2BodyDef bodyDef;
    bodyDef.type = Rigidbody2DTypeTob2BodyType(rb2d->m_BodyType);
    bodyDef.position.Set(pTr->GetRelativePos().x, pTr->GetRelativePos().y);
    bodyDef.linearDamping = rb2d->m_LinearDrag;
    bodyDef.angularDamping = rb2d->m_AngularDrag;
    bodyDef.gravityScale = rb2d->m_GravityScale;
    bodyDef.angle = pTr->GetRelativeRotation().z;
    bodyDef.enabled = rb2d->m_bSimulated;

    b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
    body->SetFixedRotation(rb2d->m_bFreezeRotation);
    rb2d->m_RuntimeBody = body;

    // Box Collider 2D 를 보유한 오브젝트
    CBoxCollider2D* bc2d = _GameObject->BoxCollider2D();
    if (nullptr != bc2d)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox(bc2d->m_Size.x * pTr->GetRelativeScale().x, bc2d->m_Size.y * pTr->GetRelativeScale().y,
                          b2Vec2(bc2d->m_Offset.x, bc2d->m_Offset.y), 0.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(bc2d);
        fixtureDef.friction = bc2d->m_Friction;
        fixtureDef.restitution = bc2d->m_Bounciness;
        fixtureDef.density = bc2d->m_Density;
        fixtureDef.isSensor = bc2d->m_bTrigger;

        fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
        fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

        bc2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
    }

    // Circle Collider 2D 를 보유한 오브젝트
    CCircleCollider2D* cc2d = _GameObject->CircleCollider2D();
    if (nullptr != cc2d)
    {
        b2CircleShape circleShape;
        circleShape.m_p.Set(cc2d->m_Offset.x, cc2d->m_Offset.y);
        circleShape.m_radius = pTr->GetRelativeScale().x * cc2d->m_Radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(cc2d);
        fixtureDef.friction = cc2d->m_Friction;
        fixtureDef.restitution = cc2d->m_Bounciness;
        fixtureDef.density = cc2d->m_Density;
        fixtureDef.isSensor = cc2d->m_bTrigger;

        fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
        fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

        cc2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
    }

    m_vecPhysicsObj.push_back(_GameObject);
}

void CPhysics2DMgr::RemoveGameObject(CGameObject* _GameObject)
{
    CRigidbody2D* rb2d = _GameObject->Rigidbody2D();
    if (nullptr == m_PhysicsWorld || nullptr == rb2d)
        return;

    m_PhysicsWorld->DestroyBody((b2Body*)rb2d->m_RuntimeBody);
    m_vecPhysicsObj.erase(remove(m_vecPhysicsObj.begin(), m_vecPhysicsObj.end(), _GameObject), m_vecPhysicsObj.end());
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
