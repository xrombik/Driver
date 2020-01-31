#include <windows.h>
#include "..\Inc\types.h"
#include "..\Inc\constants.h"
#include "..\..\include\helper.h"

#pragma comment (lib, "Advapi32.lib")

extern "C" __declspec(dllexport) int ReadByte(int Address);
extern "C" __declspec(dllexport) void WriteByte (int Address, int Data);
extern "C" __declspec(dllexport) int ReadWord(int Address);
extern "C" __declspec(dllexport) void WriteWord(int Address, int Data);
