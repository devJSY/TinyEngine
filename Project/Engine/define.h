#pragma once

#define SINGLE(classtype)                                                                                                                            \
private:                                                                                                                                             \
    classtype();                                                                                                                                     \
    ~classtype();                                                                                                                                    \
    friend class CSingleton<classtype>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_CHECK(Key, State) (CKeyMgr::GetInst()->GetKeyState(Key) == State)
#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define DT_ENGINE CTimeMgr::GetInst()->GetEngineDeltaTime()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define LAYER_MAX 16
#define MAX_MIP 8

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
    }                                                                                                                                                \
    type(const type& origin) = delete;

enum class DIR_TYPE
{
    RIGHT,
    UP,
    FRONT,
};

enum class AXIS_TYPE
{
    X,
    Y,
    Z,
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
    PHYSIC_MATERIAL,
    END,
};
extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];

enum class COMPONENT_TYPE
{
    TRANSFORM, // ������Ʈ�� ��ġ, ũ��, ȸ��

    ANIMATOR2D, // ��������Ʈ Animation
    ANIMATOR,   // Bone Skinning Animation

    LIGHT2D, // 2���� ����
    LIGHT,   // 3���� ����

    CAMERA, // ī�޶� ���

    // 2D Physics
    RIGIDBODY2D,       // ������ٵ� 2D
    BOXCOLLIDER2D,     // BOX �ݶ��̴� 2D
    CIRCLECOLLIDER2D,  // CIRCLE �ݶ��̴� 2D
    POLYGONCOLLIDER2D, // Polygon �ݶ��̴� 2D
    EDGECOLLIDER2D,    // Edge �ݶ��̴� 2D

    // 3D Physics
    RIGIDBODY,
    BOXCOLLIDER,
    SPHERECOLLIDER,
    CAPSULECOLLIDER,
    MESHCOLLIDER,
    CHARACTERCONTROLLER,

    // Sound
    SOUNDLISTENER,

    // Render Component
    MESHRENDER,
    TILEMAP,
    PARTICLESYSTEM,
    SKYBOX,
    DECAL,
    LANDSCAPE,
    TEXTRENDER,

    END,

    SCRIPT,
};
extern const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END];

// ������� ����
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
    LESS,       // ������ ���			���� ��� O
    LESS_EQUAL, // �۰ų� ������ ���		���� ��� O

    GREATER,       // �� �ָ� ���			���� ��� O
    GREATER_EQUAL, // �� �ְų� ������ ���	���� ��� O

    NO_TEST,  // ���� �׽�Ʈ X			���� ��� O
    NO_WRITE, // ���� �׽�Ʈ O			���� ��� X

    NO_TEST_NO_WRITE, // ���� �׽�Ʈ X			���� ��� X

    MASK,        // Stencil Buffer Masking ��
    DRAW_MASKED, // Stencil Buffer Masking �Ǿ��ִ� �κи� ������

    END,
};

// Blend State
enum class BS_TYPE
{
    DEFAULT,     // No Blending
    MASK,        // Alpha Coverage
    ALPHA_BLEND, // Alpha ���
    ONE_ONE,     // 1:1 ȥ��
    DECAL,
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
    TEX_6,
    TEX_7,

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
    MODEL,
    TILEMAP,
    PHYSICS2D_MATERIAL,
    PHYSIC_MATERIAL,
    SOUND,

    END,
};

enum class DEBUG_SHAPE
{
    // 2D
    LINE,
    RECT,
    CIRCLE,
    CROSS,
    POLYGON,

    // 3D
    BOX,
    SPHERE,
    CONE,
    CAPSULE,
    FRUSTUM,
    MESH,
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
    // Skybox
    DOMAIN_SKYBOX,

    // Deferred
    DOMAIN_DEFERRED, // ���� ������
    DOMAIN_DECAL,
    DOMAIN_LIGHT, // ���� ����
    DOMAIN_MERGE, // Deferred ������ SwapChain Ÿ������ �̵�

    // Forward
    DOMAIN_OPAQUE,                 // ������
    DOMAIN_MASKED,                 // ������ or ����
    DOMAIN_TRANSPARENT,            // ������
    DOMAIN_TRANSPARENT_BACKGROUND, // �������� �׷����� ������
    DOMAIN_POSTPROCESS,            // ��ó��

    // Shadow
    DOMAIN_SHADOW,

    // Debug
    DOMAIN_DEBUG,
};

enum class PARTICLE_MODULE
{
    SPAWN,           // ��ƼŬ ���� ����
    DRAG,            // ���� ���� ���
    SCALE,           // ũ�� ��ȭ ���
    ADD_VELOCITY,    // �ʱ� �ӵ� ���� ���
    SPIRAL_VELOCITY, // ���� �ӵ� ���� ���
    NOISE_FORCE,
    CALCULATE_FORCE, // �־��� ���� ����ϴ� ���

    RENDER, // ������ ���� �ɼ� ����

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
    RESPAWN,
};

enum class Physics_EVENT_TYPE
{
    RESPAWN,
};

enum class MRT_TYPE
{
    SWAPCHAIN,
    LDRI,
    HDRI,
    DEFERRED,
    DEFERRED_PHONG,
    DECAL,
    LIGHT,
    SHADOW,
    IDMAP,
    SSAO,
    END,
};

enum MOBILITY_TYPE
{
    STATIC = 0x0001,
    MOVABLE = 0x0002,
};

enum class AnimatorUpdateMode
{
    Normal,
    UnscaledTime,
};
