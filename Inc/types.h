#define ushort unsigned short


#define FORM_FLAGS_BYTE    1
#define FORM_FLAGS_WORD    2


// ������������ ��� ������ � ���������.
// ����� ��������� ������ ���������� 
// ��������� ������ ����������, ����� WriteFile
// ��� ReadFile. ����� ��������� ������� �������.
typedef struct _FORM
{
	ushort Address;	// ����� �����. ������� ��������.
	ushort Data;	// ������ �����. ������� �������� ��� ������,
					// �������� ��� ������
	int Flags;		// ����� ������� ������ ������
} FORM, *PFORM;

// ����������� ���������� ��������.
// �������������� ���� DosDeviceName,
// ������������ ��� �������� ������������� ������
#if defined(_NTDDK_)
	typedef struct _DEVICE_EXTENSION
	{
		PDEVICE_OBJECT pDevice;						
		UNICODE_STRING DosDeviceName;
	} DEVICE_EXTENSION, *PDEVICE_EXTENSION;
#endif
