#pragma once
#include "CShader.h"

struct tScalarParam
{
    SCALAR_PARAM Type;
    string Desc;
};

struct tTexParam
{
    TEX_PARAM Type;
    string Desc;
};

class CGraphicsShader : public CShader
{
private:
    ComPtr<ID3DBlob> m_VSBlob;
    ComPtr<ID3DBlob> m_HSBlob;
    ComPtr<ID3DBlob> m_DSBlob;
    ComPtr<ID3DBlob> m_GSBlob;
    ComPtr<ID3DBlob> m_PSBlob;

    ComPtr<ID3D11VertexShader> m_VS;
    ComPtr<ID3D11HullShader> m_HS;
    ComPtr<ID3D11DomainShader> m_DS;
    ComPtr<ID3D11GeometryShader> m_GS;
    ComPtr<ID3D11PixelShader> m_PS;

    ComPtr<ID3D11InputLayout> m_Layout;

    D3D11_PRIMITIVE_TOPOLOGY m_Topology;

    // Rasterizer State
    RS_TYPE m_RSType;

    // OutputMerge
    // DepthStencilState
    DS_TYPE m_DSType;

    // BlendState
    BS_TYPE m_BSType;

    // Shader Domain
    SHADER_DOMAIN m_Domain;

    // Shader 파라미터 목록
    vector<tScalarParam> m_ScalarParam;
    vector<tTexParam> m_TexParam;

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateHullShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateDomainShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
    void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

    D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return m_Topology; }
    RS_TYPE GetRSType() const { return m_RSType; }
    DS_TYPE GetDSType() const { return m_DSType; }
    BS_TYPE GetBSType() const { return m_BSType; }
    SHADER_DOMAIN GetDomain() const { return m_Domain; }

    void AddScalarParam(SCALAR_PARAM _Param, const string& _Desc) { m_ScalarParam.push_back(tScalarParam{_Param, _Desc}); }
    void AddTexParam(TEX_PARAM _Param, const string& _Desc) { m_TexParam.push_back(tTexParam{_Param, _Desc}); }
    const vector<tScalarParam>& GetScalarParam() const { return m_ScalarParam; }
    const vector<tTexParam>& GetTexParam() const { return m_TexParam; }

public:
    virtual int UpdateData() override;

public:
    CGraphicsShader();
    virtual ~CGraphicsShader();
};
