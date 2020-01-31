#ifndef _HELPER_H_
#define _HELPER_H_

#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0501
#define WIN32_WINNT 0x501
#define NTDDI_VERSION NTDDI_WIN2K
#define _WIN32_IE 0x0600

#include <windows.h>
#include <Winsock2.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>
#include <psapi.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <Ws2tcpip.h>
#include <shlobj.h>


/** ���������� ���������� ��������� � �������� ������� arg */
#define SIZETC(arg) (sizeof(arg)/sizeof(arg[0]))

#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif


#ifdef UNICODE
	#define lstrcmp_		lstrcmpW
	#define lstrlen_		lstrlenW
	#define lstrcat_		lstrcatW
	#define symCurDir		L"."
	#define symUpDir		L".."
	#define symSlashStr		L"\\"
	#define symSlashChar	L'\\'
#else
	#define lstrcmp_		lstrcmpA
	#define lstrlen_		lstrlenA
	#define lstrcat_		lstrcatA
	#define symCurDir		"."
	#define symUpDir		".."
	#define symSlashStr		"\\"
	#define symSlashChar	'\\'
#endif



typedef struct _WMNODE
{
	/** ����� ��������� ��������� ��������� */
	BOOL (*messageProc) (HWND hWnd, WPARAM wParam, LPARAM lParam);
	/** ��������� ���� */
	UINT message;
	/** ��������� �� ��������� ���� */
	_WMNODE* next;
} WMNODE;

/** ��������� ���������� ����� ������� ListView � ini-����� ���������.
HWND hLv - ���������� ListView
TCHAR* name - �������� �������, int column - ����� ������� ���� �������, �������
�� ���� */
BOOL saveListViewColumn (HWND hLv, TCHAR* name, int column);

/** ��������� ���������� ����� ������� ListView �� ini-����� ���������.
HWND hLv - ���������� ListView
TCHAR* name - �������� �������, int column - ����� ������� ���� �������, �������
�� ���� */
BOOL loadListViewColumn (HWND hLv, TCHAR* name, int column);


/** ������ ���������� COM-�����
unsigned int timeout - ����� �������� �� ������ ReadFile */
HANDLE openComPort (const char *name, HWND hDlg, unsigned int baud, unsigned int
	timeout, BOOL isSilent = FALSE);

/** ������ ������ �� �����.
HANDLE hFile - ���������� �����.
TCHAR* str - �������� ������.
TCHAR end - ������ ����� ������, �� ��������� '\n' */
DWORD readLine (HANDLE hFile, TCHAR* str, TCHAR end = TEXT('\n'));

void texOutBorder (HDC hdc, char* str, int x, int y);


/** ���������, ���������� �� ������ � �������� ���������.
TCHAR* str - ����������� ������.
TCHAR* subStr - ������� ���������.
������ ����������� ������ ������ ���� ������ ���
����� ������ ������� ��������� */
BOOL strstart (TCHAR* str, TCHAR* subStr);


/** ��������� ���� �������� ".."
TCHAR* curPath - ������� ����.
TCHAR* addPath - ����������� ����.
���������� HAP_DIR_DAWN, HAP_DIR_UP ��� HAP_CURDIR */
DWORD addPath (TCHAR* curPath, TCHAR* addPath);

/* ��������� ������������ ���������� addPath */
#define HAP_DIR_DOWN	1	/*< ������� �� ���� ������� ����	*/
#define HAP_DIR_UP		2	/*< ������� �� ���� ������� �����	*/
#define HAP_CURDIR		5	/*< ������� �����, ��� ��������		*/



INT CALLBACK nodeWndProc (WMNODE *wmNode, HWND hWnd, UINT msg,  WPARAM wParam, LPARAM lParam);


/** ���������� ������ � �������� ������������� */
TCHAR* uintToBinStr (unsigned int val, int bitsCount = 32, TCHAR* binChars = NULL);


int wideCharToMultiByte (wchar_t* wcStr, char* mbStr, UINT codePage = CP_ACP);
int multiByteToWideChar (wchar_t* wcStr, char* mbStr, UINT codePage = CP_ACP);


/** ���������� ��� �����, ������� ����� � ��������, ����
���� � ����� ������ ��� ���������� � ��� ������ �� ����� ����.
����� � �������� ������ ���� �������� ����� ����������� �������� "[" � "]".
char* path - ���� � �����, ���� � ����� */
BOOL getSafeFileNameForWrite (char* path);


/** ������� �������� ������ � Windows debug output
DWORD errorCode - ��� ������ ������������ GetLastError */
BOOL printdErrorStr (DWORD errorCode);


/** ������� �������� ������ � ����������� �����
DWORD errorCode - ��� ������ ������������ GetLastError */
BOOL printErrorStr (DWORD errorCode);


/** �������������� List View
HWND hLv - ���������� List View.
DWORD columns - ���������� �������.
TCHAR* headers [] - ��������� �������.
BOOL checks - ��������: TRUE - ����, FALSE - ��� */
BOOL initListViewColumns (HWND hLv, DWORD columns, TCHAR* headers [], BOOL checks = FALSE);


/** ��������� �������� �������� �� ini ����� � ComboBox
TCHAR* sectionName - �������� ������.
HWND hComboBox - ���������� ComboBox */
BOOL loadPresetsNamesFromIni (TCHAR* sectionName, TCHAR* fileName, HWND hComboBox);

