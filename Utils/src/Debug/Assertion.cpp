#include "utils_stdafx.h"

#include <sstream>
#include <Debug/Assertion.h>

#include "Debug/Log.h"

using namespace Debug;

Assertion::Assertion(const char* p_expression, const char* p_file, const int p_line, const std::string& p_message):
	m_expression(p_expression), m_file(p_file), m_line(p_line), m_message(p_message)
{
	std::ostringstream os;

	if (!m_message.empty())
	{
		os << m_message << ": ";
	}

	const std::string expressionString = m_expression;

	if (expressionString == "false" || expressionString == "0" || expressionString == "FALSE")
	{
		os << "Unreachable code assertion";
	}
	else
	{
		os << "Assertion '" << m_expression << "'";
	}
	m_report = os.str();

	LogError();
}

Assertion::~Assertion()
{
}

void Assertion::LogError() const
{
	LOG(Log::Fatal(), m_report);
}

const char* Assertion::what() const throw()
{
	return m_report.c_str();
}

const char* Assertion::Expression() const throw()
{
	return m_expression;
}

const char* Assertion::File() const throw()
{
	return m_file;
}

int Assertion::Line() const throw()
{
	return m_line;
}

const char* Assertion::Message() const throw()
{
	return m_message.c_str();
}