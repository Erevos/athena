#ifndef ATHENA_IO_LOGMANAGER_INL
#define ATHENA_IO_LOGMANAGER_INL

#ifndef ATHENA_IO_LOGMANAGER_HPP
	#error "logManager.hpp must be included before logManager.inl"
#endif /* ATHENA_IO_LOGMANAGER_HPP */

#include <ctime>
#include <sstream>



namespace athena
{

	namespace io
	{

		/*
			ASCII Log Manager definitions.
		*/


		// Function responsible of logging an error with the given message.
		inline void LogManagerA::logError( const std::string& message )
		{
			_log_entry(Error,message);
		};

		// Function responsible of logging an error with the contents of the given exception.
		inline void LogManagerA::logError( const std::exception& exception )
		{
			_log_entry(Error,exception.what());
		};

		// Function responsible of logging a warning with the given message.
		inline void LogManagerA::logWarning( const std::string& message )
		{
			_log_entry(Warning,message);
		};

		// Function responsible of logging a warning with the contents of the given exception.
		inline void LogManagerA::logWarning( const std::exception& exception )
		{
			_log_entry(Warning,exception.what());
		};

		// Function responsible of logging a message with the given message.
		inline void LogManagerA::logMessage( const std::string& message )
		{
			_log_entry(Message,message);
		};

		// Function responsible of logging a message with the contents of the given exception.
		inline void LogManagerA::logMessage( const std::exception& exception )
		{
			_log_entry(Message,exception.what());
		};



		/*
			UTF Log Manager definitions.
		*/

			
		// Function responsible of logging an error with the given message.
		inline void LogManagerW::logError( const std::wstring& message )
		{
			_log_entry(Error,message);
		};

		// Function responsible of logging an error with the contents of the given exception.
		inline void LogManagerW::logError( const std::exception& exception )
		{
			exception;//TODO;_log_entry(Error,exception.what());
		};

		// Function responsible of logging a warning with the given message.
		inline void LogManagerW::logWarning( const std::wstring& message )
		{
			_log_entry(Warning,message);
		};

		// Function responsible of logging a warning with the contents of the given exception.
		inline void LogManagerW::logWarning( const std::exception& exception )
		{
			exception;//TODO;_log_entry(Warning,exception.what());
		};

		// Function responsible of logging a message with the given message.
		inline void LogManagerW::logMessage( const std::wstring& message )
		{
			_log_entry(Message,message);
		};

		// Function responsible of logging a message with the contents of the given exception.
		inline void LogManagerW::logMessage( const std::exception& exception )
		{
			exception;//TODO;_log_entry(Message,exception.what());
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_LOGMANAGER_INL */