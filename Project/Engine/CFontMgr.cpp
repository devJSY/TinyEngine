#include "pch.h"
#include "CFontMgr.h"
#include "CDevice.h"

CFontMgr::CFontMgr()
    : m_pFW1Factory(nullptr)
    , m_pFontWrapper(nullptr)
    , m_ListFont{}
{
}

CFontMgr::~CFontMgr()
{
    if (nullptr != m_pFW1Factory)
        m_pFW1Factory->Release();

    if (nullptr != m_pFontWrapper)
        m_pFontWrapper->Release();
}

void CFontMgr::init()
{
    if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
    {
        assert(NULL);
    }

    if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper)))
    {
        assert(NULL);
    }
}

void CFontMgr::render()
{
    for (const auto& iter : m_ListFont)
    {
        m_pFontWrapper->DrawString(CONTEXT,
                                   iter._pStr.c_str(), // String
                                   iter._fFontSize,    // Font size
                                   iter._fPosX,        // X position
                                   iter._fPosY,        // Y position
                                   iter._Color,        // Text color, 0xAaBbGgRr
                                   FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
        );
    }

    m_ListFont.clear();
}
