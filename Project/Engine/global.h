#pragma once

#include <stdlib.h>
#include <Windows.h>

#include <wrl.h>
using namespace Microsoft::WRL;

// Directx 11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"