using System;

class Program
{
	static void Main(string[] args)
	{
		if (args[0] == "b")
		{
			if (args.Length == 2)
			{
				int adr = Convert.ToInt32(args[1]);
				int dat = IOPort.ReadByte(adr);
				Console.WriteLine("address=" + adr + " read=" + dat);
			}
			if (args.Length == 3)
			{
				Console.WriteLine("address=" + args[1] + " write=" + args[2]);
				IOPort.WriteByte(Convert.ToInt32(args[1]), Convert.ToInt32(args[2]));
			}
		}
		if (args[0] == "w")
		{
			if (args.Length == 2)
			{
				int adr = Convert.ToInt32(args[1]);
				int dat = IOPort.ReadWord(adr);
				Console.WriteLine("address=" + adr + " read=" + dat);
			}
			if (args.Length == 3)
			{
				Console.WriteLine("address=" + args[1] + " write=" + args[2]);
				IOPort.WriteWord(Convert.ToInt32(args[1]), Convert.ToInt32(args[2]));
			}
		}
	}
}

