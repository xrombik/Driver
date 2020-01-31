#include "helper.h"

BOOL saveListViewColumn (HWND hLv, TCHAR* name, int column)
{
	LVITEM lvi;
	TCHAR str [256];
	lvi.cchTextMax = SIZETC (str);
	lvi.pszText = str;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = column;
	DWORD items = SendMessage(hLv, LVM_GETITEMCOUNT, 0, 0);
	TCHAR* iniFile = getIniFilePathStr ();
	TCHAR sectionName [256];
	sprintf (sectionName, "%s column %d", name, lvi.iSubItem);
	DWORD rc;
	for (lvi.iItem = 0; lvi.iItem < items; ++lvi.iItem)
	{
		SendMessage (hLv, LVM_GETITEM, 0, (LPARAM)&lvi);
		TCHAR paramName [256];
		sprintf (paramName, "item%d", lvi.iItem);
		rc = WritePrivateProfileString (sectionName, paramName, lvi.pszText, iniFile);
	}
	items = SendMessage (hLv, LVM_GETCOLUMNWIDTH, column, 0);
	_itoa (items, lvi.pszText, 10);
	WritePrivateProfileString (sectionName, "Column width", lvi.pszText, iniFile);
	return TRUE;
}

BOOL loadListViewColumn (HWND hLv, TCHAR* name, int column)
{
	LVITEM lvi;
	TCHAR str [256];
	lvi.cchTextMax = SIZETC (str);
	lvi.pszText = str;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = column;
	TCHAR* iniFile = getIniFilePathStr ();
	TCHAR sectionName [256];
	sprintf (sectionName, "%s column %d", name, lvi.iSubItem);
	lvi.iItem = 0;
	int rc = 0;
	DWORD nChars = 0;
	do
	{
		TCHAR paramName [256];
		sprintf (paramName, "item%d",lvi.iItem);
		nChars = GetPrivateProfileString (sectionName, paramName, "", lvi.pszText, SIZETC (str), iniFile);
		if (nChars > 0)
		{
			if (lvi.iSubItem == 0)
			{
				rc = SendMessage (hLv, LVM_INSERTITEM, 0, (LPARAM)&lvi);
			}
			else
			{
				rc = SendMessage (hLv, LVM_SETITEM, 0, (LPARAM)&lvi);
			}
		}
		lvi.iItem ++;
	} while ((nChars > 0) && (rc != -1));

	GetPrivateProfileString (sectionName, "Column width", "80", lvi.pszText, SIZETC (str), iniFile);
	int colWidth = atoi (lvi.pszText);
	SendMessage (hLv, LVM_SETCOLUMNWIDTH, column, colWidth);
	return TRUE;
}


HANDLE openComPort (const char *name, HWND hDlg, unsigned int baud, unsigned int timeout, BOOL isSilent)
{
    HANDLE hCom = CreateFile (name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hCom == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError ();
        if (!isSilent)
			showErrorMessage (hDlg, err, name);
        return hCom;
    }
    DCB dcb;
    memset (&dcb, 0, sizeof (DCB));
    dcb.DCBlength = sizeof (DCB);
    BOOL rc = GetCommState (hCom, &dcb);
    if (rc == FALSE)
    {
        DWORD err = GetLastError ();
        if (!isSilent)
			showErrorMessage (hDlg, err, TEXT("Получение параметров COM-порта:"));
    }
    dcb.BaudRate = baud;//
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;

    rc = SetCommState (hCom, &dcb);
    if (rc == FALSE)
    {
        DWORD err = GetLastError ();
        if (!isSilent)
			showErrorMessage (hDlg, err, TEXT("Установка параметров COM-порта:"));
        CloseHandle (hCom);
        return hCom;
    }
    COMMTIMEOUTS cto;
    memset (&cto, 0, sizeof(COMMTIMEOUTS));
    rc = GetCommTimeouts (hCom, &cto);
    if (rc == FALSE)
    {
        DWORD err = GetLastError ();
        if (!isSilent)
			showErrorMessage (hDlg, err, TEXT("Получение таймаутов COM-порта:"));
        //CloseHandle (hCom);
        //return 0; // не опасная ошибка
    }
    cto.ReadIntervalTimeout = 4; // максимальное время между байтами в пакете
    cto.ReadTotalTimeoutConstant = timeout; // время для ожидания пакета, перед тем, как вернётся ReadFile, иными слова это таймаут для ReadFile

    rc = SetCommTimeouts (hCom, &cto);
    if (rc == FALSE)
    {
        DWORD err = GetLastError ();
        if (!isSilent)
			showErrorMessage (hDlg, err, TEXT("Установка таймаутов COM-порта:"));
        //return 0; // не опасная ошибка
    }
    return hCom;
}


