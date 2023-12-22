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

// Rasterizer State Type
enum class RS_TYPE
{
    CULL_BACK,
    CULL_FRONT,
    CULL_NONE,
    WIRE_FRAME,

    END,
};

// DepthStencil State Type
enum class DS_TYPE
{
    LESS,       // 작은게 통과			깊이 기록 O
    LESS_EQUAL, // 작거나 같으면 통과		깊이 기록 O

    GREATER,       // 더 멀면 통과			깊이 기록 O
    GREATER_EQUAL, // 더 멀거나 같으면 통과	깊이 기록 O

    NO_TEST,  // 깊이 테스트 X			깊이 기록 O
    NO_WRITE, // 깊이 테스트 O			깊이 기록 X

    NO_TEST_NO_WRITE, // 깊이 테스트 X			깊이 기록 X

    END,
};

// Blend State
enum class BS_TYPE
{
    DEFAULT,

    ALPHA_BLEND,
    
    ONE_ZERO,

    ONE_ONE,

    END,
};



enum class EDITOR_TYPE
{
    LEVEL,

    END,
};

enum TASK_TYPE
{
    // Param1 : Layer Type, Param2 : Object Adress
    CREATE_OBJECT,

    // Param1 : Object Adress
    DELETE_OBJECT,

    // Param1 : LEVEL_TYPE
    LEVEL_CHANGE,

    // Param1 : Width , Param2 : Height
    WINDOW_RESIZE,
};
