#define ushort unsigned short


#define FORM_FLAGS_BYTE    1
#define FORM_FLAGS_WORD    2


// Используется для обмена с драйвером.
// Такую структуру должны передовать 
// программы уровня приложений, через WriteFile
// или ReadFile. Такую структуру ожидает драйвер.
typedef struct _FORM
{
	ushort Address;	// Адрес порта. Входной параметр.
	ushort Data;	// Данные порта. Входной параметр при записи,
					// выходной при чтении
	int Flags;		// Флаги формата записи чтения
} FORM, *PFORM;

// Стандартное расширение драйвера.
// Дополнительное поле DosDeviceName,
// используется для хранения символической ссылки
#if defined(_NTDDK_)
	typedef struct _DEVICE_EXTENSION
	{
		PDEVICE_OBJECT pDevice;						
		UNICODE_STRING DosDeviceName;
	} DEVICE_EXTENSION, *PDEVICE_EXTENSION;
#endif
