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

void CContentBrowser::begin()
{
    m_CurrentDirectory = CPathMgr::GetContentPath();
    m_DirectoryIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DirectoryIcon");
    m_FileIcon = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FileIcon");
}

void CContentBrowser::render()
{
    ImGui::Begin("Content Browser");

    // FileName
    {
        std::string name = m_CurrentDirectory.string();

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        ImGui::InputText("Current Directory", buffer, sizeof(buffer));
    }

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

        CTexture* icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
        ImGui::ImageButton((void*)icon->GetSRV().Get(), {thumbnailSize, thumbnailSize});
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
                m_CurrentDirectory /= path.filename();
        }
        ImGui::TextWrapped(filenameString.c_str());

        ImGui::NextColumn();
    }

    // TODO: status bar
    ImGui::End();
}
