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
#include <box2d\\b2_edge_shape.h>

#include <box2d\\b2_contact.h>

void CCollision2DCallback::BeginContact(b2Contact* contact)
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

void CCollision2DCallback::EndContact(b2Contact* contact)
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
    , m_Accumulator(0.f)
    , m_StepSize(0.01666f) // 1.f / 60.f
    , m_Gravity(Vec2(0.f, -9.8f))
    , m_PPM(1.f)
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
    if (nullptr == m_PhysicsWorld || m_vecPhysicsObj.empty())
        return;

    // 시뮬레이션 안정성을 위해 StepSize 단위로 시뮬레이션
    m_Accumulator += DT;
    while (m_Accumulator < m_StepSize)
    {
        m_Accumulator -= m_StepSize;

        const int32_t velocityIterations = 6; // 속도를 얼마나 강하게 수정해야하는지
        const int32_t positionIterations = 2; // 위치를 얼마나 강하게 수정해야 하는지

        // 시뮬레이션
        m_PhysicsWorld->Step(m_StepSize, velocityIterations, positionIterations);

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

        // 시뮬레이션 결과값으로 트랜스폼 업데이트
        for (UINT i = 0; i < m_vecPhysicsObj.size(); i++)
        {
            CRigidbody2D* rb2d = m_vecPhysicsObj[i]->Rigidbody2D();
            if (nullptr == rb2d)
                continue;

            CTransform* pTr = m_vecPhysicsObj[i]->Transform();

            b2Body* body = (b2Body*)rb2d->m_RuntimeBody;
            const auto& position = body->GetPosition();
            Vec3 pos = pTr->GetLocalPos();
            Vec3 rot = pTr->GetLocalRotation();

            pos.x += (position.x * m_PPM) - pTr->GetWorldPos().x;
            pos.y += (position.y * m_PPM) - pTr->GetWorldPos().y;

            rot.z += body->GetAngle() - pTr->GetWorldRotation().z;

            pTr->SetLocalPos(pos);
            pTr->SetLocalRotation(rot);
        }
    }
}