DWORD readLine (HANDLE hFile, TCHAR* str, TCHAR end)
{
	TCHAR* begin = str;
	TCHAR symbol = '\0';
	DWORD rbytes = 0;
	do
	{
		ReadFile (hFile, &symbol, sizeof (symbol), &rbytes, NULL);
		if (rbytes == sizeof (symbol))
		{
			*str = symbol;
			if (*str == end)
			{
				*str = '\0';
			}
			str += sizeof (symbol);
		}
		else
		{
			break;
		}

	}
	while (symbol != end);
	return str - begin;
}


TCHAR* getAppDataPathStr (HWND hwndOwner, TCHAR* appName, int clsid)
{
	static TCHAR syspath [MAX_PATH];
	syspath [0] = TEXT('\0');
	SHGetFolderPath (hwndOwner, clsid, NULL, SHGFP_TYPE_CURRENT, syspath);
	BOOL rc = TRUE;
	lstrcat (syspath, TEXT("\\"));
	lstrcat (syspath, appName);
	if (FALSE == PathIsDirectory (syspath))
	{
		rc = CreateDirectory (syspath, NULL);
	}
	return rc ? syspath : NULL;
}


void texOutBorder (HDC hdc, TCHAR* str, int x, int y)
{
	int l = lstrlen (str);
	COLORREF crBk = GetBkColor (hdc);
	COLORREF crTxt = GetTextColor (hdc);
	int bm = SetBkMode (hdc, TRANSPARENT);
	SetTextColor (hdc, crBk);
	TextOut (hdc, x - 1, y - 1, str, l);
	TextOut (hdc, x - 1, y - 0, str, l);
	TextOut (hdc, x - 0, y - 1, str, l);
	TextOut (hdc, x + 1, y + 1, str, l);
	TextOut (hdc, x + 1, y - 0, str, l);
	TextOut (hdc, x - 0, y + 1, str, l);
	SetTextColor (hdc, crTxt);
	TextOut (hdc, x, y, str, l);
	SetBkMode (hdc, bm);
}


BOOL strstart (TCHAR* str, TCHAR* subStr)
{
	for (int i = 0; subStr[i]; ++ i)
	{
		if (str[i] != subStr[i]) return FALSE;
	}
	return TRUE;
}


DWORD addPath (TCHAR* curPath, TCHAR* addPath)
{
	// Если доб. путь текущий - ничего не делаем
	if (lstrcmp_ (addPath, symCurDir) == 0) return HAP_CURDIR;

	// Если доб. путь уровень выше - обрезаем
	if (lstrcmp_ (addPath, symUpDir) == 0)
	{
		cutPath (curPath);
		return HAP_DIR_UP;
	}

	// Если это есть имя, то - добавляем его ...
	// ... есть ли в конце символ "\\"? ...
	TCHAR* last1 = lstrrchr (curPath, symSlashChar);
	TCHAR* last2 = lstrlen_ (curPath) + curPath - 1;
	// ... нет - дописываем
	if (last1 != last2) lstrcat_ (curPath, symSlashStr);
	// ... здесь в любом случае есть, дописываем добавляемый путь
	lstrcat_ (curPath, addPath);

	// что значит переход на уровень ниже
	return HAP_DIR_DOWN;
}




INT CALLBACK nodeWndProc (WMNODE* wmNode, HWND hWnd, UINT msg,  WPARAM wParam, LPARAM lParam)
{
	for (WMNODE* curWmNode = wmNode; curWmNode->next; curWmNode = curWmNode->next)
	{
		if (msg == curWmNode->message)
		{
			if (curWmNode->messageProc)
			{
				return curWmNode->messageProc (hWnd, wParam, lParam);
			}
		}
	}
	return 0;
}



