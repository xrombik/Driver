#define UNICODE

#include <ntddk.h>
#include "..\Inc\types.h"
#include "..\Inc\constants.h"


void ReadPort(ushort portAddress, ushort* pData);
void WritePort(ushort portAddress, ushort* pData);
void ReadPortByte(ushort adres, ushort* pData);
void WritePortByte(ushort adres, ushort* pData);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT, IN PUNICODE_STRING);
VOID DriverUnload(IN PDRIVER_OBJECT);
NTSTATUS Open(IN PDEVICE_OBJECT, IN PIRP);
NTSTATUS Close(IN PDEVICE_OBJECT, IN PIRP);
NTSTATUS Read(IN PDEVICE_OBJECT, IN PIRP);
NTSTATUS Write(IN PDEVICE_OBJECT, IN PIRP);

