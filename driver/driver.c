#include "driver.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	NTSTATUS ns;
	PDEVICE_EXTENSION pDevExt;
	UNICODE_STRING ntDeviceName;
	
	RtlInitUnicodeString(&ntDeviceName, DEVICE_NAME);
	
	ns = IoCreateDevice(pDriverObject,
						sizeof (DEVICE_EXTENSION),
                        &ntDeviceName,
                        FILE_DEVICE_UNKNOWN,
                        METHOD_NEITHER, 
                        FALSE,
    					&pDriverObject->DeviceObject);
	
	if (ns != STATUS_SUCCESS)
	{
		return ns;
	}
	
	pDevExt = pDriverObject->DeviceObject->DeviceExtension;
	pDevExt->pDevice = pDriverObject->DeviceObject;

	RtlInitUnicodeString(&pDevExt->DosDeviceName, DOSDEVICE_NAME);
	ns = IoCreateSymbolicLink(&pDevExt->DosDeviceName, &ntDeviceName);
	
	if (ns != STATUS_SUCCESS)    
    {                           
		IoDeleteDevice(pDriverObject->DeviceObject);
        return ns;
    }
	
	pDriverObject->MajorFunction [IRP_MJ_CREATE] = Open;
	pDriverObject->MajorFunction [IRP_MJ_CLOSE ] = Close;
	pDriverObject->MajorFunction [IRP_MJ_READ  ] = Read;
	pDriverObject->MajorFunction [IRP_MJ_WRITE ] = Write;
	pDriverObject->DriverUnload = DriverUnload;
	
	return ns;
}

VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	PDEVICE_EXTENSION pDevExt = pDriverObject->DeviceObject->DeviceExtension;
	if (IoDeleteSymbolicLink(&pDevExt->DosDeviceName) != STATUS_SUCCESS)
	{
		return;
	}
	IoDeleteDevice(pDriverObject->DeviceObject);
	return;
}

NTSTATUS Open(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS Close(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS Read(IN PDEVICE_OBJECT pDO, IN PIRP pIrp)
{
	ULONG bytesWriten;
	NTSTATUS status;
	PFORM pForm;
	PIO_STACK_LOCATION pIrpStack;
	
	bytesWriten = 0;
	status = STATUS_SUCCESS;
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	
	if (pIrpStack->Parameters.Read.Length == sizeof(FORM))
	{
		pForm = pIrp->UserBuffer;
		bytesWriten = sizeof(FORM);
		if (pForm->Flags & FORM_FLAGS_WORD)
		{
			ReadPort (pForm->Address, &pForm->Data);
		}
		else if (pForm->Flags & FORM_FLAGS_BYTE)
		{
			ReadPortByte (pForm->Address, &pForm->Data);
		}
		else
		{
			status = STATUS_INVALID_PARAMETER_3;
			bytesWriten = 0;
		}
	}
	else
	{
		status = STATUS_INVALID_BUFFER_SIZE;
	}
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = bytesWriten;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS Write(IN PDEVICE_OBJECT pDO, IN PIRP pIrp)
{
	ULONG bytesWriten;
	NTSTATUS status;
	PFORM pForm;
	PIO_STACK_LOCATION pIrpStack;
	
	bytesWriten = 0;
	status = STATUS_SUCCESS;
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	
	if (pIrpStack->Parameters.Write.Length == sizeof(FORM))
	{
		pForm = pIrp->UserBuffer;
		bytesWriten = sizeof(FORM);
		if (pForm->Flags & FORM_FLAGS_WORD)
		{
			WritePort (pForm->Address, &pForm->Data);
		}
		else if (pForm->Flags & FORM_FLAGS_BYTE)
		{
			WritePortByte (pForm->Address, &pForm->Data);
		}
		else
		{
			status = STATUS_INVALID_PARAMETER_3;
			bytesWriten = 0;
		}
	}
	else
	{
		status = STATUS_INVALID_BUFFER_SIZE;
	}
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = bytesWriten;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}

void ReadPort(ushort adres, ushort* pData)
{
	ushort data;
	__asm
	{
		mov dx, adres;
		in  ax, dx;
		mov data, ax;
	};
	*pData = data;
}

void WritePort(ushort adres, ushort* pData)
{
	ushort data = *pData;
	__asm
	{
		mov ax, data;
		mov dx, adres;
		out dx, ax;
	};
}


void ReadPortByte(ushort adres, ushort* pData)
{
	char data;
	ushort outB;
	__asm
	{
		mov dx, adres;
		in  al, dx;
		mov data, al;
	};
	outB = (ushort) data;
	*pData = outB;
}

void WritePortByte(ushort adres, ushort* pData)
{
	char data = (char)((*pData) & 0xff);
	__asm
	{
		mov al, data;
		mov dx, adres;
		out dx, al;
	};
}