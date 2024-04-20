#pragma once

#include <PhysX\\PxPhysicsAPI.h>

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
    physx::PxDefaultAllocator m_Allocator;
    physx::PxDefaultErrorCallback m_ErrorCallback;
    physx::PxFoundation* m_Foundation;
    physx::PxPhysics* m_Physics;
    physx::PxDefaultCpuDispatcher* m_Dispatcher;
    physx::PxScene* m_Scene;
    physx::PxPvd* m_Pvd;

public:
    void tick();

    void OnPhysicsStart();
    void OnPhysicsStop();
};
