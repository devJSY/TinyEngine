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
    m_DirectoryIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DirectoryIcon");
    m_FileIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FileIcon");
}

void CContentBrowser::render()
{
    ImGui::Begin("Content Browser");

    // ←
    if (m_CurrentDirectory != std::filesystem::path(CPathMgr::GetContentPath()))
    {
        if (ImGui::Button("<-"))
        {
            // 예외처리 - L"\\" 가 붙어있는 경로라면 L"\\" 를 제거
            if (!m_CurrentDirectory.has_stem())
                m_CurrentDirectory = m_CurrentDirectory.parent_path();

            m_CurrentDirectory = m_CurrentDirectory.parent_path();
            m_CurrentDirectory += L"\\";
        }
    }

    // FileName
    {
        std::string name = m_CurrentDirectory.string();

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        ImGui::InputText("Current Directory", buffer, sizeof(buffer));
    }

    static float padding = 16.0f;
    static float thumbnailSize = 110.0f;

    ImGui::Columns(1);
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 256);
    ImGui::SliderFloat("Padding", &padding, 0, 32);

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
        std::string filenameString = relativePath.filename().string();

        ImGui::PushID(filenameString.c_str());

        Ptr<CTexture> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton((void*)icon->GetSRV().Get(), {thumbnailSize, thumbnailSize});

        // Drag & Drop
        if (ImGui::BeginDragDropSource())
        {
            const char* filname = filenameString.c_str();
            ImGui::Text("%s", filname, filenameString.size());

            CAssetMgr::GetInst()->Load<CTexture>(relativePath.filename().wstring(), relativePath);

            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", filname, filenameString.size());
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

    // TODO: status bar
    ImGui::End();
}
