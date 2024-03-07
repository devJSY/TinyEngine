#include "pch.h"
#include "CPhysics2DMgr.h"

#include <box2d\\b2_world.h>

CPhysics2DMgr::CPhysics2DMgr()
    : m_PhysicsWorld(nullptr)
{
}

CPhysics2DMgr::~CPhysics2DMgr()
{
    if (nullptr != m_PhysicsWorld)
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }
}

void CPhysics2DMgr::init()
{
}

void CPhysics2DMgr::tick()
{
}

void CPhysics2DMgr::OnPhysics2DStart()
{
    m_PhysicsWorld = new b2World({0.0f, -9.8f});
}

void CPhysics2DMgr::OnPhysics2DStop()
{
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
}
