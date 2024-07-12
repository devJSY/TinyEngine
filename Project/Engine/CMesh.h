#pragma once
#include "CAsset.h"

#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
    ComPtr<ID3D11Buffer> pIB;
    D3D11_BUFFER_DESC tIBDesc;
    UINT iIdxCount;
    void* pIdxSysMem;
    wstring IBName;
};

class CMesh : public CAsset
{
private:
    ComPtr<ID3D11Buffer> m_VB;
    D3D11_BUFFER_DESC m_VBDesc;
    UINT m_VtxCount;
    void* m_VtxSysMem;

    // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
    vector<tIndexInfo> m_vecIdxInfo;

    // Animator 정보
    vector<tMTAnimClip> m_vecAnimClip;
    vector<tMTBone> m_vecBones;
    vector<tBoneSocket*> vecBoneSocket; // Bone이 보유한 모든 Bone Socket

    CStructuredBuffer* m_pBoneFrameData; // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
    CStructuredBuffer* m_pBoneOffset;    // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)

private:
    void UpdateData(UINT _iSubset);
    void UpdateData_Inst(UINT _iSubset);

public:
    UINT GetVtxCount() const { return m_VtxCount; }
    Vtx* GetVtxSysMem() const { return (Vtx*)m_VtxSysMem; }
    UINT GetSubsetCount() const { return (UINT)m_vecIdxInfo.size(); }
    wstring GetIBName(UINT _Idx) const { return m_vecIdxInfo[_Idx].IBName; }

public:
    bool IsSkeletalMesh() const { return !m_vecBones.empty(); }

    const vector<tMTAnimClip>* GetAnimClip() const { return &m_vecAnimClip; }
    bool IsAnimMesh() const { return !m_vecAnimClip.empty(); }

    const vector<tMTBone>* GetBones() const { return &m_vecBones; }
    UINT GetBoneCount() const { return (UINT)m_vecBones.size(); }

    void AddBoneSocket(int _BoneIndex, tBoneSocket* _BoneSocket);
    void RemoveBoneSocket(int _BoneIndex, tBoneSocket* _BoneSocket);

    const vector<tBoneSocket*>& GetvecBoneSocket() const { return vecBoneSocket; }
    tBoneSocket* GetBoneSocket(const wstring& _SocketName);

    CStructuredBuffer* GetBoneFrameDataBuffer() const { return m_pBoneFrameData; } // 전체 본 프레임 정보
    CStructuredBuffer* GetBoneOffsetBuffer() const { return m_pBoneOffset; }       // 각 뼈의 offset 행렬

public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);

    void render(UINT _iSubset);
    void render_draw(UINT _iSubset);
    void render_IndexedInstanced(UINT _InstanceCount);
    void render_instancing(UINT _iSubset);

public:
    virtual int Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;

    CLONE_DISABLE(CMesh);

public:
    CMesh(bool _bEngineAsset = false);
    virtual ~CMesh();
};