TCHAR* uintToBinStr  (unsigned int val, int bitsCount, TCHAR* binChars)
{
/*#ifdef UNICODE
	static wchar_t str [33];
	static wchar_t bVals [] = {L'0', L'1'};
	str [32] = L'\0';
#else
	static char str [33];
	static char bVals [] = {'0', '1'};
	str [32] = '\0';
#endif*/
	static TCHAR str [33];
	static TCHAR bVals [] = {TEXT('0'), TEXT('1')};
	str [32] = TEXT ('\0');
	if (binChars)
	{
		bVals [0] = binChars [0];
		bVals [1] = binChars [1];
	}
	for (int i = 0; i < bitsCount; ++ i)
	{
		str [bitsCount - i - 1] = bVals [(val & (1 << i)) >> i];
	}
	return str;
}



int multiByteToWideChar (wchar_t* wcStr, char* mbStr, UINT codePage)
{
	int len = lstrlenA (mbStr) + sizeof ('\0');
	return MultiByteToWideChar (codePage, MB_PRECOMPOSED, mbStr, -1, wcStr, len);
}


int wideCharToMultiByte (wchar_t* wcStr, char* mbStr, UINT codePage)
{
	int len = lstrlenW (wcStr) + sizeof ('\0');
	return WideCharToMultiByte (codePage, 0, wcStr, -1, mbStr, len, NULL, NULL);
}




BOOL getSafeFileNameForWrite (char* path)
{
	return TRUE;
}


DWORD showFormatMessage (HWND hWnd, DWORD flags, TCHAR* format, ...)
{
	TCHAR* buf = (TCHAR*) LocalAlloc (LMEM_FIXED | LMEM_ZEROINIT, 1024 * sizeof (TCHAR));
	DWORD rc = MB_OK;
	if (buf != NULL)
	{
		DWORD localSize = (DWORD) LocalSize ((HLOCAL)buf);
		va_list args;
		va_start (args, format);
		#ifdef UNICODE
		_vsnwprintf (buf, localSize, format, args);
		#else
		_vsnprintf (buf, localSize, format, args);
		#endif
		va_end (args);
		TCHAR* msgCaption = buf;
		int count = lstrlen (buf);
		int i = 0;
		for (; i < count; ++ i)
		{
			if (buf[i] == TEXT('\n'))
			{
				buf[i] = TEXT('\0');
				break;
			}
		}
		rc = MessageBox (hWnd, &buf [i + 1], msgCaption, flags);
		LocalFree (buf);
	}
	return rc;
}



BOOL initListViewColumns (HWND hLv, DWORD columns, TCHAR* headers [], BOOL checks)
{
	LVCOLUMN lvc = {0};
	RECT rect;
	GetWindowRect (hLv, &rect);
	lvc.cx = (rect.right - rect.left) / columns;
	// The mask specifies that the format, width, text, and subitem members of the structure are valid.
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_CENTER;
    for (DWORD iCol = 0; iCol < columns; ++ iCol)
	{
		lvc.pszText = headers[iCol];
		SendMessage (hLv, LVM_INSERTCOLUMN, iCol, (LPARAM)&lvc);
	 }

	SendMessage (hLv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES, (LPARAM)LVS_EX_GRIDLINES);
	SendMessage (hLv, LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)LVS_EX_FULLROWSELECT, (LPARAM)LVS_EX_FULLROWSELECT);
	if (checks)
	{
		SendMessage (hLv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_CHECKBOXES, (LPARAM)LVS_EX_CHECKBOXES);
	}
	return TRUE;
}


