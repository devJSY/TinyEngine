#include "pch.h"
#include "PathMgr.h"

vector<wstring> g_vecName;

int main()
{
    CPathMgr::init();
    wstring strProjPath = CPathMgr::GetProjectPath();
    wstring strCppPath = strProjPath + L"Scripts\\CScriptMgr.cpp";
    wstring strHeaderPath = strProjPath + L"Scripts\\CScriptMgr.h";

    // 1. 현재 존재하는 모든 스크립트를 알아내야함.
    wstring strScriptIncludePath = CPathMgr::GetIncludePath();
    wstring strScriptCode = strScriptIncludePath + L"Scripts\\";

    WIN32_FIND_DATA tData = {};
    HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);

    if (INVALID_HANDLE_VALUE == handle)
        return 0;

    // 예외 리스트 목록을 알아낸다.
    FILE* pExeptList = nullptr;
    _wfopen_s(&pExeptList, L"exeptlist.txt", L"r");

    vector<wstring> strExept;

    if (nullptr != pExeptList)
    {
        wchar_t szName[255] = L"";

        while (true)
        {
            int iLen = fwscanf_s(pExeptList, L"%s", szName, 255);
            if (iLen == -1)
                break;

            strExept.push_back(szName);
        }
        fclose(pExeptList);
    }

    while (true)
    {
        // 예외가 아닌경우, 스크립트 이름으로 본다.
        bool bExeption = false;
        for (size_t i = 0; i < strExept.size(); ++i)
        {
            if (!wcscmp(tData.cFileName, strExept[i].c_str()))
            {
                bExeption = true;
                break;
            }
        }

        if (!bExeption)
        {
            g_vecName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
        }

        if (!FindNextFile(handle, &tData))
            break;
    }

    FindClose(handle);

    // 이전에 CodeGen 이 실행할때 체크해둔 스크립트 목록
    FILE* pScriptListFile = nullptr;
    _wfopen_s(&pScriptListFile, L"ScriptList.txt", L"r");

    if (nullptr != pScriptListFile)
    {
        wchar_t szScriptName[50] = L"";
        vector<wstring> strCurScriptList;
        while (true)
        {
            int iLen = fwscanf_s(pScriptListFile, L"%s", szScriptName, 50);
            if (iLen == -1)
                break;

            strCurScriptList.push_back(szScriptName);
        }
        fclose(pScriptListFile);

        if (g_vecName.size() == strCurScriptList.size())
        {
            bool bSame = true;
            for (UINT i = 0; i < g_vecName.size(); ++i)
            {
                if (g_vecName[i] != strCurScriptList[i])
                {
                    // 같지 않은게 1개이상 있다
                    bSame = false;
                    break;
                }
            }

            // 이전 목록과, 현재 스크립트 목록이 완전 일치한다(변경사항 없다)
            if (bSame)
                return 0;
        }
    }

    FILE* pFile = NULL;

    //=================
    // ScriptMgr h 작성
    //=================
    _wfopen_s(&pFile, strHeaderPath.c_str(), L"w");
    fwprintf_s(pFile, L"#pragma once\n\n");
    fwprintf_s(pFile, L"#include <vector>\n");
    fwprintf_s(pFile, L"#include <string>\n\n");

    fwprintf_s(pFile, L"enum SCRIPT_TYPE\n{\n");
    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        wstring strScriptUpperName = L"";
        for (UINT j = 1; j < g_vecName[i].size(); ++j)
        {
            strScriptUpperName += toupper(g_vecName[i][j]);
        }

        fwprintf_s(pFile, L"\t");
        fwprintf_s(pFile, strScriptUpperName.c_str());
        fwprintf_s(pFile, L",\n");
    }
    fwprintf_s(pFile, L"\tEND,\n");
    fwprintf_s(pFile, L"};\n\n");

    fwprintf_s(pFile, L"using namespace std;\n\n");
    fwprintf_s(pFile, L"class CScript;\n\n");

    fwprintf_s(pFile, L"class CScriptMgr\n{\n");
    fwprintf_s(pFile, L"public:\n\tstatic void GetScriptInfo(vector<wstring>& _vec);\n");
    fwprintf_s(pFile, L"\tstatic CScript* GetScript(const wstring& _strScriptName);\n");
    fwprintf_s(pFile, L"\tstatic CScript* GetScript(UINT _iScriptType);\n");
    fwprintf_s(pFile, L"\tstatic const wchar_t* GetScriptName(CScript* _pScript);\n};\n");

    fclose(pFile);

    //====================
    // ScriptMgr cpp 작성
    //====================
    _wfopen_s(&pFile, strCppPath.c_str(), L"w");

    // 헤더 입력
    fwprintf_s(pFile, L"#include \"pch.h\"\n");
    fwprintf_s(pFile, L"#include \"CScriptMgr.h\"\n\n");

    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        fwprintf_s(pFile, L"#include \"");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L".h\"\n");
    }

    // 첫 번째 함수 작성
    fwprintf_s(pFile, L"\nvoid CScriptMgr::GetScriptInfo(vector<wstring>& _vec)\n{\n");

    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        fwprintf_s(pFile, L"\t_vec.push_back(L\"");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L"\");\n");
    }
    fwprintf_s(pFile, L"}\n\n");

    // 두번째 함수 작성
    fwprintf_s(pFile, L"CScript* CScriptMgr::GetScript(const wstring& _strScriptName)\n{\n");

    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        fwprintf_s(pFile, L"\tif (L\"");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L"\" == _strScriptName)\n");
        fwprintf_s(pFile, L"\t\treturn new ");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L";\n");
    }
    fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");

    // 세번째 함수
    fwprintf_s(pFile, L"CScript* CScriptMgr::GetScript(UINT _iScriptType)\n{\n");

    fwprintf_s(pFile, L"\tswitch (_iScriptType)\n\t{\n");
    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        wstring strScriptUpperName = L"";
        for (UINT j = 1; j < g_vecName[i].size(); ++j)
        {
            strScriptUpperName += toupper(g_vecName[i][j]);
        }

        fwprintf_s(pFile, L"\tcase (UINT)SCRIPT_TYPE::");
        fwprintf_s(pFile, strScriptUpperName.c_str());
        fwprintf_s(pFile, L":\n");

        fwprintf_s(pFile, L"\t\treturn new ");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L";\n");
    }

    fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}\n\n");

    // 네번째 함수
    fwprintf_s(pFile, L"const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)\n{\n");
    fwprintf_s(pFile, L"\tswitch ((SCRIPT_TYPE)_pScript->GetScriptType())\n\t{\n");
    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        fwprintf_s(pFile, L"\tcase SCRIPT_TYPE::");

        wstring strScriptUpperName = L"";
        for (UINT j = 1; j < g_vecName[i].size(); ++j)
        {
            strScriptUpperName += toupper(g_vecName[i][j]);
        }

        fwprintf_s(pFile, strScriptUpperName.c_str());

        fwprintf_s(pFile, L":\n\t\treturn ");
        fwprintf_s(pFile, L"L\"");
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L"\";\n");
    }

    fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}");

    fclose(pFile);

    // 다음번 실행시 비교하기위한 정보 저장
    _wfopen_s(&pFile, L"ScriptList.txt", L"w");

    for (UINT i = 0; i < g_vecName.size(); ++i)
    {
        fwprintf_s(pFile, g_vecName[i].c_str());
        fwprintf_s(pFile, L"\n");
    }

    fclose(pFile);

    return 0;
}