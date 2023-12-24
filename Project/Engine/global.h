#pragma once

#include <stdlib.h>
#include <Windows.h>
#include <typeinfo>

#include <string>

#include <vector>
#include <list>
#include <map>

using std::list;
using std::make_pair;
using std::map;
using std::vector;

using std::string;
using std::wstring;

#include <wrl.h>
using namespace Microsoft::WRL;

// Directx 11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
    #pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
    #pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"

// imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

// Assimp
#define NOMINMAX
#ifdef _DEBUG
    #pragma comment(lib, "Assimp\\assimp-vc143-mtd_debug")
#else
    #pragma comment(lib, "Assimp\\assimp-vc143-mt")
#endif
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

// ================
#include <iostream>
#include <algorithm>

using std::tuple;
