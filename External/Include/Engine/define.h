#pragma once

#define SINGLE(classtype)                                                                                              \
private:                                                                                                               \
    classtype();                                                                                                       \
    ~classtype();                                                                                                      \
    friend class CSingleton<classtype>;

#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_CHECK(Key, State) CKeyMgr::GetInst()->GetKeyState(Key) == State
#define DT                    CTimeMgr::GetInst()->GetDeltaTime()

#define KEY_TAP(Key)      KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key)  KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key)     KEY_CHECK(Key, NONE)

#define LAYER_MAX 32

enum class DIR_TYPE
{
    RIGHT,
    UP,
    FRONT,
};

enum class ASSET_TYPE
{
    MESH,
    MESHDATA,
    TEXTURE,
    MATERIAL,
    SOUND,
    COMPUTE_SHADER,
    GRAPHICS_SHADER,
    END,
};

enum class COMPONENT_TYPE
{
    TRANSFORM, // 오브젝트의 위치, 크기, 회전

    COLLIDER2D, // 2차원 충돌체
    COLLIDER3D, // 3차원 충돌체

    ANIMATOR2D, // 스프라이트 Animation
    ANIMATOR3D, // Bone Skinning Animation

    LIGHT2D, // 2차원 광원
    LIGHT3D, // 3차원 광원

    CAMERA, // 카메라 기능

    // Render Component
    MESHRENDER,
    TILEMAP,
    PARTICLESYSTEM,
    SKYBOX,
    DECAL,
    LANDSCAPE,

    END,

    SCRIPT,
};

// 상수버퍼 종류
enum class CB_TYPE
{
    TRANSFORM,
    MATERIAL_CONST,
    GLOBAL_DATA,
    ANIMATION,

    END,
};

enum class EDITOR_TYPE
{
    LEVEL,

    END,
};