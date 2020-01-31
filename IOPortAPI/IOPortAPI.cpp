// Библиотека для работы с драйвером портов ввода-вывода
#include "IOPortAPI.h"

HANDLE dh;
FORM form;
DWORD bytesNumber;
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  call_reason, LPVOID lpReserved)
{
	TCHAR szDriverPath [65535];
	BOOL bRc = TRUE;
	SC_HANDLE scDataBase;
	static SC_HANDLE schService;
	
	if (call_reason == DLL_PROCESS_ATTACH)
	{
		scDataBase = OpenSCManager (NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (scDataBase == NULL)
		{
			DWORD err = GetLastError();
			printfd ("Ошибка %u при подключении к менеджеру сервисов\n", err);
			return FALSE;
		}
			
		GetModuleFileName (NULL, szDriverPath, sizeof (szDriverPath));
		lstrcpy (PathFindFileName (szDriverPath), TEXT("driver.sys"));
				
		schService = OpenService (scDataBase, TEXT("PROBA SERVICE (NAME OF SERVICE)"), SERVICE_ALL_ACCESS);
		if (schService == NULL)
		{
			if (ERROR_SERVICE_DOES_NOT_EXIST == GetLastError ())
			{
				schService = CreateService ( 
					scDataBase,											// SCManager database 
					TEXT("PROBA SERVICE (NAME OF SERVICE)"),			// name of service 
					TEXT("PROBA SERVICE (service name to display)"),	// service name to display 
					SERVICE_ALL_ACCESS,									// desired access 
					SERVICE_KERNEL_DRIVER,								// service type 
					SERVICE_DEMAND_START,								// start type 
					SERVICE_ERROR_NORMAL,								// error control type 
					szDriverPath,										// path to service's binary 
					NULL,												// no load ordering group 
					NULL,												// no tag identifier 
					NULL,												// no dependencies 
					NULL,												// LocalSystem account 
					NULL);												// no password 
			}
		}
		
		if (schService == NULL) 
		{
			DWORD err = GetLastError();
			CloseServiceHandle (scDataBase);
			printfd ("Ошибка %u при загрузке драйвера %s\n", err, szDriverPath);
			return FALSE;
		}
		
		bRc = StartService (schService, 0, NULL);
		if (bRc == FALSE)
		{
			DWORD err = GetLastError();
			if (err) printfd ("Ошибка %u при запуске драйвера %s\n", err, szDriverPath);
			switch (err)
			{
				case NOERROR:
					break;
				case SERVICE_RUNNING:
					printfd ("Драйвер уже запущен %s\n", err, szDriverPath);
					break;
				case 1058:
					printfd ("Нет особых действий\n");
					break;
				default:
					CloseServiceHandle (scDataBase);
					DeleteService (schService);
					CloseServiceHandle (schService);
					return FALSE;
			}
		}
				
		CloseServiceHandle (scDataBase);

		dh = CreateFile (IOPORT_FILE_NAME, GENERIC_WRITE|GENERIC_READ,
		0, NULL, OPEN_EXISTING, 0, 0);
		if (dh == INVALID_HANDLE_VALUE)
		{
			DWORD err = GetLastError();
			printfd("IOPortAPI.dll: Ошибка %u при создании дескриптора устройства\n", err);
			return FALSE;
		}
	}

	if (call_reason == DLL_PROCESS_DETACH)
	{
		SERVICE_STATUS ss;
		CloseHandle (dh);
		if (schService != NULL)
		{
			ControlService (schService, SERVICE_CONTROL_STOP, &ss);
			DeleteService (schService);
			CloseServiceHandle (schService);
		}
	}
    return TRUE;
}

// Записывает слово (2 байта) в заданный порт ввода-вывода
// Аргументы:
//	Address - адрес порта (0x0000-0xffff),
//	Data - данные для записи (0x0000-0xffff)
extern "C" __declspec(dllexport) void WriteWord(int Address, int Data)
{
	form.Address = Address;
	form.Data = Data;
	form.Flags = FORM_FLAGS_WORD;
	WriteFile(dh, &form, sizeof(FORM), &bytesNumber, NULL);
}
// Читает порт ввода-вывода.
// Аргументы:
//	Address - адрес порта (0x0000-0xffff),
// Возвращаемое значение
//  Слово (2 байта, 0x0000-0xffff), прочитанное из порта ввода-вывода
extern "C" __declspec(dllexport) int ReadWord(int Address)
{
	form.Address = Address;
	form.Flags = FORM_FLAGS_WORD;
	ReadFile(dh, &form, sizeof(FORM), &bytesNumber, NULL);
	return form.Data;
}

// Записывает байт в заданный порт ввода-вывода
// Аргументы:
//	Address - адрес порта (0x0000-0xffff),
//	Data - данные для записи (0x00-0xff)
extern "C" __declspec(dllexport) void WriteByte (int Address, int Data)
{
	form.Data = Data;
	form.Address = Address;
	form.Flags = FORM_FLAGS_BYTE;
	WriteFile(dh, &form, sizeof(FORM), &bytesNumber, NULL);
}

// Читает порт ввода-вывода.
// Аргументы:
//	Address - адрес порта (0x0000-0xffff),
// Возвращаемое значение
//  Байт (0x00-0xff), прочитанный из порта ввода-вывода
extern "C" __declspec(dllexport) int ReadByte(int Address)
{
	form.Address = Address;
	form.Flags = FORM_FLAGS_BYTE;
	ReadFile(dh, &form, sizeof(FORM), &bytesNumber, NULL);
	return form.Data;
}
