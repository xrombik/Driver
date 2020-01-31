// Константы используемые драйвером для создания символической
// ссылки на порты ввода-выода
#define DOSDEVICE_NAME TEXT("\\DosDevices\\IOPort")
#define DEVICE_NAME TEXT("\\Device\\IOPort")

// Название файла, используемое для открытия доступа к
// драйверу, через CreateFile (уровень приложения)
#define IOPORT_FILE_NAME TEXT("\\\\.\\IOPort")