BOOL loadPresetsNamesFromIni (TCHAR* sectionName, TCHAR* fileName, HWND hComboBox)
{
	// Загрузка названий профилей
	TCHAR path [65535];
	TCHAR str [1024];
	TCHAR buf [65535];
	DWORD strLen = 0;
	DWORD strBegin;
	DWORD strEnd;
	if (!getIniFilePath (path, fileName, SIZETC(path)))
	{
		printfd ((char*)"Файл инициализации не найден\n");
		return FALSE;
	}
	DWORD chars = GetPrivateProfileSection (sectionName, buf, sizeof(buf), path);
	if (chars == 0)
	{
		printfd ((char*)"Названия профилей не загружены\n");
		return FALSE;
	}
	SendMessage (hComboBox, CB_RESETCONTENT, 0, 0);
	BOOL flag = TRUE;
	for (DWORD i = 0; i < chars; ++ i)
	{
		if (buf[i] == L'=')
		{
			strEnd = i;
			strBegin = i;
			while (buf[strBegin] != TEXT('\0'))
			{
				if (strBegin == 0)
				{
					break;
				}
				-- strBegin;
			}
			if (strBegin > 0)
			{
				++ strBegin;
			}
			size_t bytesCount = (strEnd - strBegin) << 1;
			memcpy (str, &buf[strBegin], bytesCount);
			str[strEnd - strBegin] =  TEXT('\0');
			SendMessage (hComboBox, CB_ADDSTRING, 0, (LPARAM)str);
		}
		strLen += flag;
	}
	return TRUE;
}


BOOL loadPresetsNamesFromIniEx (TCHAR* sectionName, TCHAR* path, HWND hComboBox)
{
	// Загрузка названий профилей
	TCHAR str [1024];
	TCHAR buf [65535];
	DWORD strLen = 0;
	DWORD strBegin;
	DWORD strEnd;
	DWORD chars = GetPrivateProfileSection (sectionName, buf, sizeof(buf), path);
	if (chars == 0)
	{
		printfd ((char*)"Названия профилей не загружены\n");
		return FALSE;
	}
	SendMessage (hComboBox, CB_RESETCONTENT, 0, 0);
	BOOL flag = TRUE;
	for (DWORD i = 0; i < chars; ++ i)
	{
		if (buf[i] == L'=')
		{
			strEnd = i;
			strBegin = i;
			while (buf[strBegin] != TEXT('\0'))
			{
				if (strBegin == 0)
				{
					break;
				}
				-- strBegin;
			}
			if (strBegin > 0)
			{
				++ strBegin;
			}
			size_t bytesCount = (strEnd - strBegin) << 1;
			memcpy (str, &buf[strBegin], bytesCount);
			str[strEnd - strBegin] =  TEXT('\0');
			SendMessage (hComboBox, CB_ADDSTRING, 0, (LPARAM)str);
		}
		strLen += flag;
	}
	return TRUE;
}



BOOL getIniFilePath (TCHAR* path, TCHAR* name, DWORD len)
{
	if (GetModuleFileName (NULL, path, len))
	{
		lstrcpy (PathFindFileName (path), name);
		return TRUE;
	}
	return FALSE;
}


TCHAR* getIniFilePathStr (VOID)
{
	static TCHAR path [65535];
	path [0] = TEXT('\0');
	if (GetModuleFileName (NULL, path, SIZETC(path)))
	{
		lstrcpy (lstrrchr (path, TEXT('.')), TEXT(".ini"));
	}
	return path;
}




DWORD normalizePath (TCHAR* path)
{

	if ((path + lstrlen(path) - 1) == lstrrchr (path, TEXT('\\')))
	{
		*lstrrchr (path, TEXT('\\')) = TEXT('\0');
	}

	DWORD fileAttr = GetFileAttributes (path);
	if (fileAttr == INVALID_FILE_ATTRIBUTES)
	{
		TCHAR* tmpChar = lstrrchr (path, TEXT('\\'));
		if (tmpChar != NULL) *tmpChar = TEXT('\0');
		return 0;
	}

	if (fileAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		lstrcat (path, TEXT("\\"));
	}
	return fileAttr;
}


VOID showErrorMessage (HWND hDlg, DWORD err, const TCHAR* caption)
{
	HLOCAL hlocal = NULL;
	FormatMessage  (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					(LPTSTR) &hlocal, 0, NULL);
	if (hlocal != NULL)
	{
		MessageBox (hDlg, (TCHAR*)hlocal, caption, MB_OK | MB_ICONERROR);
		LocalFree (hlocal);
	}
	else
	{
		MessageBox (hDlg, TEXT("Не известная ошибка"), caption, MB_OK | MB_ICONERROR);
	}
}


