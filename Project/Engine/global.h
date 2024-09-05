#pragma once

// #define DISTRIBUTE // 게임 배포 시 정의

#include <stdlib.h>
#include <Windows.h>
#include <typeinfo>

#include <iostream>
#include <algorithm>
#include <filesystem>

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <stack>

#include <thread>
#include <future>

#include <commdlg.h> // OPENFILENAME

using std::hash;
using std::list;
using std::make_pair;
using std::map;
using std::tuple;
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
#include <DirectXTex\\DirectXTex.h>

#ifdef _DEBUG
    #pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
    #pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

// Directx Mesh
#include <DirectXMesh\\DirectXMesh.h>

#ifdef _DEBUG
    #pragma comment(lib, "DirectXMesh\\DirectXMesh_debug")
#else
    #pragma comment(lib, "DirectXMesh\\DirectXMesh")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
typedef Quaternion Quat;

// ImGui
#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// ImGuizmo
#include <ImGuizmo\\ImGuizmo.h>

// ImGui Node Editor
#include <imgui_node_editor\\imgui_node_editor.h>

// Box2D
#ifdef _DEBUG
    #pragma comment(lib, "box2d\\box2d_debug")
#else
    #pragma comment(lib, "box2d\\box2d")
#endif

// PhysX
#ifdef _DEBUG
    #pragma comment(lib, "PhysX\\PhysX_64_debug")
    #pragma comment(lib, "PhysX\\PhysXCommon_64_debug")
    #pragma comment(lib, "PhysX\\PhysXCooking_64_debug")
    #pragma comment(lib, "PhysX\\PhysXExtensions_static_64_debug")
    #pragma comment(lib, "PhysX\\PhysXFoundation_64_debug")
    #pragma comment(lib, "PhysX\\PhysXPvdSDK_static_64_debug")
    #pragma comment(lib, "PhysX\\PhysXVehicle_static_64_debug")
    #pragma comment(lib, "PhysX\\PhysXCharacterKinematic_static_64_debug")
#else
    #pragma comment(lib, "PhysX\\PhysX_64")
    #pragma comment(lib, "PhysX\\PhysXCommon_64")
    #pragma comment(lib, "PhysX\\PhysXCooking_64")
    #pragma comment(lib, "PhysX\\PhysXExtensions_static_64")
    #pragma comment(lib, "PhysX\\PhysXFoundation_64")
    #pragma comment(lib, "PhysX\\PhysXPvdSDK_static_64")
    #pragma comment(lib, "PhysX\\PhysXVehicle_static_64")
    #pragma comment(lib, "PhysX\\PhysXCharacterKinematic_static_64")
#endif

// Fbx Loader
#include <FBXLoader\\fbxsdk.h>
#ifdef _DEBUG
    #pragma comment(lib, "FBXLoader\\libfbxsdk-md_debug")
#else
    #pragma comment(lib, "FBXLoader\\libfbxsdk-md")
#endif

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"

#include "COutputLog.h"