void CPhysics2DMgr::OnPhysics2DStart()
{
    m_PhysicsWorld = new b2World(m_Gravity);
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
    // Physics Object가 보유하고있던 데이터 초기화
    for (size_t i = 0; i < m_vecPhysicsObj.size(); i++)
    {
        CRigidbody2D* rb2d = m_vecPhysicsObj[i]->Rigidbody2D();
        CBoxCollider2D* bc2d = m_vecPhysicsObj[i]->BoxCollider2D();
        CCircleCollider2D* cc2d = m_vecPhysicsObj[i]->CircleCollider2D();
        CPolygonCollider2D* pc2d = m_vecPhysicsObj[i]->PolygonCollider2D();
        CEdgeCollider2D* ec2d = m_vecPhysicsObj[i]->EdgeCollider2D();

        if (nullptr != rb2d)
            rb2d->m_RuntimeBody = nullptr;
        if (nullptr != bc2d)
            bc2d->m_RuntimeFixture = nullptr;
        if (nullptr != cc2d)
            cc2d->m_RuntimeFixture = nullptr;
        if (nullptr != pc2d)
            pc2d->m_RuntimeFixture = nullptr;
        if (nullptr != ec2d)
            ec2d->m_RuntimeFixture = nullptr;
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
    if (nullptr == m_PhysicsWorld || nullptr == _GameObject || !_GameObject->IsActive() || _GameObject->IsDead())
        return;

    CTransform* pTr = _GameObject->Transform();
    CRigidbody2D* rb2d = _GameObject->Rigidbody2D();
    CBoxCollider2D* bc2d = _GameObject->BoxCollider2D();
    CCircleCollider2D* cc2d = _GameObject->CircleCollider2D();
    CPolygonCollider2D* pc2d = _GameObject->PolygonCollider2D();
    CEdgeCollider2D* ec2d = _GameObject->EdgeCollider2D();

    if (nullptr == rb2d && nullptr == bc2d && nullptr == cc2d && nullptr == pc2d && nullptr == ec2d)
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
        bodyDef.allowSleep = false; // Sleep Mode Disable

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
        Vec3 scale = pTr->GetWorldScale();
        b2PolygonShape boxShape;
        boxShape.SetAsBox(bc2d->m_Size.x * scale.x / m_PPM, bc2d->m_Size.y * scale.y / m_PPM, bc2d->m_Offset * Vec2(scale.x, scale.y) / m_PPM, 0.f);

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
        body->SetEnabled(bc2d->IsEnabled());
    }

    // Circle Collider 2D
    if (nullptr != cc2d)
    {
        Vec3 scale = pTr->GetWorldScale();
        b2CircleShape circleShape;
        circleShape.m_p.Set(cc2d->m_Offset.x * scale.x / m_PPM, cc2d->m_Offset.y * scale.y / m_PPM);
        circleShape.m_radius = scale.x * cc2d->m_Radius / m_PPM;

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
        body->SetEnabled(cc2d->IsEnabled());
    }

    // Polygon Collider 2D
    if (nullptr != pc2d)
    {
        const vector<Vec2>& Points = pc2d->GetPoints();
        vector<b2Vec2> vertices;
        vertices.resize(Points.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertices[i].x = (Points[i].x + pc2d->m_Offset.x) * pTr->GetWorldScale().x / m_PPM;
            vertices[i].y = (Points[i].y + pc2d->m_Offset.y) * pTr->GetWorldScale().y / m_PPM;
        }

        b2PolygonShape PolygonShape;
        PolygonShape.Set(vertices.data(), (int)vertices.size());

        // 유효한 다각형인경우에만 생성
        if (PolygonShape.Validate())
        {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &PolygonShape;
            fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(pc2d);
            Ptr<CPhysics2DMaterial> Mtrl = pc2d->GetMaterial();
            if (nullptr != Mtrl)
            {
                fixtureDef.friction = Mtrl->m_Friction;
                fixtureDef.restitution = Mtrl->m_Bounciness;
            }
            fixtureDef.density = 1.f;
            fixtureDef.isSensor = nullptr == rb2d ? true : pc2d->m_bTrigger;

            fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
            fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

            pc2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
            body->SetEnabled(pc2d->IsEnabled());
        }
    }

    // Edge Collider 2D
    if (nullptr != ec2d)
    {
        Vec2 Scale = Vec2(pTr->GetWorldScale().x, pTr->GetWorldScale().y);
        b2EdgeShape EdgeShape;
        EdgeShape.m_radius = ec2d->GetEdgeRadius();
        EdgeShape.m_vertex1 = (ec2d->GetStartPoint() + ec2d->m_Offset) * Scale / m_PPM;
        EdgeShape.m_vertex2 = (ec2d->GetEndPoint() + ec2d->m_Offset) * Scale / m_PPM;

        if (ec2d->IsUseAdjacentStartPoint())
        {
            EdgeShape.m_vertex0 = (ec2d->GetAdjacentStartPoint() + ec2d->m_Offset) * Scale / m_PPM;
            EdgeShape.m_oneSided = true;
        }

        if (ec2d->IsUseAdjacentEndPoint())
        {
            EdgeShape.m_vertex3 = (ec2d->GetAdjacentEndPoint() + ec2d->m_Offset) * Scale / m_PPM;
            EdgeShape.m_oneSided = true;
        }

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &EdgeShape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(ec2d);
        Ptr<CPhysics2DMaterial> Mtrl = ec2d->GetMaterial();
        if (nullptr != Mtrl)
        {
            fixtureDef.friction = Mtrl->m_Friction;
            fixtureDef.restitution = Mtrl->m_Bounciness;
        }
        fixtureDef.density = 1.f;
        fixtureDef.isSensor = nullptr == rb2d ? true : ec2d->m_bTrigger;

        fixtureDef.filter.categoryBits = (1 << _GameObject->GetLayerIdx());
        fixtureDef.filter.maskBits = m_Matrix[_GameObject->GetLayerIdx()];

        ec2d->m_RuntimeFixture = body->CreateFixture(&fixtureDef);
        body->SetEnabled(ec2d->IsEnabled());
    }

    // 질량 설정
    if (nullptr != rb2d)
    {
        if (!rb2d->m_bAutoMass)
        {
            b2MassData MassData = b2MassData();
            MassData.mass = rb2d->m_Mass;
            body->SetMassData(&MassData);
        }

        body->SetEnabled(rb2d->m_bSimulated);
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
        CPolygonCollider2D* pc2d = _GameObject->PolygonCollider2D();
        CEdgeCollider2D* ec2d = _GameObject->EdgeCollider2D();

        b2Body* body = nullptr;

        if (nullptr != rb2d)
            body = (b2Body*)rb2d->m_RuntimeBody;
        else if (nullptr != bc2d && nullptr != bc2d->m_RuntimeFixture)
            body = ((b2Fixture*)bc2d->m_RuntimeFixture)->GetBody();
        else if (nullptr != cc2d && nullptr != cc2d->m_RuntimeFixture)
            body = ((b2Fixture*)cc2d->m_RuntimeFixture)->GetBody();
        else if (nullptr != pc2d && nullptr != pc2d->m_RuntimeFixture)
            body = ((b2Fixture*)pc2d->m_RuntimeFixture)->GetBody();
        else if (nullptr != ec2d && nullptr != ec2d->m_RuntimeFixture)
            body = ((b2Fixture*)ec2d->m_RuntimeFixture)->GetBody();

        if (nullptr != body)
        {
            m_PhysicsWorld->DestroyBody(body);
        }

        m_vecPhysicsObj.erase(m_vecPhysicsObj.begin() + i);

        if (nullptr != rb2d)
            rb2d->m_RuntimeBody = nullptr;
        if (nullptr != bc2d)
            bc2d->m_RuntimeFixture = nullptr;
        if (nullptr != cc2d)
            cc2d->m_RuntimeFixture = nullptr;
        if (nullptr != pc2d)
            pc2d->m_RuntimeFixture = nullptr;
        if (nullptr != ec2d)
            ec2d->m_RuntimeFixture = nullptr;

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
    if (nullptr == pLeftLayer || nullptr == pRightLayer)
    {
        assert(nullptr);
        return;
    }

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
        CPolygonCollider2D* pc2d = m_vecPhysicsObj[i]->PolygonCollider2D();
        CEdgeCollider2D* ec2d = m_vecPhysicsObj[i]->EdgeCollider2D();

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

        if (nullptr != pc2d && pc2d->IsCollision(_Point))
        {
            pCollisionObj = m_vecPhysicsObj[i];
            break;
        }

        if (nullptr != ec2d && ec2d->IsCollision(_Point))
        {
            pCollisionObj = m_vecPhysicsObj[i];
            break;
        }
    }

    if (!IsRunning)
        OnPhysics2DStop();

    return pCollisionObj;
}

RaycastHit2D CPhysics2DMgr::RayCast(Vec2 _Origin, Vec2 _Direction, float _Distance, WORD _LayerMask)
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
        CPolygonCollider2D* pc2d = m_vecPhysicsObj[i]->PolygonCollider2D();
        CEdgeCollider2D* ec2d = m_vecPhysicsObj[i]->EdgeCollider2D();

        b2Fixture* fixture = nullptr;

        if (nullptr != bc2d && bc2d->RayCast(_Origin, _Direction, _Distance, Hit))
            break;

        if (nullptr != cc2d && cc2d->RayCast(_Origin, _Direction, _Distance, Hit))
            break;

        if (nullptr != pc2d && pc2d->RayCast(_Origin, _Direction, _Distance, Hit))
            break;

        if (nullptr != ec2d && ec2d->RayCast(_Origin, _Direction, _Distance, Hit))
            break;
    }

    if (!IsRunning)
        OnPhysics2DStop();

    return Hit;
}

RaycastHit2D CPhysics2DMgr::RayCast(Vec2 _Origin, Vec2 _Direction, float _Distance, const vector<wstring>& _LayerNames)
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
        return RaycastHit2D();

    return RayCast(_Origin, _Direction, _Distance, LayerMask);
}