BOOL printdErrorStr (DWORD errorCode)
{
	HLOCAL hlocal = NULL;
	FormatMessage  (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					(LPTSTR) &hlocal, 0, NULL);
	if (hlocal != NULL)
	{
		#ifdef UNICODE
		wprintfd (L"%s\n", (wchar_t*)hlocal);
		#else
		printfd ((char*)"%s\n", (char*) hlocal);
		#endif
		LocalFree (hlocal);
		return TRUE;
	}
	else
	{
		#ifdef UNICODE
		HMODULE hLibrary = LoadLibraryW (L"netmsg.dll");
		#else
		HMODULE hLibrary = LoadLibraryA ("netmsg.dll");
		#endif
		if (hLibrary)
		{
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
				(LPTSTR) &hlocal, 0, NULL);
			if (hlocal != NULL)
			{
				#ifdef UNICODE
				wprintfd (L"%s\n", (wchar_t*)hlocal);
				#else
				printfd ((char*)"%s\n", (char*) hlocal);
				#endif
				LocalFree (hlocal);
				return TRUE;
			}
			FreeLibrary (hLibrary);
		}
	}
	#ifdef UNICODE
	wprintfd (L"Нет описания для кода ошибки %u.\n", errorCode);
	#else
	printfd ((char*)"Нет описания для кода ошибки %u.\n", errorCode);
	#endif
	return FALSE;
}


BOOL printErrorStr (DWORD errorCode)
{
	HLOCAL hlocal = NULL;
	FormatMessage  (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					(LPTSTR) &hlocal, 0, NULL);
	if (hlocal != NULL)
	{
		#ifdef UNICODE
		wprintf (L"%u: %s", errorCode, (wchar_t*)hlocal);
		#else
		printf ("%u: %s", (unsigned int)errorCode,(char*) hlocal);
		#endif
		LocalFree (hlocal);
		return TRUE;
	}
	else
	{
		#ifdef UNICODE
		wprintf (L"Нет описания для кода ошибки %u\n", errorCode);
		#else
		printf ((char*)"Нет описания для кода ошибки %u\n", (unsigned int)errorCode);
		#endif
	}
	return FALSE;
}




VOID wprintfd (wchar_t* format, ...)
{
	wchar_t* buf = (wchar_t*) LocalAlloc (LMEM_FIXED | LMEM_ZEROINIT, 2048);
	if (buf == NULL) return;
	va_list args;
	va_start (args, format);
	_vsnwprintf (buf, LocalSize ((HLOCAL)buf), format, args);
	va_end (args);
	OutputDebugStringW (buf);
	LocalFree (buf);
}

VOID printfd (char* format, ...)
{
	char* buf = (char*) LocalAlloc (LMEM_FIXED | LMEM_ZEROINIT, 2048);
	if (buf == NULL) return;
	va_list args;
	va_start (args, format);
	_vsnprintf (buf, LocalSize ((HLOCAL)buf), format, args);
	va_end (args);
	OutputDebugStringA (buf);
	LocalFree (buf);
}



TCHAR* lstrrchr (TCHAR* str, TCHAR rchar)
{
	TCHAR* end = NULL;
	if (str)
	{
		end = str + lstrlen (str);
		while (*end != rchar)
		{
			if (end == str)
			{
				end = NULL;
				break;
			}
			-- end;
		}
	}
	return end;
}


TCHAR* lcutrchr (TCHAR* str, TCHAR ch)
{
	TCHAR* cutCh = lstrrchr (str, ch);
	if (cutCh != NULL) *cutCh = TEXT('\0');
	return cutCh;
}



