#pragma once

#define SINGLE(classtype)                                                                                                                            \
private:                                                                                                                                             \
    classtype();                                                                                                                                     \
    ~classtype();                                                                                                                                    \
    friend class CSingleton<classtype>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define CLONE(type)                                                                                                                                  \
public:                                                                                                                                              \
    virtual type* Clone()                                                                                                                            \
    {                                                                                                                                                \
        return new type(*this);                                                                                                                      \
    }

#define KEY_CHECK(Key, State) (CKeyMgr::GetInst()->GetKeyState(Key) == State)
#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define DT_ENGINE CTimeMgr::GetInst()->GetEngineDeltaTime()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define LAYER_MAX 16

#define CLONE(type)                                                                                                                                  \
public:                                                                                                                                              \
    virtual type* Clone()                                                                                                                            \
    {                                                                                                                                                \
        return new type(*this);                                                                                                                      \
    }

#define CLONE_DISABLE(type)                                                                                                                          \
public:                                                                                                                                              \
    virtual type* Clone()                                                                                                                            \
    {                                                                                                                                                \
        return nullptr;                                                                                                                              \
        assert(nullptr);                                                                                                                             \
    }                                                                                                                                                \
    type(const type& origin) = delete;

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
    PREFAB,
    TEXTURE,
    COMPUTE_SHADER,
    GRAPHICS_SHADER,
    MATERIAL,
    SOUND,
    PHYSICS2D_MATERIAL,
    END,
};
extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];

enum class COMPONENT_TYPE
{
    TRANSFORM, // 오브젝트의 위치, 크기, 회전

    ANIMATOR2D, // 스프라이트 Animation
    ANIMATOR3D, // Bone Skinning Animation

    LIGHT2D, // 2차원 광원
    LIGHT3D, // 3차원 광원

    CAMERA, // 카메라 기능
    
    // 2D Physics
    RIGIDBODY2D,      // 리지드바디 2D
    BOXCOLLIDER2D,    // BOX 콜라이더 2D
    CIRCLECOLLIDER2D, // CIRCLE 콜라이더 2D

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
extern const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END];

// 상수버퍼 종류
enum class CB_TYPE
{
    TRANSFORM,
    MATERIAL_CONST,
    ANIM2D_DATA,
    GLOBAL_DATA,
    ANIMATION,

    END,
};

// Rasterizer State Type
enum class RS_TYPE
{
    CULL_BACK,
    CULL_BACK_CCW,

    CULL_FRONT,
    CULL_FRONT_CCW,

    CULL_NONE,
    CULL_NONE_CCW,

    WIRE_FRAME,
    WIRE_FRAME_CCW,

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

    MASK,        // Stencil Buffer Masking 용
    DRAW_MASKED, // Stencil Buffer Masking 되어있는 부분만 렌더링

    END,
};

// Blend State
enum class BS_TYPE
{
    DEFAULT,     // No Blending
    MASK,        // Alpha Coverage
    ALPHA_BLEND, // Alpha 계수
    ONE_ONE,     // 1:1 혼합
    END,
};

// Sampler State
enum class SS_TYPE
{
    LINEAR_WRAP,
    LINEAR_CLAMP,
    POINT,
    ANISOTROPIC,
    SHADOW_POINT,
    SHADOW_COMPARISION,
    END,
};

// ScalarParam
enum SCALAR_PARAM
{
    INT_0,
    INT_1,
    INT_2,
    INT_3,

    FLOAT_0,
    FLOAT_1,
    FLOAT_2,
    FLOAT_3,

    VEC2_0,
    VEC2_1,
    VEC2_2,
    VEC2_3,

    VEC4_0,
    VEC4_1,
    VEC4_2,
    VEC4_3,

    MAT_0,
    MAT_1,
    MAT_2,
    MAT_3,
};

enum TEX_PARAM
{
    TEX_0,
    TEX_1,
    TEX_2,
    TEX_3,
    TEX_4,
    TEX_5,

    TEXCUBE_0,
    TEXCUBE_1,

    TEXARR_0,
    TEXARR_1,

    TEX_END,
};

enum class EDITOR_TYPE
{
    LEVEL,
    MATERIAL,
    BLUEPRINT,
    SPRITE,
    TILEMAP,

    END,
};

enum class DEBUG_SHAPE
{
    RECT,
    CIRCLE,
    CROSS,

    BOX,
    SPHERE,
};

enum class LIGHT_TYPE
{
    DIRECTIONAL = 0,
    POINT = 1,
    SPOT = 2,
    END
};

enum class SHADER_DOMAIN
{
    DOMAIN_OPAQUE,      // 불투명
    DOMAIN_MASKED,      // 불투명 or 투명
    DOMAIN_TRANSPARENT, // 반투명
    DOMAIN_POSTPROCESS, // 후처리

    DOMAIN_DEBUG,
};

enum class PARTICLE_MODULE
{
    SPAWN,        // 파티클 생성 관련
    DRAG,         // 감속 관련 모듈
    SCALE,        // 크기 변화 모듈
    ADD_VELOCITY, // 초기 속도 지정 모듈
    NOISE_FORCE,
    CALCULATE_FORCE, // 주어진 힘을 계산하는 모듈

    RENDER, // 렌더링 때의 옵션 관련

    END,
};

enum class LEVEL_STATE
{
    PLAY,
    SIMULATE,
    PAUSE,
    STOP,
};

enum class Physics2D_EVENT_TYPE
{
    ADD,
    REMOVE,
    RESPAWN,
};
