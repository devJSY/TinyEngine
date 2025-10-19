#pragma once

#include <Engine/global.h>

#ifdef _DEBUG
    #pragma comment(lib, "Engine/Engine_d.lib")
#else
    #pragma comment(lib, "Engine/Engine.lib")
#endif

// ImGui
#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// ImGuizmo
#include <ImGuizmo/ImGuizmo.h>

// ImGui Node Editor
#include <imgui_node_editor/imgui_node_editor.h>

// 에디터 헤더 참조
#include "define.h"
#include "func.h"
