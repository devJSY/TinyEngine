#pragma once
#include "CEntity.h"
#include "CTexture.h"

class CContentBrowser : public CEntity
{
private:
    std::filesystem::path m_CurrentDirectory;

    Ptr<CTexture> m_DirectoryIcon;
    Ptr<CTexture> m_FileIcon;

public:
    void init();
    void render();

    CLONE_DISABLE(CContentBrowser);

private:
    CContentBrowser();
    virtual ~CContentBrowser();

    friend class CLevelEditor; // CLevelEditor 에서만 생성가능하도록 제한
};
