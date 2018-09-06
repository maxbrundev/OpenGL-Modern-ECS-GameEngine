#include "utils_stdafx.h"

#include <Debug/ConsoleLogger.h>

using namespace Debug;

ConsoleLogger::ConsoleLogger()
{}

ConsoleLogger::~ConsoleLogger()
{}

void ConsoleLogger::LogLine(const std::string& p_message, const char* p_function, const char* p_file, const int p_line, const LogLevel p_level)
{
	const HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	const HANDLE hstderr = GetStdHandle(STD_ERROR_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO out_csbi;
	CONSOLE_SCREEN_BUFFER_INFO err_csbi;

	GetConsoleScreenBufferInfo(hstdout, &out_csbi);
	GetConsoleScreenBufferInfo(hstderr, &err_csbi);
	
	std::string file = p_file;
	file = file.substr(file.find_last_of("\\") + 1);

	switch (p_level)
	{
	case LogLevel::INFO:
		SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << ":: INFO   ::  ";
		SetConsoleTextAttribute(hstdout, out_csbi.wAttributes);
		std::cout << p_message << '\n';
		break;

	case LogLevel::DEBUG_:
		SetConsoleTextAttribute(hstdout, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << ":: DEBUG  ::  ";
		SetConsoleTextAttribute(hstdout, out_csbi.wAttributes);
		std::cout << p_function << "() in " << file << "(" << p_line << ") : " << p_message << '\n';
		break;

	case LogLevel::WARNING:
		SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << ":: WARNING::  ";
		SetConsoleTextAttribute(hstdout, out_csbi.wAttributes);
		std::cout << p_function << "() in " << file << "(" << p_line << ") : " << p_message << '\n';
		break;

	case LogLevel::ERROR_:
		SetConsoleTextAttribute(hstderr, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cerr << ":: ERROR  ::  ";
		SetConsoleTextAttribute(hstderr, err_csbi.wAttributes);
		std::cout << p_function << "()in " << file << "(" << p_line << ") : " << p_message << '\n';
		break;

	case LogLevel::FATAL:
		SetConsoleTextAttribute(hstderr, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cerr << ":: FATAL  ::  ";
		SetConsoleTextAttribute(hstderr, err_csbi.wAttributes);
		std::cout << p_function << "()in " << file << "(" << p_line << ") : " << p_message << '\n';
	default:;
	}
}