#include "pch.h"
#include "global.h"
#include "CSkyBox.h"

tTransform g_Transform = {};
tGlobalData g_Global = {};

const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END]{"Transform",      "Collider2D", "Collider3D", "Animation2D", "Animation3D",
                                                             "Light2D",        "Light3D",    "Camera",     "MeshRender",  "TileMap",
                                                             "ParticleSystem", "Skybox",     "Decal",      "Landscape"};

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]{
    "Mesh", "Meshdata", "Prefab", "Texture", "Compute_Shader", "Graphics_Shader", "Material", "Sound",
};
