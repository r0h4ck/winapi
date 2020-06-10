#include "stdafx.h"
#include <iostream>
#include <shellapi.h>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
#define CREATE_DIRS 1
#define IT_OVER 2
#define CREATE_FILE 3
#define COPY_FILES 4
#define SEARCH_FILES 5
#define OPEN_FILE 6
#define WRITE_FILE 7
#define SET_FILE_ATTR 8
#define DEL_LAST_STR 9
#define BLOCK_STR 10
#define UNLOCK_STR 11

wchar_t dir[1000];
wchar_t create_file[1000];

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void AddMenus(HWND);
void AddFields(HWND);
void CreateDirs(wstring path, HWND hWnd);
void iterate_over_system(wstring system_name, HWND hWnd);
void displayFile(wchar_t* path);
void OpenDialog(HWND hwnd);
void LoadFile(wchar_t* file);
void Create_and_Write_to_File(HWND hWnd);
DWORD get_file_size(HANDLE file);
DWORD get_last_line_size(const wchar_t* path);
DWORD get_first_line_length(const wchar_t* path);
void LockString(HANDLE hfile);
void UnLockString(HANDLE hfile);
HWND file_source, file_dest,search_field,opened_file;
HMENU hMenu;
HANDLE my_file;
wchar_t copy_source[1000], copy_destination[1000];


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASSW wc = {};
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;


	if (!RegisterClassW(&wc)) return -1;

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 800, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_COMMAND:
		switch (wp) {
		case CREATE_DIRS:
			CreateDirs(L"D:\\FILE11\\FILE12\\FILE13\\", hWnd);
			CreateDirs(L"D:\\FILE21\\FILE22\\", hWnd);
			break;
		case IT_OVER:
			iterate_over_system(L"D:\\FILE11\\FILE12\\FILE13\\", hWnd);
			break;
		case CREATE_FILE:
			wcscpy_s(create_file, 1000, dir);
			wcscat_s(create_file, 1000, L"\\FILE22\\tolik.txt");
			MessageBoxW(hWnd, create_file, L"The folder was created", MB_OK);
			my_file = CreateFile(create_file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;

		case SEARCH_FILES:
			wchar_t place_to_search[1000];
			bool not_found;
			GetWindowText(search_field, place_to_search, 1000);
			wcscat_s(place_to_search, 1000, L"\\?i*.*");
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;
			hFind = FindFirstFileW(place_to_search, &FindFileData);



			while (FindNextFileW(hFind, &FindFileData) != 0) {
				not_found = false;
				MessageBox(hWnd, FindFileData.cFileName, L"Found", MB_OK);
			}

			if(not_found) MessageBox(hWnd, L"There is no such files", L"Found", MB_OK);

			FindClose(hFind);

			break;

		case OPEN_FILE:
			OpenDialog(hWnd);
			break;
		case WRITE_FILE:
			Create_and_Write_to_File(hWnd);
			break;

		case SET_FILE_ATTR:
			wchar_t hidden_path[1000];
			GetFinalPathNameByHandleA(my_file, (LPSTR)hidden_path, 1000, FILE_NAME_NORMALIZED);
			SetFileAttributesA((LPCSTR)hidden_path, FILE_ATTRIBUTE_HIDDEN);
			DWORD error;
			FILETIME ft;
			SYSTEMTIME setTime;
			setTime.wYear = 2008;
			setTime.wMonth = 1;
			setTime.wDayOfWeek = 0;
			setTime.wDay = 1;
			setTime.wHour = 12;
			setTime.wMinute = 0;
			setTime.wSecond = 0;
			setTime.wMilliseconds = 0;
			SystemTimeToFileTime(&setTime, &ft);
			if (SetFileTime(my_file, &ft, (LPFILETIME)NULL, (LPFILETIME)NULL) == 0) error = GetLastError();
			break;

		case DEL_LAST_STR:
			SetFilePointer(my_file, get_file_size(my_file) - get_last_line_size(L"D:\\FILE21\\FILE22\\tolik.txt"), 0, FILE_BEGIN);
			SetEndOfFile(my_file);
			break;

		case BLOCK_STR:
			LockString(my_file);
			break;

		case UNLOCK_STR:
			UnLockString(my_file);
			break;

		case COPY_FILES:
			SHFILEOPSTRUCTW s = { 0 };

			GetWindowText(file_source, copy_source, 100);
			GetWindowText(file_dest, copy_destination, 100);

			s.hwnd = hWnd;
			s.wFunc = FO_COPY;
			s.fFlags = FOF_SIMPLEPROGRESS;
			s.pTo = copy_destination;
			s.pFrom = copy_source;
			SHFileOperation(&s);

			break;



		}
		break;



	case WM_CREATE:
		AddFields(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}



void AddFields(HWND hWnd) {

	CreateWindowW(L"Button", L"Create Dirs", WS_VISIBLE | WS_CHILD, 0, 0, 98, 38, hWnd, (HMENU)CREATE_DIRS, NULL, NULL);
	CreateWindowW(L"Button", L"Iterate", WS_VISIBLE | WS_CHILD, 130, 0, 98, 38, hWnd, (HMENU)IT_OVER, NULL, NULL);
	CreateWindowW(L"Button", L"Create File", WS_VISIBLE | WS_CHILD, 260, 0, 98, 38, hWnd, (HMENU)CREATE_FILE, NULL, NULL);
	file_source = CreateWindowW(L"Edit", L"", ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, 370, 0, 120, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Copy", WS_VISIBLE | WS_CHILD, 530, 0, 98, 38, hWnd, (HMENU)COPY_FILES, NULL, NULL);
	file_dest = CreateWindowW(L"Edit", L"", ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, 650, 0, 120, 40, hWnd, NULL, NULL, NULL);
	search_field = CreateWindowW(L"Edit", L"", ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, 0, 75, 120, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Search", WS_VISIBLE | WS_CHILD, 120, 75, 98, 38, hWnd, (HMENU)SEARCH_FILES, NULL, NULL);
	CreateWindowW(L"Button", L"Open File", WS_VISIBLE | WS_CHILD, 250, 75, 98, 38, hWnd, (HMENU)OPEN_FILE, NULL, NULL);
	opened_file = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL, 350, 75, 400, 300, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Write", WS_VISIBLE | WS_CHILD, 450, 400, 98, 38, hWnd, (HMENU)WRITE_FILE, NULL, NULL);
	CreateWindowW(L"Button", L"hidden", WS_VISIBLE | WS_CHILD, 0, 400, 98, 38, hWnd, (HMENU)SET_FILE_ATTR, NULL, NULL);
	CreateWindowW(L"Button", L"Delete str", WS_VISIBLE | WS_CHILD, 200, 400, 98, 38, hWnd, (HMENU)DEL_LAST_STR, NULL, NULL);
	CreateWindowW(L"Button", L"Block str", WS_VISIBLE | WS_CHILD, 600, 400, 98, 38, hWnd, (HMENU)BLOCK_STR, NULL, NULL);
	CreateWindowW(L"Button", L"Unlock str", WS_VISIBLE | WS_CHILD, 600, 600, 98, 38, hWnd, (HMENU)UNLOCK_STR, NULL, NULL);


}

void CreateDirs(wstring path, HWND hWnd) {

	for (int i = 0; i < path.length(); i++)
		if (path[i] == '\\') {
			CreateDirectoryW(path.substr(0, i).c_str(), NULL);
			MessageBoxW(hWnd, path.substr(0, i).c_str(), L"The folder was created", MB_OK);
		}


}

void iterate_over_system(wstring system_name, HWND hWnd) {
	int counter = 0;

	for (int i = 0; i < system_name.length(); i++)
		if (system_name[i] == '\\') {
			counter++;
			SetCurrentDirectory(system_name.substr(0, i).c_str());
			GetCurrentDirectory(1000, dir);
			MessageBoxW(hWnd, dir, L"The folder was created", MB_OK);
		}

	for (int i = 0; i < counter; i++) {
		SetCurrentDirectory(L"..");
		GetCurrentDirectory(1000, dir);
		MessageBoxW(hWnd, dir, L"The folder was created", MB_OK);
	}
	SetCurrentDirectory(L"FILE21");
	GetCurrentDirectory(1000, dir);
	MessageBoxW(hWnd, dir, L"The folder was created", MB_OK);


}

void OpenDialog(HWND hwnd) {

	OPENFILENAME ofn;
	wchar_t szFile[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("Text files\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
		LoadFile(ofn.lpstrFile);
}

void LoadFile(wchar_t* file) {

	HANDLE hFile;
	DWORD dwSize;
	DWORD dw;

	LPBYTE lpBuffer = NULL;

	hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	dwSize = GetFileSize(hFile, NULL);
	lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(),
		HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
	ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);
	CloseHandle(hFile);
	lpBuffer[dwSize] = 0;
	SetWindowText(opened_file, (LPCWSTR)lpBuffer);
	HeapFree(GetProcessHeap(), 0, lpBuffer);
}

void Create_and_Write_to_File(HWND hWnd) {

	const DWORD len = GetWindowTextLength(opened_file) + 1;
	char* text = new char[len];

	DWORD bytes_written = 0;
	DWORD error;
	bool bfile;
	GetWindowTextA(opened_file, text, len);
	bfile = WriteFile(my_file, text, len, &bytes_written, NULL);
	if (bfile == FALSE) error = GetLastError();

}

DWORD get_file_size(HANDLE file) {
	return GetFileSize(file, NULL);
}

DWORD get_last_line_size(const wchar_t* path) {
	string result = "";
	ifstream fin(path);

	if (fin.is_open()) {
		fin.seekg(0, std::ios_base::end);
		char ch = ' ';
		while (ch != '\n') {
			fin.seekg(-2, std::ios_base::cur);

			if ((int)fin.tellg() <= 0) {
				fin.seekg(0);
				break;
			}
			fin.get(ch);
		}

		std::getline(fin, result);
		fin.close();


	}
	DWORD leen = result.size();
	return leen;

}


DWORD get_first_line_length(const wchar_t* path) {
	ifstream infile(path);
	DWORD first_strlen = 0;
	if (infile.good())
	{
		string sLine;
		getline(infile, sLine);
		first_strlen = sLine.length();
	}

	infile.close();
	return first_strlen;
}

void LockString(HANDLE hfile) {

	wchar_t path[1000];
	GetFinalPathNameByHandleA(my_file, (LPSTR)path, 1000, FILE_NAME_NORMALIZED);

	DWORD first_line = get_first_line_length(path);
	DWORD last_line = get_last_line_size(path);
	DWORD file_size = get_file_size(hfile) - last_line;

	OVERLAPPED first_over;
	first_over.Offset = 0;
	first_over.OffsetHigh = 0;

	OVERLAPPED last_over;
	last_over.Offset = file_size;
	last_over.OffsetHigh = 0;

	LockFileEx(hfile,LOCKFILE_EXCLUSIVE_LOCK,0,first_line,0,&first_over);
	LockFileEx(hfile,LOCKFILE_EXCLUSIVE_LOCK,0,last_line,0,&last_over);
}

void UnLockString(HANDLE hfile) {
	wchar_t path[1000];
	GetFinalPathNameByHandleA(my_file, (LPSTR)path, 1000, FILE_NAME_NORMALIZED);

	DWORD first_line = get_first_line_length(path);
	DWORD last_line = get_last_line_size(path);
	DWORD file_size = get_file_size(hfile) - last_line;

	OVERLAPPED first_over;
	first_over.Offset = 0;
	first_over.OffsetHigh = 0;

	OVERLAPPED last_over;
	last_over.Offset = file_size;
	last_over.OffsetHigh = 0;


	UnlockFileEx(hfile,0,first_line,0,&first_over);
	UnlockFileEx(hfile,0,last_line,0,&last_over);


}
