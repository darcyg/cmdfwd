#include <iostream>
#include <fstream>
using namespace std;
#include "windows.h"
// cmdfwd.cpp : 定义控制台应用程序的入口点。
//

DWORD RunCmd(wofstream & fout, wstring & strCmdLine)
{
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hRead;
	HANDLE hWrite;

	fout << "Running: " << strCmdLine.c_str() << endl;
	if (CreatePipe(&hRead, &hWrite, &sa, 0) == FALSE)
	{
		wcout << L"create pipe failed!" << endl;
		fout << "create pipe failed!" << endl;
		fout.flush();
		fout.close();

		return 0;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	wchar_t * pName = new wchar_t[strCmdLine.size() + 256];
	wcscpy_s(pName, strCmdLine.size() + 1, strCmdLine.c_str());

	if (CreateProcess(0, pName, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
	{
		ERROR_SUCCESS;
		wchar_t buf[1024];
		DWORD dwErr = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buf, 1024, 0);
		wcout << L"create proc failed!" << endl;
		fout << "create proc failed!" << endl;
		delete[] pName;

		return 0;

	}
	delete[] pName;

	CloseHandle(hWrite);

	wchar_t buf[1024] = L"";
	char abuf[1024];
	DWORD dwRead;

	while (ReadFile(hRead, abuf, 1023, &dwRead, NULL) != NULL)
	{
		MultiByteToWideChar(CP_UTF8, 0, abuf, dwRead, buf, 1024);
		fout.write(buf, dwRead);
		cout.write(abuf, dwRead);
		Sleep(100);
	}

	CloseHandle(hRead);

	DWORD dwRet;
	GetExitCodeProcess(pi.hProcess, &dwRet);

	return dwRet;
}


int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	wstring strFileName = argv[0];
	size_t nPos = strFileName.find_last_of('.');
	strFileName = strFileName.substr(0, nPos + 1) + L"log";
	wofstream fout(strFileName.c_str());
	fout.imbue(std::locale("CHS"));
	for (auto i = 0; i < argc; ++i)
	{
		wcout << argv[i] << ' ';
		fout << argv[i] << ' ';
	}
	wcout << endl;
	fout << endl;
	wcout << L"env:" << endl;
	fout << L"env:" << endl;
	wchar_t sCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, sCurDir);
	wcout << L"Cur Dir: \"" << sCurDir << L"\"" << endl;
	fout << L"Cur Dir: \"" << sCurDir << L"\"" << endl;

	for (auto i = 0; i < argc; ++i)
	{
		wcout << envp[i] << ' ';
		fout << envp[i] << ' ';
	}
	wcout << endl;
	fout << endl;

	strFileName = argv[0];
	nPos = strFileName.find_last_of('.');
	strFileName.insert(nPos, L"_j");
	strFileName = L'\"' + strFileName + L'\"';

	if (argc > 1)
	{
		for (auto i = 1; i < argc; ++i)
		{
			strFileName += L' ';
			strFileName += argv[i];
		}
	}

	RunCmd(fout, strFileName);

	strFileName = argv[0];
	nPos = strFileName.find_last_of('.');
	strFileName.insert(nPos, L"_2");
	strFileName = L'\"' + strFileName + L'\"';

	if (argc > 1)
	{
		for (auto i = 1; i < argc; ++i)
		{
			strFileName += L' ';
			strFileName += argv[i];
		}
	}

	DWORD dwRet = RunCmd(fout, strFileName);

	fout.flush();
	fout.close();

    return dwRet;
}

