#include "pch.h"
#include "Test.h"

#include "CDevice.h"

// 삼각형 그리기
Vtx g_vtx[3] = {};

// 정점을 저장하는 정점버퍼
ComPtr<ID3D11Buffer> g_VB = nullptr;

ComPtr<ID3D11VertexShader> g_VS = nullptr;
ComPtr<ID3D11PixelShader> g_PS = nullptr;

int TestInit()
{
    // 전역변수에 삼각형 위치 설정
    //     0
    //   /   \
	//  2 --- 1
    g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
    g_vtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    g_vtx[0].vUV = Vec2(0.f, 0.f);

    g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
    g_vtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    g_vtx[1].vUV = Vec2(0.f, 0.f);

    g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
    g_vtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    g_vtx[2].vUV = Vec2(0.f, 0.f);

    // 버텍스 버퍼 생성
    D3D11_BUFFER_DESC BufferDesc = {};

    BufferDesc.ByteWidth = sizeof(Vtx) * 3;         // 버텍스 3개 저장할 버퍼
    BufferDesc.StructureByteStride = sizeof(Vtx);   // 버텍스 1개의 크기
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    // g_Vtx 배열의 데이터를 초기 데이터로 설정
    D3D11_SUBRESOURCE_DATA tSubData = {};
    tSubData.pSysMem = g_vtx;

    // 버텍스 버퍼 생성
    if (FAILED(DEVICE->CreateBuffer(&BufferDesc, &tSubData, g_VB.GetAddressOf())))
    {
        MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"TestInit 오류", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void TestProgress()
{
    float ClearColor[4] = {0.3f, 0.3f, 0.3f, 1.f};
    CDevice::GetInst()->ClearRenderTarget(ClearColor);

    //// 삼각형 그리기
    //UINT iStride = 0;
    //UINT iOffset = 0;
    //CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);

    //CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
    //CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

    //CONTEXT->Draw(3, 0);

    CDevice::GetInst()->Present();
}

void TestRelease()
{
}
