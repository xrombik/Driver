using System;
using System.Runtime.InteropServices;

public class IOPort
{
	// Номера функций определены эмперически
	[DllImport("IOPortAPI.dll", EntryPoint = "#4")]
	public static extern unsafe void WriteWord(int Address, int Data);
	[DllImport("IOPortAPI.dll", EntryPoint = "#2")]
	public static extern unsafe int ReadWord(int Address);
	[DllImport("IOPortAPI.dll", EntryPoint = "#3")]
	public static extern unsafe void WriteByte(int Address, int Data);
	[DllImport("IOPortAPI.dll", EntryPoint = "#1")]
	public static extern unsafe int ReadByte(int Address);
}