/** ��������� �������� �������� �� ini ����� � ComboBox
TCHAR* sectionName - �������� ������.
HWND hComboBox - ���������� ComboBox
TCHAR *fileName - ������ ���� � ini ����� */
BOOL loadPresetsNamesFromIniEx (TCHAR* sectionName, TCHAR* fileName, HWND hComboBox);


/** ���������� ���� � ����� �������������, ����������� ��� ��
��������� � ����� ����� � ����������� �������
TCHAR* path - ���� � ����� � ini ������.
TCHAR* name - ��� ini �����.
DWORD len - ������ ������ �� ������� ��������� path */
BOOL getIniFilePath (TCHAR* path, TCHAR* name, DWORD len);

/** ���������� ���� � ����� ��� ��������� ����� ������� ���� ������
HWND hwndOwner - ���������� ���� ��������� (��������� � MSDN\SHGetFolderPath).
TCHAR* appName - �������� ����������.
int clsid - ������������� ����������� ����� (��������� � MSDN\SHGetFolderPath), �� ��������� CSIDL_LOCAL_APPDATA.
����� ����� ����� �� ����������, �� ��� ����� �������. */
TCHAR* getAppDataPathStr (HWND hwndOwner, TCHAR* appName, int clsid = CSIDL_LOCAL_APPDATA);


/** ���������� ���� � ����� �������������, ����������� ��� ��
��������� � ����� ����� � ����������� ������� */
TCHAR* getIniFilePathStr (VOID);


/** ���������� �������� �� ������ ������ ������ � ������
TCHAR* str - ��������� �� ������.
TCHAR rchar - ������ */
TCHAR* lstrrchr (TCHAR* str, TCHAR rchar);


/** ����������� ������ ������ �� ������� ��������� �������
TCHAR* str - ������������� ������
TCHAR ch - �������� ������
���������� ��������� �� ����� �������� ������ � ������,
���� ����� ��� ������, ��� NULL ���� ������ ������� ��� � ������ */
TCHAR* lcutrchr (TCHAR* str, TCHAR rchar);


/** ��������� ������ ����� ����� � ����� ������ � ����,
���� ������ ������� ��� � ����� ������
TCHAR* path - ������ � ����
���������� TRUE ���� ������ path ���� ��������,
FALSE ���� �� ���� �������� */
BOOL addSlashToPath (TCHAR* path);


/** ������� ������ ����� ����� � ����� ������ � ����
TCHAR* path - ������ � ����
���������� TRUE ���� ������ path ���� ��������, FALSE ���� �� ���� �������� */
BOOL delSlashFromPath (TCHAR* path);


/** ������� ��������������� UNICODE-������ � Windows Debug output */
VOID wprintfd (wchar_t* format, ...);


/** ������� ��������������� ANSI-������ � Windows Debug output */
VOID printfd (char* format, ...);


/** ���������� ��������� ������� �� ������
HWND hDlg - ���������� �������
DWORD err - ��� ������
TCHAR* caption - ��������� ��������� */
VOID showErrorMessage (HWND hDlg, DWORD err, const TCHAR* caption);


/** ���������� ��������� c ��������������� �������
HWND hDlg - ���������� ���� ��������.
DWORD flags - ����� ������ � ������ (��. MSDN/Platform SDK/MessageBox).
TCHAR* format - ������ ������ ���������, �������� ��������� ���� � ���� ���������.
��������� ��� ������ � ���������� �� ��������� �������� '\n'.
���������� �������� ������������ MessageBox */
DWORD showFormatMessage (HWND hWnd, DWORD flags, TCHAR* format, ...);


/** ��������� ������ �� ��������
HWND hDlg - ���������� �������
UINT strId - ������������� ������
TCHAR* destStr - �������� �����
size_t strLen - ������ �������� ������ */
BOOL loadStringFromResource (HWND hDlg, UINT strId, TCHAR* destStr, SIZE_T strLen);


/** ���������� ��� ����� �� ��� �����������
HANDLE hFile - ���������� �����
TCHAR* pszFilename - ��������� �� �����, � ������� ����� �������� ��� �����
SIZE_T fileNameLen - ������ ������ pszFilename */
BOOL getFileNameFromHandle (HANDLE hFile, TCHAR* pszFilename, SIZE_T fileNameLen);


/** �������� ���� � ����������� ����. ���� ������ ���� � ����������,
�� ��������� ������ L'\\' � ����� ������, ���� ������ ���� � �����,
�� ������� ������ L'\\' � ����� ������, ���� ������ L'\\' ����
TCHAR* path - ������ � ���� */
DWORD normalizePath	(TCHAR* path);


/** �������� ���� �� ���� ������� ���� (�������� "cd ..")
TCHAR* path - ����.
���������� TRUE, ���� ���� ��� �������, FALSE ���� ���� �� ��� ������� */
BOOL cutPath (TCHAR* path);


/** ���������� ���-��� ������
TCHAR* str - ��������� �� ������ */
DWORD getHash (TCHAR* str);


/** ���������� ������ ��������� ����� �� ����� */
BOOL getDiskFreeSpace (TCHAR* pszDrive, ULARGE_INTEGER* i64FreeBytes);


#endif // _HELPER_H_
