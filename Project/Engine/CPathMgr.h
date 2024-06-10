#pragma once

class CPathMgr
{
private:
    static wchar_t g_szContent[255];
    static wchar_t g_szOutput[255];

public:
    static void init();
    static const wchar_t* GetOutputPath() { return g_szOutput; }
    static const wchar_t* GetContentPath() { return g_szContent; }
    static wstring GetRelativePath(const wstring& _FullPath);
};
