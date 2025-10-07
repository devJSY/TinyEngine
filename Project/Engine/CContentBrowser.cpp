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
    m_DirectoryIcon = CAssetMgr::GetInst()->Load<CTexture>(L"Icons/ContentBrowser/DirectoryIcon.png", L"Icons/ContentBrowser/DirectoryIcon.png");
    m_FileIcon = CAssetMgr::GetInst()->Load<CTexture>(L"Icons/ContentBrowser/FileIcon.png", L"Icons/ContentBrowser/FileIcon.png");
}

void CContentBrowser::render()
{
    ImGui_SetWindowClass(EDITOR_TYPE::LEVEL);
    ImGui::Begin("Content Browser");

    // 뒤로가기 버튼
    if (m_CurrentDirectory != std::filesystem::path(CPathMgr::GetContentPath()))
    {
        if (ImGui::Button("Go Back"))
        {
            // 예외처리 - L"/" 가 붙어있는 경로라면 L"/" 를 제거
            if (!m_CurrentDirectory.has_stem())
                m_CurrentDirectory = m_CurrentDirectory.parent_path();

            m_CurrentDirectory = m_CurrentDirectory.parent_path();
            m_CurrentDirectory += L"/";
        }

        ImGui::Separator();
    }

    static float padding = 16.0f;
    static float thumbnailSize = 110.0f;

    ImGui::Columns(1);

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
        string relativePathString = relativePath.string();

        ImGui::PushID(relativePath.filename().string().c_str());

        Ptr<CTexture> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton((void*)icon->GetSRV().Get(), {thumbnailSize, thumbnailSize});

        // Drag & Drop
        if (ImGui::BeginDragDropSource())
        {
            ImGui::Text("%s", relativePathString.c_str(), relativePathString.size());

            // 텍스춰 포맷이면 로딩
            if (relativePath.extension() == ".png" || relativePath.extension() == ".PNG" || relativePath.extension() == ".bmp" ||
                relativePath.extension() == ".BMP" || relativePath.extension() == ".jpg" || relativePath.extension() == ".JPG" ||
                relativePath.extension() == ".jpeg" || relativePath.extension() == ".JPEG")
            {
                CAssetMgr::GetInst()->Load<CTexture>(relativePath, relativePath);
            }

            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePathString.c_str(), relativePathString.size());
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
                m_CurrentDirectory /= path.filename();
        }
        ImGui::TextWrapped(relativePath.filename().string().c_str());

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::End();
}
