#pragma once

#include "utils_stdafx.h"
#include "UtilsAPI.h"

#pragma warning(push)
#pragma warning(disable:4251 4275)

namespace Debug
{
	class UTILS_API Assertion : public std::exception
	{
	private:
		const char* m_expression;
		const char* m_file;
		int m_line;
		std::string m_message;
		std::string m_report;

	public:
		Assertion(const char* p_expression, const char* p_file, int p_line, const std::string& p_message);
		~Assertion() throw();

		void LogError() const;

		const char* what() const throw() override;
		const char* Expression() const throw();
		const char* File() const throw();
		int Line() const throw();
		const char* Message() const throw();
	};

	#define ASSERT(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw Debug::Assertion(#EXPRESSION, __FILE__, __LINE__, MESSAGE); }
}

#pragma warning(pop)