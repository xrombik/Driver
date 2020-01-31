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


/** Возвращает количество элементов в стековом массиве arg */
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
	/** Адрес процедуры обработки сообщения */
	BOOL (*messageProc) (HWND hWnd, WPARAM wParam, LPARAM lParam);
	/** Сообщение окну */
	UINT message;
	/** Указатель на следующую ноду */
	_WMNODE* next;
} WMNODE;

/** Сохраняет содержимое одной колонки ListView в ini-файле программы.
HWND hLv - дескриптор ListView
TCHAR* name - название колонки, int column - номер колонки слво направо, начиная
от нуля */
BOOL saveListViewColumn (HWND hLv, TCHAR* name, int column);

/** Загружает содержимое одной колонки ListView из ini-файла программы.
HWND hLv - дескриптор ListView
TCHAR* name - название колонки, int column - номер колонки слво направо, начиная
от нуля */
BOOL loadListViewColumn (HWND hLv, TCHAR* name, int column);


/** Создаёт дескриптор COM-порта
unsigned int timeout - время ожидания на вызове ReadFile */
HANDLE openComPort (const char *name, HWND hDlg, unsigned int baud, unsigned int
	timeout, BOOL isSilent = FALSE);

/** Читает строку из файла.
HANDLE hFile - дескриптор файла.
TCHAR* str - выходная строка.
TCHAR end - символ конца строки, по умолчанию '\n' */
DWORD readLine (HANDLE hFile, TCHAR* str, TCHAR end = TEXT('\n'));

void texOutBorder (HDC hdc, char* str, int x, int y);


/** Проверяет, начинается ли строка с заданной подстроки.
TCHAR* str - проверяемая строка.
TCHAR* subStr - искомая подстрока.
Длинна проверяемой строки должна быть больше или
равна длинне искомой подстроки */
BOOL strstart (TCHAR* str, TCHAR* subStr);


/** Добавляет путь учитывая ".."
TCHAR* curPath - текущий путь.
TCHAR* addPath - добавляемый путь.
Возвращает HAP_DIR_DAWN, HAP_DIR_UP или HAP_CURDIR */
DWORD addPath (TCHAR* curPath, TCHAR* addPath);

/* Константы возвращаемые процедурой addPath */
#define HAP_DIR_DOWN	1	/*< Переход на один уровень вниз	*/
#define HAP_DIR_UP		2	/*< Переход на один уровень вверх	*/
#define HAP_CURDIR		5	/*< Текущая папка, нет действий		*/



INT CALLBACK nodeWndProc (WMNODE *wmNode, HWND hWnd, UINT msg,  WPARAM wParam, LPARAM lParam);


/** Возвращает строку в двоичном представлении */
TCHAR* uintToBinStr (unsigned int val, int bitsCount = 32, TCHAR* binChars = NULL);


int wideCharToMultiByte (wchar_t* wcStr, char* mbStr, UINT codePage = CP_ACP);
int multiByteToWideChar (wchar_t* wcStr, char* mbStr, UINT codePage = CP_ACP);


/** Возвращает имя файла, изменяя номер в названии, если
файл с таким именем уже существует и его длинна не равна нулю.
Номер в названии должен быть заключён между квадратными скобками "[" и "]".
char* path - путь к файлу, вход и выход */
BOOL getSafeFileNameForWrite (char* path);


/** Выводит описание ошибки в Windows debug output
DWORD errorCode - код ошибки возвращаемый GetLastError */
BOOL printdErrorStr (DWORD errorCode);


/** Выводит описание ошибки в стандартный вывод
DWORD errorCode - код ошибки возвращаемый GetLastError */
BOOL printErrorStr (DWORD errorCode);


/** Инициализирует List View
HWND hLv - дескриптор List View.
DWORD columns - количество колонок.
TCHAR* headers [] - заголовки колонок.
BOOL checks - чекбоксы: TRUE - есть, FALSE - нет */
BOOL initListViewColumns (HWND hLv, DWORD columns, TCHAR* headers [], BOOL checks = FALSE);


/** Загружает названия пресетов из ini файла в ComboBox
TCHAR* sectionName - название секции.
HWND hComboBox - дескриптор ComboBox */
BOOL loadPresetsNamesFromIni (TCHAR* sectionName, TCHAR* fileName, HWND hComboBox);

/** Загружает названия пресетов из ini файла в ComboBox
TCHAR* sectionName - название секции.
HWND hComboBox - дескриптор ComboBox
TCHAR *fileName - полный путь к ini файлу */
BOOL loadPresetsNamesFromIniEx (TCHAR* sectionName, TCHAR* fileName, HWND hComboBox);


