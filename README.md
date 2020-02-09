# Driver
Simple legacy device driver for Windows XP, Windows 7 32bit
This driver grant access to 16 bit addressed native x86 IO-ports.
Checked with:
- Windows XP 32bit
- Windows 7 32bit

Repo contains precompiled *.dll and *.sys files intended to use
in 32 bit OS Windows 7 and XP. Also it contains CPP wraper that
uses this driver and some test benches in C#. Also source code of
*.dll and *.sys is provided here. And if you need to run this
driver in Windows 8 64 bit and later, you have to rebuild *.dll
and *.sys files yourself. This package provide sources and
project files to build in Visual Studio 6, and Visual Studio 2005.

Check Visual Studio project settings, they are may be incomplete.
