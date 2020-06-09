//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_DEPENDENCY_H
#define WORKSHOP1_DEPENDENCY_H

#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

class Dependency
{
 public:
	static void EndLine()
	{
		cout << endl;
	}

	static void ClearScreen(string HeaderText)
	{
		cout << "\033[2J\033[1;1H";
		cout << "\e[1m" << "\e[36m" << HeaderText << "\e[39;49m" << "\e[0m" << endl;
		EndLine();
	};
	static void SleepCommand(int sec)
	{
		this_thread::sleep_for(chrono::milliseconds(sec));
	};

	static string bold_on()
	{
		return "\e[1m";
	}

	static string bold_off()
	{
		return "\e[0m";
	}

	//Disable Echo
	static void SetStdinEcho(bool enable = true)
	{
#ifdef WIN32
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
	GetConsoleMode(hStdin, &mode);

	if( !enable )
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode );

#else
		struct termios tty;
		tcgetattr(STDIN_FILENO, &tty);
		if (!enable)
			tty.c_lflag &= ~ECHO;
		else
			tty.c_lflag |= ECHO;

		(void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
	}
};

#endif //WORKSHOP1_DEPENDENCY_H
