using System;
using System.Threading;

class Program
{
	static int baseAdr; // базовый адрес
	static int readSize; // 1 - читаем по байтам, 2 - читаем по словам 
	static int holeNum; // количество просматриваемых адресов
	static void Main(string[] args)
	{
		baseAdr = Convert.ToInt32(args[0]); // базовый адрес
		readSize = Convert.ToInt32(args[1]); // 1 - читаем по байтам, 2 - читаем по словам 
		holeNum = Convert.ToInt32(args[2]); // количество просматриваемых адресов
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

