using System;
using System.Threading;

class Program
{
	static int baseAdr; // ������� �����
	static int readSize; // 1 - ������ �� ������, 2 - ������ �� ������ 
	static int holeNum; // ���������� ��������������� �������
	static void Main(string[] args)
	{
		baseAdr = Convert.ToInt32(args[0]); // ������� �����
		readSize = Convert.ToInt32(args[1]); // 1 - ������ �� ������, 2 - ������ �� ������ 
		holeNum = Convert.ToInt32(args[2]); // ���������� ��������������� �������
		ThreadStart ts = null;
		if (readSize == 1)
			ts = new ThreadStart(readByteMethod);
		if (readSize == 2)
			ts = new ThreadStart(readWordMethod);
		Thread readThread = new Thread(ts);
		readThread.Start();
		Console.ReadKey();
	}
	
	static void readByteMethod()
	{
		while (true)
		{
			for (int i = baseAdr; i < baseAdr + holeNum; i += readSize)
			{
				Console.Write(IOPort.ReadByte(i) + " ");
			}
			Console.WriteLine();
			System.Threading.Thread.Sleep(100);
		}
	}

	static void readWordMethod()
	{
		while (true)
		{
			for (int i = baseAdr; i < baseAdr + holeNum; i += readSize)
			{
				Console.Write(IOPort.ReadWord(i) + "\t");
			}
			Console.WriteLine();
			System.Threading.Thread.Sleep(100);
		}
	}
}

