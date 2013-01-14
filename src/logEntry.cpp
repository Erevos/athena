#include "logEntry.hpp"

namespace athena
{
	namespace io
	{
		// The constructor of the class.
		LogEntryA::LogEntryA(const LogEntryType& type, const std::string& message, const std::string& timestamp) :
			m_timestamp(timestamp),
			m_message(message),
			m_type(type)
		{}

		// The destructor of the class.
		LogEntryA::~LogEntryA()
		{}


		// The constructor of the class.
		LogEntryW::LogEntryW(const LogEntryType& type, const std::wstring& message, const std::wstring& timestamp) :
			m_timestamp(timestamp),
			m_message(message),
			m_type(type)
		{}

		// The destructor of the class.
		LogEntryW::~LogEntryW()
		{}

	} /* io */
} /* athena */
