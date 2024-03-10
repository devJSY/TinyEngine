#include "pch.h"
#include "global.h"
#include "CSkyBox.h"

tTransform g_Transform = {};
tGlobalData g_Global = {};

const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END]{
    "Transform",          "Animation 2D", "Animation 3D", "Light 2D",       "Light 3D", "Camera", "Rigidbody 2D", "Box Collider 2D",
    "Circle Collider 2D", "MeshRender",   "TileMap",      "ParticleSystem", "Skybox",   "Decal",  "Landscape"};

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]{
    "Mesh", "Meshdata", "Prefab", "Texture", "Compute Shader", "Graphics Shader", "Material", "Sound", "Physics2D Material",
};
