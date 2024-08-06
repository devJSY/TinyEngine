#include "pch.h"
#include "CUIMoveUpDownScript.h"

CUIMoveUpDownScript::CUIMoveUpDownScript()
    : CUIAnimScript(UIMOVEUPDOWNSCRIPT)
    , m_vOriginPos{}
    , m_eState(DirState::Stop)
    , m_fUpDistance(0.f)
    , m_fSpeed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fUpDistance, "UpDistance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "UpSpeed");
}

CUIMoveUpDownScript::CUIMoveUpDownScript(const CUIMoveUpDownScript& Origin)
    : CUIAnimScript(Origin)
    , m_vOriginPos{}
    , m_eState(DirState::Stop)
    , m_fUpDistance(Origin.m_fUpDistance)
    , m_fSpeed(Origin.m_fSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fUpDistance, "UpDistance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "UpSpeed");
}

CUIMoveUpDownScript::~CUIMoveUpDownScript()
{
}

void CUIMoveUpDownScript::begin()
{
    m_vOriginPos = GetOwner()->Transform()->GetLocalPos();
}

void CUIMoveUpDownScript::tick()
{
    CUIAnimScript::tick();

    if (GetUIAnimState() == UIAnimState::Start && m_eState == DirState::Stop)
    {
        m_eState = DirState::Up;
        SetFinish(false);
    }

    switch (m_eState)
    {
    case DirState::Stop:
        break;
    case DirState::Up: {
        MoveUp();
    }
    break;
    case DirState::Down: {
        MoveDown();
    }
    break;
    case DirState::End:
        break;
    default:
        break;
    }
}

void CUIMoveUpDownScript::MoveUp()
{
    Vec3 vPos = Vec3(m_vOriginPos.x, m_vOriginPos.y + m_fUpDistance, m_vOriginPos.z);
    if (vPos.y <= MoveTransform().y)
    {
        TransformApply(vPos);
        m_eState = DirState::Down;
    }
}

void CUIMoveUpDownScript::MoveDown()
{
    Vec3 vPos = Vec3(m_vOriginPos.x, m_vOriginPos.y, m_vOriginPos.z);
    if (vPos.y > MoveTransform().y)
    {
        TransformApply(vPos);
        SetUIAnimState(UIAnimState::Tick);
        m_eState = DirState::Stop;
        SetFinish(true);
    }
}

Vec3 CUIMoveUpDownScript::MoveTransform()
{
    CTransform* pTr = GetOwner()->Transform();
    Vec3 vOrigin = pTr->GetLocalPos();
    Vec3 vDir = {};
    Vec3 vPos = {};

    switch (m_eState)
    {
    case DirState::Up: {
        vDir = Vec3(0.f, 1.f, 0.f);
    }
    break;
    case DirState::Down: {

        vDir = Vec3(0.f, -1.f, 0.f);
    }
    break;
    case DirState::End:
        break;
    default:
        break;
    }

    vPos = vOrigin + vDir * m_fSpeed * DT;

    TransformApply(vPos);

    return vPos;
}

void CUIMoveUpDownScript::TransformApply(Vec3 _vPos)
{
    CTransform* pTr = GetOwner()->Transform();
    pTr->SetLocalPos(_vPos);
}

UINT CUIMoveUpDownScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fUpDistance, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    
    MemoryByte += CUIAnimScript::SaveToLevelFile(_File);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIMoveUpDownScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    fread(&m_fUpDistance, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);

    MemoryByte += CUIAnimScript::LoadFromLevelFile(_File);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
