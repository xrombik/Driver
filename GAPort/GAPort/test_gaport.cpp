// ������ ������������� 

#include <stdio.h>  // printf () � �.�.
#include "gaport.h" // ���������� ���� ��� ������� � ������

int main(int argc, _TCHAR* argv[])
{
	GAPort gp;      // ������ � ������
	unsigned int addr = 888;
	if (argc == 2)
	addr = atoi((const char*)argv[1]);
	for (int i = 0; i < 65536; ++ i)
	{
		gp.writeIOP (addr, i);
		printf ("Addr: %04x Write word: %04x Read word: %04x\n", addr & 0xffff, i, gp.readIOP (addr) & 0xffff);
	}
	
	for (int i = 0; i < 256; ++ i)
	{
		gp.writeByteIOP (addr, i);
		printf ("Addr: %04x Write byte: %02x Read byte: %04x\n", addr & 0xffff, i & 0xff, gp.readByteIOP (addr));
	}
	return 0;
}

