#include "utils_stdafx.h"

#include "Debug/FileLogger.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace Debug;

FileLogger::FileLogger() : m_logDirectory("log"), m_filename("log.txt"), m_counter (0)
{
	const fs::path fullPath(PathToSessionLogFile());
	if (!fs::exists(m_logDirectory))
		fs::create_directory(m_logDirectory);
		
	m_out = std::ofstream(fullPath);
}

FileLogger::~FileLogger()
{
	if (m_counter < 1)
		m_out << "No Error neither warning found.";
	m_out.close();
}

FileLogger::FileLogger(FileLogger&& p_fileLogger) noexcept : m_filename(std::move(p_fileLogger.m_filename)), m_out(std::move(p_fileLogger.m_out))
{}

void FileLogger::LogLine(const std::string& p_message, const char* p_function, const char* p_file, const int p_line,
                         const LogLevel p_level)
{	
	m_out <<  "\n::  " <<logLevelToStr(p_level) << "   :: " 
		  << p_function << "() in " << p_file << "(" 
		  << p_line     << ")\n"    << "::  MESSAGE ::" << p_message << '\n';
	m_counter++;
}

void FileLogger::Flush()
{
	m_out.flush();
}

std::string FileLogger::TimeStampPerso()
{
	const std::chrono::time_point<std::chrono::system_clock> temp = std::chrono::system_clock::now();
	std::time_t now = std::chrono::system_clock::to_time_t(temp);
	
	struct tm structTemp;
	char cstr[32]{};

	localtime_s(&structTemp, &now);
	return std::strftime(cstr, sizeof(cstr), "%m_%d_%H-%M", &structTemp) ? cstr : "" ;
}

std::string  FileLogger::PathToSessionLogFile()
{
	m_filename = TimeStampPerso() + '_' + m_filename;
	return m_logDirectory.string() + "/" +  m_filename;
}