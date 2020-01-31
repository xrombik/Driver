#include "gaport.h"

#include <shlwapi.h> // включим здесь, что бы не было конфликта с borland

GAPort::GAPort (TCHAR* szDriverPath, TCHAR* szDllPath)
{
	TCHAR path [65536];
	TCHAR* procName;
	static const char* cfpe = "Невозможно найти процедуру";
	
	if (szDllPath == NULL)
	{
		szDllPath = path;
		lstrcpy (PathFindFileName (szDllPath), TEXT("IOPortAPI.dll"));
	}
	
	this->hApi = LoadLibrary (szDllPath);
	if (this->hApi == NULL)
	{
		printfd ("Невозожно загрузить %s\n", szDllPath);
		/* Настройка заглушек */
		this->readIOP = this->readStub;
		this->writeIOP = this->writeStub;
		this->readByteIOP = this->readByteStub;
		this->writeByteIOP = this->writeByteStub;
		return;
	}
	

	procName = "ReadWord";
	readIOP = (READIOP) GetProcAddress (this->hApi, (LPCSTR)procName);
	DWORD err = GetLastError();
	if (readIOP == NULL)
	{
		printfd ("%s %s в %s\n", cfpe, procName, szDllPath);
		this->readIOP = this->readStub;
	}
	
	procName = "WriteWord";
	writeIOP = (WRITEIOP) GetProcAddress (this->hApi, procName);	
	if (writeIOP == NULL)
	{
		printfd ("%s %s в %s\n", cfpe, procName, szDllPath);
		this->writeIOP = this->writeStub;
	}
	
	procName = "WriteByte";
	writeByteIOP = (WRITEIOP) GetProcAddress (this->hApi, procName);	
	if (writeByteIOP == NULL)
	{
		printfd ("%s %s в %s\n", cfpe, procName, szDllPath);
		this->writeByteIOP = this->writeByteStub;
	}
	
	procName = "ReadByte";
	readByteIOP = (READIOP) GetProcAddress (this->hApi, procName);	
	if (readByteIOP == NULL)
	{
		printfd ("%s %s в %s\n", cfpe, procName, szDllPath);
		this->readByteIOP = this->readByteStub;
	}

}


void GAPort :: writeStub (int adr, int data)
{
	printfd ("Заглушка для GAPort::writeIOP (%d, %d)\n", adr, data);
}


int GAPort :: readStub (int adr)
{
	printfd ("Заглушка для GAPort::readIOP (%d)\n", adr);
	return 0xffff;
}


void GAPort :: writeByteStub (int adr, int data)
{
	printfd ("Заглушка для GAPort::writeByteIOP (%d, %d)\n", adr, data);
}


int GAPort :: readByteStub (int adr)
{
	printfd ("Заглушка для GAPort::readByteIOP (%d)\n", adr);
	return 0xffff;
}


GAPort::~ GAPort (void)
{
	this->free();
}

void GAPort::free (void)
{
	if (this->hApi)	FreeLibrary (this->hApi);
}