#include "pch.h"
#include "CContentBrowser.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

CContentBrowser::CContentBrowser()
    : m_CurrentDirectory()
    , m_DirectoryIcon(nullptr)
    , m_FileIcon(nullptr)
{
}

CContentBrowser::~CContentBrowser()
{
}

void CContentBrowser::init()
{
    m_CurrentDirectory = CPathMgr::GetContentPath();
    m_DirectoryIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DirectoryIconTex");
    m_FileIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FileIconTex");
}

void CContentBrowser::render()
{
    ImGui_SetWindowClass_LevelEditor();
    ImGui::Begin("Content Browser");

    // 뒤로가기 버튼
    if (m_CurrentDirectory != std::filesystem::path(CPathMgr::GetContentPath()))
    {
        if (ImGui::ArrowButton("##left", ImGuiDir_Left))
        {
            // 예외처리 - L"\\" 가 붙어있는 경로라면 L"\\" 를 제거
            if (!m_CurrentDirectory.has_stem())
                m_CurrentDirectory = m_CurrentDirectory.parent_path();

            m_CurrentDirectory = m_CurrentDirectory.parent_path();
            m_CurrentDirectory += L"\\";
        }

        ImGui::Separator();
    }

    // FileName
    ImGui_InputText("Current Directory", m_CurrentDirectory.string());

    static float padding = 16.0f;
    static float thumbnailSize = 110.0f;

    ImGui::Columns(1);
    ImGui::SliderFloat(ImGui_LabelPrefix("Thumbnail Size").c_str(), &thumbnailSize, 16, 256);
    ImGui::SliderFloat(ImGui_LabelPrefix("Padding").c_str(), &padding, 0, 32);

    ImGui::Separator();

    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto& path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, CPathMgr::GetContentPath());
        string filenameString = relativePath.filename().string();

        ImGui::PushID(filenameString.c_str());

        Ptr<CTexture> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton((void*)icon->GetSRV().Get(), {thumbnailSize, thumbnailSize});

        // Drag & Drop
        if (ImGui::BeginDragDropSource())
        {
            ImGui::Text("%s", filenameString.c_str(), filenameString.size());

            std::filesystem::path fileNamePath = filenameString;

            // 텍스춰 포맷이면 로딩
            if (fileNamePath.extension() == ".png" || fileNamePath.extension() == ".PNG" ||
                fileNamePath.extension() == ".bmp" || fileNamePath.extension() == ".BMP" ||
                fileNamePath.extension() == ".jpg" || fileNamePath.extension() == ".JPG" ||
                fileNamePath.extension() == ".jpeg" || fileNamePath.extension() == ".JPEG")
            {
                CAssetMgr::GetInst()->Load<CTexture>(relativePath.filename().wstring(), relativePath);
            }

            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", filenameString.c_str(), filenameString.size());
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
                m_CurrentDirectory /= path.filename();
        }
        ImGui::TextWrapped(filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::End();
}
