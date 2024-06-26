#include "pch.h"
#include "CModelEditor.h"

#include "CGameObjectEx.h"

#include "CMeshRender.h"

#include "CAnimator.h"

CModelEditor::CModelEditor()
    : CEditor(EDITOR_TYPE::MODEL)
    , m_ModelObj(nullptr)
    , m_SelectedBoneIdx(-1)
    , m_FinalBoneMat{}
{
}

CModelEditor::~CModelEditor()
{
    if (nullptr != m_ModelObj)
    {
        delete m_ModelObj;
        m_ModelObj = nullptr;
    }
}

void CModelEditor::init()
{
}

void CModelEditor::tick()
{
    if (nullptr == m_ModelObj)
        return;

    m_ModelObj->tick();
}

void CModelEditor::finaltick()
{
    if (nullptr == m_ModelObj)
        return;

    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pComp = m_ModelObj->GetComponent((COMPONENT_TYPE)i);
        if (nullptr != pComp)
        {
            if (i == (UINT)COMPONENT_TYPE::ANIMATOR)
            {
                m_ModelObj->Animator()->finaltick_ModelEditor();
            }
            else
            {
                pComp->finaltick();
            }
        }
    }

    UINT BoneCount = m_ModelObj->Animator()->GetBoneCount();

    if (m_FinalBoneMat.size() != BoneCount)
    {
        m_FinalBoneMat.resize(BoneCount);
    }

    // 최종 Bone 행렬을 받아온다.
    m_ModelObj->Animator()->GetFinalBoneMat()->GetData(m_FinalBoneMat.data(), BoneCount);
}