/** Возвращает путь к файлу инициализации, предполагая что он
находится в одной папке с исполняемым модулем
TCHAR* path - путь к папке с ini файлом.
TCHAR* name - имя ini файла.
DWORD len - длинна буфера на который указывает path */
BOOL getIniFilePath (TCHAR* path, TCHAR* name, DWORD len);

/** Возвращает путь к папке где программа может хранить свои данные
HWND hwndOwner - Дескриптор окна владельца (подробнее в MSDN\SHGetFolderPath).
TCHAR* appName - название приложения.
int clsid - идентификатор специальной папки (подробнее в MSDN\SHGetFolderPath), по умолчанию CSIDL_LOCAL_APPDATA.
Еслии такой папки не существует, то она будет создана. */
TCHAR* getAppDataPathStr (HWND hwndOwner, TCHAR* appName, int clsid = CSIDL_LOCAL_APPDATA);


/** Возвращает путь к файлу инициализации, предполагая что он
находится в одной папке с исполняемым модулем */
TCHAR* getIniFilePathStr (VOID);


/** Возвращает указтель на первый справо символ в строке
TCHAR* str - указатель на строку.
TCHAR rchar - символ */
TCHAR* lstrrchr (TCHAR* str, TCHAR rchar);


/** Укорачивает строку справо до первого заданного символа
TCHAR* str - укорачиваемая строка
TCHAR ch - заданный символ
Возвращает указатель на певый заданный символ в строке,
если такой был найден, или NULL если такого символа нет в строке */
TCHAR* lcutrchr (TCHAR* str, TCHAR rchar);


/** Добавляет символ косой черты к концу строки с путём,
если такого символа нет в конце строки
TCHAR* path - строка с путём
Возвращает TRUE если строка path была изменена,
FALSE если не была изменена */
BOOL addSlashToPath (TCHAR* path);


/** Удаляет символ косой черты в конце строки с путём
TCHAR* path - строка с путём
Возвращает TRUE если строка path была изменена, FALSE если не была изменина */
BOOL delSlashFromPath (TCHAR* path);


/** Выводит форматированную UNICODE-строку в Windows Debug output */
VOID wprintfd (wchar_t* format, ...);


/** Выводит форматированную ANSI-строку в Windows Debug output */
VOID printfd (char* format, ...);


/** Показывает сообщение системы об ошибке
HWND hDlg - дескриптор диалога
DWORD err - код ошибки
TCHAR* caption - заголовок сообщения */
VOID showErrorMessage (HWND hDlg, DWORD err, const TCHAR* caption);


/** Показывает сообщение c форматированной строкой
HWND hDlg - Дескриптор окна родителя.
DWORD flags - Флаги кнопок и иконок (см. MSDN/Platform SDK/MessageBox).
TCHAR* format - Формат строки сообщения, содержит заголовок окна и само сообщение.
Заголовок идёт первым и отделяется от сообщения символом '\n'.
Возвращает значение возвращаемое MessageBox */
DWORD showFormatMessage (HWND hWnd, DWORD flags, TCHAR* format, ...);


/** Загружает строку из ресурсов
HWND hDlg - дескриптор диалога
UINT strId - идентификатор строки
TCHAR* destStr - выходной буфер
size_t strLen - длинна входного буфера */
BOOL loadStringFromResource (HWND hDlg, UINT strId, TCHAR* destStr, SIZE_T strLen);


/** Возвращает имя файла по его дескриптору
HANDLE hFile - дескриптор файла
TCHAR* pszFilename - указатель на буфер, в который будет помещено имя файла
SIZE_T fileNameLen - длинна буфера pszFilename */
BOOL getFileNameFromHandle (HANDLE hFile, TCHAR* pszFilename, SIZE_T fileNameLen);


/** Приводит путь к нормальному виду. Если строка путь к директории,
то добавляет символ L'\\' в конец строки, Если строка путь к файлу,
то убирает символ L'\\' в конце строки, если символ L'\\' есть
TCHAR* path - Строка с путём */
DWORD normalizePath	(TCHAR* path);


/** Обрезает путь на один уровень выше (комманда "cd ..")
TCHAR* path - путь.
Возвращает TRUE, если путь был обрезан, FALSE если путь не был обрезан */
BOOL cutPath (TCHAR* path);


/** Возвращает хэш-код строки
TCHAR* str - указатель на строку */
DWORD getHash (TCHAR* str);


/** Возвращает размер свобоного места на диске */
BOOL getDiskFreeSpace (TCHAR* pszDrive, ULARGE_INTEGER* i64FreeBytes);


#endif // _HELPER_H_