BOOL loadStringFromResource (HWND hDlg, UINT strId, TCHAR* destStr, SIZE_T strLen)
{
	LONG hInst = GetWindowLong(hDlg, GWL_HINSTANCE);
	BOOL rc = FALSE;
	if (LoadString((HINSTANCE)hInst, strId, destStr, strLen) == 0)
	{
		#ifdef UNICODE
		wprintfd (TEXT("loadStringFromResource(): Невозможно загрузить ресурс id %u"), strId);
		#else
		printfd (TEXT((char*)"loadStringFromResource(): Невозможно загрузить ресурс id %u"), strId);
		#endif
		rc = FALSE;
	}
	return rc;
}


BOOL getFileNameFromHandle (HANDLE hFile, TCHAR* pszFilename, SIZE_T fileNameLen)
{
	pszFilename [0] = TEXT('\0');

	// Создать объект отображения файла
	HANDLE hFileMap = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 1, NULL);
	if (hFileMap == NULL) return FALSE;

	// Создать отображение файла для получения имени файла
	void* pMem = MapViewOfFile (hFileMap, FILE_MAP_READ, 0, 0, 1);
	if (pMem == NULL)
	{
		CloseHandle (hFileMap);
		return FALSE;
	}

	if (GetMappedFileName (GetCurrentProcess(), pMem, pszFilename, fileNameLen) == 0) return FALSE;
	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = TEXT('\0');
	if (GetLogicalDriveStrings(MAX_PATH, szTemp) == 0)
	{
		UnmapViewOfFile (pMem);
		CloseHandle (hFileMap);
		return FALSE;
	}
	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do
	{
		// Copy the drive letter to the template string
		*szDrive = *p;
		// Look up each device name
		if (QueryDosDevice(szDrive, szName, sizeof(szName)))
		{
			UINT uNameLen = lstrlen (szName);
			if (uNameLen < MAX_PATH)
			{
				#ifdef UNICODE
				bFound = _wcsnicmp (pszFilename, szName, uNameLen) == 0;
				#else
				bFound = _strnicmp (pszFilename, szName, uNameLen) == 0;
				#endif
				if (bFound)
				{
					#ifdef UNICODE
					wsprintf (szTemp, TEXT("%s%s"), szDrive, pszFilename + uNameLen);
					#else
					sprintf (szTemp, TEXT("%s%s"), szDrive, pszFilename + uNameLen);
					#endif
					lstrcpy (pszFilename, szTemp);
				}
			}
		}

		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	UnmapViewOfFile (pMem);
	CloseHandle (hFileMap);
	return TRUE;
}



BOOL addSlashToPath (TCHAR* path)
{
	TCHAR* tc = lstrrchr (path, TEXT('*'));
	if (tc) *tc = TEXT('\0');
	tc = lstrrchr (path, TEXT('\\'));
	TCHAR* lc = path + lstrlen (path) - 1;
	if (lc != tc)
	{
		lstrcat (path, TEXT("\\"));
		return TRUE;
	}
	return FALSE;
}


BOOL delSlashFromPath (TCHAR* path)
{
	TCHAR* tc = lstrrchr (path, TEXT('\\'));
	if (tc)
	{
		*tc = TEXT('\0');
		return TRUE;
	}
	return FALSE;
}


BOOL cutPath (TCHAR* path)
{
	TCHAR* tchar = lstrrchr (path, TEXT('*'));
	if (tchar) *tchar = TEXT('\0');

	tchar = lstrrchr (path, TEXT('\\'));
	TCHAR* last = path + lstrlen (path) - 1;
	if (last == tchar) *tchar = TEXT('\0');

	tchar = lstrrchr (path, TEXT('\\'));
	if (tchar) *tchar = TEXT('\0');

	return tchar != NULL;
}



//DWORD  getHash (TCHAR *str)
//{
//	DWORD hash = 5381;
//	DWORD c;
//	while (c = (*str)++)
//	{
//		hash = ((hash << 5) + hash) + c;
//	}
//	return hash;
//}


BOOL getDiskFreeSpace (TCHAR* pszDrive, ULARGE_INTEGER* i64FreeBytes)
{
	ULARGE_INTEGER i64FreeBytesToCaller;
	ULARGE_INTEGER i64TotalBytes;
	return GetDiskFreeSpaceEx
		(pszDrive, &i64FreeBytesToCaller, &i64TotalBytes, i64FreeBytes);
}

