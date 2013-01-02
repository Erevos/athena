#ifndef ATHENA_IO_LOGMANAGER_INL
#define ATHENA_IO_LOGMANAGER_INL

#ifndef ATHENA_IO_LOGMANAGER_HPP
	#error "logManager.hpp must be included before logManager.inl"
#endif /* ATHENA_IO_LOGMANAGER_HPP */

#include <ctime>
#include <sstream>
#include "../utility/stringUtilities.hpp"



namespace athena
{

	namespace io
	{

		/*
			ASCII Log Manager definitions.
		*/


		// Function responsible of logging an error with the given message.
		inline void LogManager::log_error( const std::string& message )
		{
			log_entry(Error,message);
		}

		// Function responsible of logging an error with converted contents of the given message.
		inline void LogManager::log_error( const std::wstring& message )
		{
			log_entry(Error,utility::wide_string_to_string(message));
		}

		// Function responsible of logging an error with the contents of the given exception.
		inline void LogManager::log_error( const std::exception& exception )
		{
			log_entry(Error,exception.what());
		}

		// Function responsible of logging a warning with the given message.
		inline void LogManager::log_warning( const std::string& message )
		{
			log_entry(Warning,message);
		}

		// Function responsible of logging a warning with the converted contents of the given message.
		inline void LogManager::log_warning( const std::wstring& message )
		{
			log_entry(Warning,utility::wide_string_to_string(message));
		}

		// Function responsible of logging a warning with the contents of the given exception.
		inline void LogManager::log_warning( const std::exception& exception )
		{
			log_entry(Warning,exception.what());
		}

		// Function responsible of logging a message with the given message.
		inline void LogManager::log_message( const std::string& message )
		{
			log_entry(Message,message);
		}

		// Function responsible of logging a message with the converted contents of the given message.
		inline void LogManager::log_message( const std::wstring& message )
		{
			log_entry(Message,utility::wide_string_to_string(message));
		}

		// Function responsible of logging a message with the contents of the given exception.
		inline void LogManager::log_message( const std::exception& exception )
		{
			log_entry(Message,exception.what());
		}

	} /* io */

} /* athena */



#endif /* ATHENA_IO_LOGMANAGER_INL */
