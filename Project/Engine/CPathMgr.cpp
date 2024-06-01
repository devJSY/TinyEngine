#include "pch.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_szOutput[255] = {};
wchar_t CPathMgr::g_szContent[255] = {};

void CPathMgr::init()
{
    // 실행파일 경로
    GetCurrentDirectory(255, g_szContent);

    size_t Len = wcslen(g_szContent);

    for (size_t i = Len - 1; i >= 0; --i)
    {
        if ('\\' == g_szContent[i])
        {
            g_szContent[i + 1] = '\0';
            break;
        }
    }

    wcscpy_s(g_szOutput, g_szContent);
    wcscat_s(g_szContent, 255, L"content\\");
}

wstring CPathMgr::GetRelativePath(const wstring& _FullPath)
{
    wstring strContentPath = GetContentPath();

    // 입력된 경로에, content 경로가 없으면 실패
    if (-1 == _FullPath.find(strContentPath, 0))
        return wstring();

    // Content 경로 끝에서 문자열 끝까지를 잘라서 반환
    wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());

    return strRelativePath;
}
