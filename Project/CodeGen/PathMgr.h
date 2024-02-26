#pragma once

class CPathMgr
{
private:
	static wchar_t g_szSolutionPath[255];
	static wchar_t g_szResPath[255];
	static wchar_t g_szIncPath[255];
	static wchar_t g_szProjPath[255];
	static wchar_t g_szRelativePath[255];
	static wchar_t g_szFile[255];
	static wchar_t g_szExt[50];

public:
	static void init();
	static wchar_t* GetResPath();
	static wchar_t* GetIncludePath();
	static wchar_t* GetProjectPath();
	static wchar_t* GetFileName(const wchar_t* _strPath);
	static wchar_t* GetExt(const wchar_t* _strPath);
	static wchar_t * GetRelativePath(const wchar_t * _pFullPath);
	static wchar_t* GetSolutionPath();

private:
	CPathMgr() {};
	~CPathMgr() {};
};


