// Класс-оболочка для работы с IOPortAPI.dll
// Обеспечивает доступ к портам ввода-вывода через простые вызовы
// Горшков А. С.


#include <windows.h>
#include <tchar.h>

#include <helper.h>

#pragma comment(lib, "shlwapi.lib")

typedef int (*READIOP)(int adr);
typedef void (*WRITEIOP)(int adr, int val);

class GAPort 
{
	SC_HANDLE schService;
	HMODULE hApi;
	void free (void);
	static void writeStub (int adr, int data);
	static int readStub (int adr);
	static void writeByteStub (int adr, int data);
	static int readByteStub (int adr);
	
	public:
	
	GAPort (TCHAR* szDriverPath = NULL, TCHAR* szDllPath = NULL);
	~ GAPort (void);
	READIOP readIOP;
	WRITEIOP writeIOP;
	READIOP readByteIOP;
	WRITEIOP writeByteIOP;
};