void CModelEditor::render(bool* open)
{
    // =====================================
    // DockSpace
    // =====================================
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetMainViewport()->ID;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockingAllowUnclassed = true;
    ImGui::SetNextWindowClass(&window_class);

    if (!ImGui::Begin(ToString(GetName()).c_str(), open))
    {
        ImGui::End();
        return;
    }

    ImGuiID dockSpace = ImGui::GetID("Model Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Model Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CModelEditor::render()
{
    ImGui_SetWindowClass(GetEditorType());
    DrawViewport();

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();

    ImGui_SetWindowClass(GetEditorType());
    DrawSkeletonTree();

    ImGui_SetWindowClass(GetEditorType());
    DrawAnimation();
}

void CModelEditor::DrawViewport()
{
    ImGui::Begin("Viewport##ModelEditor");
    ImGui::End();
}

void CModelEditor::DrawDetails()
{
    ImGui::Begin("Details##ModelEditor");

    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                                    ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    // ==========================
    // Mesh Data
    // ==========================
    if (ImGui::TreeNodeEx("Mesh Data##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        string ModelName = string();
        if (nullptr != m_ModelObj)
        {
            ModelName = ToString(m_ModelObj->GetName());
        }

        const map<wstring, Ptr<CAsset>>& mapMeshData = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::MESHDATA);

        if (ImGui_ComboUI(ImGui_LabelPrefix("Mesh Data").c_str(), ModelName, mapMeshData))
        {
            Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(ToWstring(ModelName));

            if (nullptr != pMeshData)
            {
                ReleaseModel();
                m_ModelObj = pMeshData->InstantiateEx();
                m_ModelObj->MeshRender()->SetFrustumCheck(false);
                m_ModelObj->Animator()->SetPlay(false);
            }
        }

        if (ImGui_AlignButton("Load Model", 1.f))
        {
            std::filesystem::path filePath = OpenFileDialog(L"fbx\\", TEXT("FBX Files\0*.fbx\0모든 파일(*.*)\0*.*\0"));

            // .fbx 포맷이 아닌 경우
            if (L".fbx" != filePath.extension())
            {
                MessageBox(nullptr, L"fbx 포맷 파일이 아닙니다.", L"모델 로딩 실패", MB_OK);
            }
            // 경로에 Content 폴더가 포함되지 않은 경우
            else if (string::npos == wstring(filePath).find(CPathMgr::GetContentPath()))
            {
                MessageBox(nullptr, L"Content 폴더에 존재하는 모델이 아닙니다.", L"모델 로딩 실패", MB_OK);
            }
            // 취소, 닫기 버튼 체크를 클릭하지 않은 경우
            else if (!filePath.empty())
            {
                Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->LoadFBX(filePath.lexically_relative(CPathMgr::GetContentPath()));

                if (nullptr != pMeshData)
                {
                    ReleaseModel();
                    m_ModelObj = pMeshData->InstantiateEx();
                    m_ModelObj->MeshRender()->SetFrustumCheck(false);
                    m_ModelObj->Animator()->SetPlay(false);
                }
            }
        }

        ImGui::TreePop();
    }

    // ==========================
    // Bone
    // ==========================
    if (ImGui::TreeNodeEx("Bone##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj && -1 < m_SelectedBoneIdx)
        {
            const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);
            ImGui_InputText("Bone Name", ToString(CurBone.strBoneName).c_str());
        }

        ImGui::TreePop();
    }

    // ==========================
    // Transforms
    // ==========================
    if (ImGui::TreeNodeEx("Transforms##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj && -1 < m_SelectedBoneIdx)
        {
            const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);

            // Bone
            if (ImGui::TreeNodeEx("Bone##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                Vec3 pos = Vec3();
                Vec3 rot = Vec3();
                Vec3 scale = Vec3();
                ImGuizmo::DecomposeMatrixToComponents(*m_FinalBoneMat[m_SelectedBoneIdx].m, pos, rot, scale);

                ImGui_DrawVec3Control("Location", pos, 10.f);
                ImGui_DrawVec3Control("Rotation", rot, 1.f);
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }

            // Offset
            if (ImGui::TreeNodeEx("Offset##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);

                Vec3 pos = Vec3();
                Vec3 rot = Vec3();
                Vec3 scale = Vec3();
                ImGuizmo::DecomposeMatrixToComponents(*CurBone.matOffset.m, pos, rot, scale);

                ImGui_DrawVec3Control("Location", pos, 10.f);
                ImGui_DrawVec3Control("Rotation", rot, 1.f);
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void CModelEditor::DrawSkeletonTree()
{
    ImGui::Begin("Skeleton Tree##ModelEditor");

    if (nullptr != m_ModelObj)
    {
        int NodeOpenFlag = 0;
        if (ImGui::Button("Expand All"))
        {
            NodeOpenFlag = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Collapse All"))
        {
            NodeOpenFlag = -1;
        }

        ImGui::Separator();

        SkeletonRe(*m_ModelObj->MeshRender()->GetMesh()->GetBones(), 0, NodeOpenFlag);
    }

    ImGui::End();
}

void CModelEditor::SkeletonRe(const vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag)
{
    if (-1 == _NodeOpenFlag)
    {
        ImGui::SetNextItemOpen(false);
    }
    else if (1 == _NodeOpenFlag)
    {
        ImGui::SetNextItemOpen(true);
    }

    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx(ToString(_vecBone[_BoneIdx].strBoneName).c_str(),
                                    m_SelectedBoneIdx == _BoneIdx ? ImGuiTreeNodeFlags_Selected : 0 | DefaultTreeNodeFlag);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_SelectedBoneIdx = _BoneIdx;
    }

    if (opened)
    {
        for (UINT i = 0; i < (UINT)_vecBone.size(); ++i)
        {
            // 본인의 BoneIdx과 부모의 BoneIdx가 같은 경우
            if (i == _BoneIdx)
                continue;

            if (_BoneIdx == _vecBone[i].iParentIndx)
            {
                SkeletonRe(_vecBone, i, _NodeOpenFlag);
            }
        }

        ImGui::TreePop();
    }
}

void CModelEditor::DrawAnimation()
{
    ImGui::Begin("Animation##ModelEditor");
    if (nullptr != m_ModelObj && m_ModelObj->Animator())
    {
        static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                                        ImGuiTreeNodeFlags_FramePadding;

        CAnimator* pAnimator = m_ModelObj->Animator();
        Ptr<CMesh> pSkeletalMesh = m_ModelObj->Animator()->GetSkeletalMesh();

        // Skeletal Mesh
        if (ImGui::TreeNodeEx("Skeletal Mesh##ModelEditor Animation", DefaultTreeNodeFlag))
        {
            string name;
            if (nullptr != pSkeletalMesh)
                name = ToString(pSkeletalMesh->GetName());

            ImGui_InputText("Skeletal Mesh", name);

            ImGui::TreePop();
        }

        // Animation
        if (nullptr != pSkeletalMesh)
        {
            if (ImGui::TreeNodeEx("Animation##ModelEditor Animation", DefaultTreeNodeFlag))
            {
                // =====================
                // Animations
                // =====================
                const vector<tMTAnimClip>* vecAnimClip = pSkeletalMesh->GetAnimClip();

                int CurClipIdx = pAnimator->GetCurClipIdx();
                const tMTAnimClip& CurClip = vecAnimClip->at(CurClipIdx);

                string CurClipName = ToString(CurClip.strAnimName);

                int ChangedClipIdx = -1;
                ImGui::Text("Animations");
                if (ImGui::BeginCombo("##Anim", CurClipName.c_str()))
                {
                    for (int i = 0; i < vecAnimClip->size(); i++)
                    {
                        string ClipName = ToString(vecAnimClip->at(i).strAnimName);
                        bool is_selected = (CurClipName == ClipName);
                        if (ImGui::Selectable(ClipName.c_str(), is_selected))
                        {
                            CurClipName = ClipName;
                            ChangedClipIdx = i;
                        }

                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ChangedClipIdx >= 0)
                {
                    pAnimator->SetCurClipIdx(ChangedClipIdx);
                }

                // Frame Index
                int FrameIdx = pAnimator->GetCurFrameIdx();
                if (ImGui::SliderInt(ImGui_LabelPrefix("Frame Index").c_str(), &FrameIdx, CurClip.iStartFrame, CurClip.iEndFrame))
                {
                    pAnimator->SetFrameIdx(FrameIdx);
                }

                bool bPlaying = pAnimator->IsPlaying();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Play").c_str(), &bPlaying))
                    pAnimator->SetPlay(bPlaying);

                bool bRepeat = pAnimator->IsRepeat();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Repeat").c_str(), &bRepeat))
                    pAnimator->SetRepeat(bRepeat);

                float PlaySpeed = pAnimator->GetPlaySpeed();
                if (ImGui::DragFloat(ImGui_LabelPrefix("Play Speed").c_str(), &PlaySpeed, 0.01f, 0.f, 100.f))
                    pAnimator->SetPlaySpeed(PlaySpeed);

                // =====================
                // Animation Clip Info
                // =====================
                ImGui::Text("Clip Infomation");
                static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                               ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame;
                if (ImGui::BeginTable("##AnimClip", 3, flags, ImVec2(600.f, 150.f)))
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Animation Name");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", ToString(CurClip.strAnimName).c_str());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Clip Index");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClipIdx);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Rate");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", (int)pAnimator->GetFrameRate());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Range");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClip.iStartFrame);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d", CurClip.iEndFrame);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Length");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClip.iFrameLength);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Time Range");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3f", CurClip.dStartTime);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.3f", CurClip.dEndTime);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Time Length");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3f", CurClip.dTimeLength);

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}

void CModelEditor::ReleaseModel()
{
    if (nullptr != m_ModelObj)
    {
        delete m_ModelObj;
        m_ModelObj = nullptr;
    }

    // Bone 데이터 초기화
    m_SelectedBoneIdx = -1;
    m_FinalBoneMat.clear();
}
