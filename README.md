# InquisitorCollinCodez-oracle-addition
This is a tool to help with Parsec Support.

This is still in development.

How to compile:

	1) Open a developer commmand line from Microsoft Visual Studio in the directory with the code for Inqusitor
	2) run nmake
	3) Bada Bing, Bada Boom! Debug Away!

Current Features:

	-Grabs and prints Log files from both the Appdata path and the Programdata path
	-Grabs and prints config files from both the Appdata path and the Programdata path
	-Grab the System's CPU. GPU(s), Drivers, and what monitors are attached to each GPU, using Parsec's Oracle

Future Plans:

	-Save the output to an Inquisitor.txt file in the same directory as Inquisitor.exe (using Oracle's log_and_print)
	-Remove IP's from log
	-Run a tracerout to 1.1.1.1 for determining the number of NAT's (and potentially just telling how many there are)
	-Grab what network connections the device has currently active 
	-Possibly run a speedtest
