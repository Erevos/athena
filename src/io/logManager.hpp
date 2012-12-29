#ifndef ATHENA_IO_LOGMANAGER_HPP
#define ATHENA_IO_LOGMANAGER_HPP

#include "../athenaDefinitions.hpp"
#include "logEntry.hpp"
#include "../utility/readersWriterLock.hpp"
#include <deque>
#include <exception>
#include <fstream>
#include <cstdarg>


namespace athena
{

	namespace io
	{

		/* 
			The possible open modes for the files used by the log manager.
		*/
		enum LogFileOpenMode
		{
			Append = 0 , 
			Truncate
		};


		/*
			A singleton class responsible of handling a ASCII log for the application.
		*/
		class LogManagerA
		{
			private:

				// The single instance of the class.
				static LogManagerA* _instance;
				// The loc that is used to handle concurrency issues regarding the instance of the class.
				static utility::ReadersWriterLock _instance_lock;
				// The maximum possible size for the buffer that is used in the functions of the class.
				static const unsigned int _MAX_BUFFER_SIZE;


				// A list of the entries that represent the log.
				std::deque<LogEntryA> _log;
				// A lock that is used to handle concurrency issues for the class.
				mutable utility::ReadersWriterLock _lock;
				// The file stream that is used for the auto-dump operations.
				std::ofstream _auto_dump_filestream;
				// The name of the file that is used for auto-dump operations.
				std::string _auto_dump_filename;
				// The tag that is used to identify error type entries.
				std::string _error_tag;
				// The tag that is used to identify warning type entries.
				std::string _warning_tag;
				// The tag that is used to identify message type entries.
				std::string _message_tag;
				// The separator that is used to separate the timestamp from the type of the entry.
				std::string _type_separator;
				// The separator that is used to separate the type of the entry from the message of the entry.
				std::string _message_separator;
				// The separator that is used to separate the date from the time in the timestamp.
				std::string _timestamp_separator;
				// The file openmode that is used for the auto-dump file.
				std::ofstream::openmode _auto_dump_file_open_mode;
				// The stream that is used for echo operations.
				std::ostream* _echo_stream;
				// The maximum log size;
				unsigned int _max_log_size;
				// The threshold for the auto purge functionality.
				unsigned int _auto_purge_threshold;
				// The threshold for the auto dump functionality.
				unsigned int _auto_dump_threshold;
				// The current count for the auto dump functionality.
				unsigned int _auto_dump_count;
				// The offset that is used when dumping to the auto-dump file.
				unsigned int _auto_dump_offset;
				// Whether the auto purge functionality is enabled.
				bool _auto_purge;
				// Whether the auto dump functionality is enabled.
				bool _auto_dump;
				// Whether echoing is enabled.
				bool _echo;


				// The constructor of the class.
				LogManagerA();
				// The destructor of the class.
				~LogManagerA();


				// Function responsible of generating a string with the current timestamp.
				std::string _generate_timestamp();
				// Function responsible of updating the timestamps of the log with the new separator.
				void _update_timestamps( const std::string& new_separator );
				// Function posting an entry to the given stream.
				void _post_entry( std::ostream* stream , const LogEntryA& entry );
				// Function dumping the contents from the log to the given stream starting from offset.
				bool _dump_log( std::ostream* stream  , const unsigned int offset );
				// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
				void _manage_log_size();
				// Function responsible of handling the auto-dump filestream.
				void _manage_dump_filestream();
				// Function responsible of parsing the parameters that were given to the log functions.
				std::string _parse_parameters( const char* input , va_list parameters );
				// Function responsible of adding a new entry of the given type and with the given message to the log.
				void _log_entry( const LogEntryType& type , const std::string& message );


			public:

				// Function responsible of initialising the instance of the class. Returns true on success.
				ATHENA_DLL static bool initialise();
				// Function responsible of deinitialising the instance of the class.
				ATHENA_DLL static void deinitialise();
				// Function returning a pointer to the single instance of the class.
				ATHENA_DLL static LogManagerA* get();


				// Function responsible of setting the maximum size of the log.
				ATHENA_DLL void max_log_size( const unsigned int size );
				// Function responsible of enabling or disabling the auto purge functionality.
				ATHENA_DLL void auto_purge( const bool value );
				// Function responsible of setting the threshold of the auto-purge functionality.
				ATHENA_DLL void auto_purge_threshold( const unsigned int threshold );
				// Function responsible of setting the name of the file that is used for the auto dump filestream.
				ATHENA_DLL void auto_dump_filename( const std::string& filename );
				// Function responsible of setting the open mode for the auto dump filestream.
				ATHENA_DLL void auto_dump_file_open_mode( const LogFileOpenMode mode );
				// Function responsible of setting the threshold for the auto dump functionality.
				ATHENA_DLL void auto_dump_threshold( const unsigned int threshold );
				// Function responsible of enabling or disabling the auto dump functionality.
				ATHENA_DLL void auto_dump( const bool value );
				// Function responsible of setting the stream that is used for the echo functionality.
				ATHENA_DLL void echo_stream( std::ostream* stream );
				// Function responsible of enabling or disabling the echoing the contents of the log.
				ATHENA_DLL void echo( const bool value );
				// Function responsible of setting the error tag.
				ATHENA_DLL void error_tag( const std::string& tag );
				// Function responsible of setting the warning tag.
				ATHENA_DLL void warning_tag( const std::string& tag );
				// Function responsible of setting the message tag.
				ATHENA_DLL void message_tag( const std::string& tag );
				// Function responsible of setting the type separator.
				ATHENA_DLL void type_separator( const std::string& separator );
				// Function responsible of setting the message separator.
				ATHENA_DLL void message_separator( const std::string& separator );
				// Function responsible of setting the timestamp separator.
				ATHENA_DLL void timestamp_separator( const std::string& separator );
				// Function responsible of logging an error with the given message.
				ATHENA_DLL void logError( const std::string& message );
				// Function responsible of logging an error with the contents of the given exception.
				ATHENA_DLL void logError( const std::exception& exception );
				// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logError( const char* format , ... );
				// Function responsible of logging a warning with the given message.
				ATHENA_DLL void logWarning( const std::string& message );
				// Function responsible of logging a warning with the contents of the given exception.
				ATHENA_DLL void logWarning( const std::exception& exception );
				// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logWarning( const char* message , ... );
				// Function responsible of logging a message with the given message.
				ATHENA_DLL void logMessage( const std::string& message );
				// Function responsible of logging a message with the contents of the given exception.
				ATHENA_DLL void logMessage( const std::exception& exception );
				// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logMessage( const char* message , ... );
				// Function responsible of dumping the log the file with the given filename.
				ATHENA_DLL void dump_log( const std::string& filename , const LogFileOpenMode mode = Append );
				// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
				ATHENA_DLL void purge_log();


				// Function returning the maximum size of the log.
				ATHENA_DLL unsigned int max_log_size() const;
				// Function returning whether the auto purge functionality is enabled.
				ATHENA_DLL bool auto_purge() const;
				// Function returning the threshold of the auto purge functionality.
				ATHENA_DLL unsigned int auto_purge_threshold() const;
				// Function returning the name of the file that is used in the auto dump functionality.
				ATHENA_DLL std::string auto_dump_filename() const;
				// Function returning the open mode of the auto dump file.
				ATHENA_DLL LogFileOpenMode auto_dump_file_open_mode() const;
				// Function returning the threshold of the auto dump functionality.
				ATHENA_DLL unsigned int auto_dump_threshold() const;
				// Function returning whether the auto dump functionality is enabled.
				ATHENA_DLL bool auto_dump() const;
				// Function returning a pointer to the stream that is used for echo operations.
				ATHENA_DLL std::ostream* echo_stream() const;
				// Function returning whether echoing is enabled.
				ATHENA_DLL bool echo() const;
				// Function returning the error tag.
				ATHENA_DLL std::string error_tag() const;
				// Function returning the warning tag.
				ATHENA_DLL std::string warning_tag() const;
				// Function returning the message tag.
				ATHENA_DLL std::string message_tag() const;
				// Function returning the type separator.
				ATHENA_DLL std::string type_separator() const;
				// Function returning the message separator.
				ATHENA_DLL std::string message_separator() const;
				// Function returning the timestamp separator.
				ATHENA_DLL std::string timestamp_separator() const;
				// Function returning the size of the log.
				ATHENA_DLL unsigned int log_size() const;
				// Function returning the entry at the given index. If the index is greater than the size of the log the last entry is returned.
				ATHENA_DLL LogEntryA entry( const unsigned int index = INFINITE ) const;
				// Function returning the desired number of entries from the given start.
				ATHENA_DLL std::deque<LogEntryA> entries( const unsigned int number, const unsigned int start = 0 ) const;
		};


		/*
			A singleton class responsible of handling a UTF log for the application.
		*/
		class LogManagerW
		{
			private:

				// The single instance of the class.
				static LogManagerW* _instance;
				// The loc that is used to handle concurrency issues regarding the instance of the class.
				static utility::ReadersWriterLock _instance_lock;
				// The maximum possible size for the buffer that is used in the functions of the class.
				static const unsigned int _MAX_BUFFER_SIZE;


				// A list of the entries that represent the log.
				std::deque<LogEntryW> _log;
				// A lock that is used to handle concurrency issues for the class.
				mutable utility::ReadersWriterLock _lock;
				// The file stream that is used for the auto-dump operations.
				std::wofstream _auto_dump_filestream;
				// The name of the file that is used for auto-dump operations.
				std::wstring _auto_dump_filename;
				// The tag that is used to identify error type entries.
				std::wstring _error_tag;
				// The tag that is used to identify warning type entries.
				std::wstring _warning_tag;
				// The tag that is used to identify message type entries.
				std::wstring _message_tag;
				// The separator that is used to separate the timestamp from the type of the entry.
				std::wstring _type_separator;
				// The separator that is used to separate the type of the entry from the message of the entry.
				std::wstring _message_separator;
				// The separator that is used to separate the date from the time in the timestamp.
				std::wstring _timestamp_separator;
				// The file openmode that is used for the auto-dump file.
				std::wofstream::openmode _auto_dump_file_open_mode;
				// The stream that is used for echo operations.
				std::wostream* _echo_stream;
				// The maximum log size;
				unsigned int _max_log_size;
				// The threshold for the auto purge functionality.
				unsigned int _auto_purge_threshold;
				// The threshold for the auto dump functionality.
				unsigned int _auto_dump_threshold;
				// The current count for the auto dump functionality.
				unsigned int _auto_dump_count;
				// The offset that is used when dumping to the auto-dump file.
				unsigned int _auto_dump_offset;
				// Whether the auto purge functionality is enabled.
				bool _auto_purge;
				// Whether the auto dump functionality is enabled.
				bool _auto_dump;
				// Whether echoing is enabled.
				bool _echo;


				// The constructor of the class.
				LogManagerW();
				// The destructor of the class.
				~LogManagerW();


				// Function responsible of generating a string with the current timestamp.
				std::wstring _generate_timestamp();
				// Function responsible of updating the timestamps of the log with the new separator.
				void _update_timestamps( const std::wstring& new_separator );
				// Function posting an entry to the given stream.
				void _post_entry( std::wostream* stream , const LogEntryW& entry );
				// Function dumping the contents from the log to the given stream starting from offset.
				bool _dump_log( std::wostream* stream  , const unsigned int offset );
				// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
				void _manage_log_size();
				// Function responsible of handling the auto-dump filestream.
				void _manage_dump_filestream();
				// Function responsible of parsing the parameters that were given to the log functions.
				std::wstring _parse_parameters( const wchar_t* input , va_list parameters );
				// Function responsible of adding a new entry of the given type and with the given message to the log.
				void _log_entry( const LogEntryType& type , const std::wstring& message );


			public:

				// Function responsible of initialising the instance of the class. Returns true on success.
				ATHENA_DLL static bool initialise();
				// Function responsible of deinitialising the instance of the class.
				ATHENA_DLL static void deinitialise();
				// Function returning a pointer to the single instance of the class.
				ATHENA_DLL static LogManagerW* get();


				// Function responsible of setting the maximum size of the log.
				ATHENA_DLL void max_log_size( const unsigned int size );
				// Function responsible of enabling or disabling the auto purge functionality.
				ATHENA_DLL void auto_purge( const bool value );
				// Function responsible of setting the threshold of the auto-purge functionality.
				ATHENA_DLL void auto_purge_threshold( const unsigned int threshold );
				// Function responsible of setting the name of the file that is used for the auto dump filestream.
				ATHENA_DLL void auto_dump_filename( const std::wstring& filename );
				// Function responsible of setting the open mode for the auto dump filestream.
				ATHENA_DLL void auto_dump_file_open_mode( const LogFileOpenMode mode );
				// Function responsible of setting the threshold for the auto dump functionality.
				ATHENA_DLL void auto_dump_threshold( const unsigned int threshold );
				// Function responsible of enabling or disabling the auto dump functionality.
				ATHENA_DLL void auto_dump( const bool value );
				// Function responsible of setting the stream that is used for the echo functionality.
				ATHENA_DLL void echo_stream( std::wostream* stream );
				// Function responsible of enabling or disabling the echoing the contents of the log.
				ATHENA_DLL void echo( const bool value );
				// Function responsible of setting the error tag.
				ATHENA_DLL void error_tag( const std::wstring& tag );
				// Function responsible of setting the warning tag.
				ATHENA_DLL void warning_tag( const std::wstring& tag );
				// Function responsible of setting the message tag.
				ATHENA_DLL void message_tag( const std::wstring& tag );
				// Function responsible of setting the type separator.
				ATHENA_DLL void type_separator( const std::wstring& separator );
				// Function responsible of setting the message separator.
				ATHENA_DLL void message_separator( const std::wstring& separator );
				// Function responsible of setting the timestamp separator.
				ATHENA_DLL void timestamp_separator( const std::wstring& separator );
				// Function responsible of logging an error with the given message.
				ATHENA_DLL void logError( const std::wstring& message );
				// Function responsible of logging an error with the contents of the given exception.
				ATHENA_DLL void logError( const std::exception& exception );
				// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logError( const wchar_t* format , ... );
				// Function responsible of logging a warning with the given message.
				ATHENA_DLL void logWarning( const std::wstring& message );
				// Function responsible of logging a warning with the contents of the given exception.
				ATHENA_DLL void logWarning( const std::exception& exception );
				// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logWarning( const wchar_t* message , ... );
				// Function responsible of logging a message with the given message.
				ATHENA_DLL void logMessage( const std::wstring& message );
				// Function responsible of logging a message with the contents of the given exception.
				ATHENA_DLL void logMessage( const std::exception& exception );
				// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
				ATHENA_DLL void logMessage( const wchar_t* message , ... );
				// Function responsible of dumping the log the file with the given filename.
				ATHENA_DLL void dump_log( const std::wstring& filename , const LogFileOpenMode mode = Append );
				// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
				ATHENA_DLL void purge_log();


				// Function returning the maximum size of the log.
				ATHENA_DLL unsigned int max_log_size() const;
				// Function returning whether the auto purge functionality is enabled.
				ATHENA_DLL bool auto_purge() const;
				// Function returning the threshold of the auto purge functionality.
				ATHENA_DLL unsigned int auto_purge_threshold() const;
				// Function returning the name of the file that is used in the auto dump functionality.
				ATHENA_DLL std::wstring auto_dump_filename() const;
				// Function returning the open mode of the auto dump file.
				ATHENA_DLL LogFileOpenMode auto_dump_file_open_mode() const;
				// Function returning the threshold of the auto dump functionality.
				ATHENA_DLL unsigned int auto_dump_threshold() const;
				// Function returning whether the auto dump functionality is enabled.
				ATHENA_DLL bool auto_dump() const;
				// Function returning a pointer to the stream that is used for echo operations.
				ATHENA_DLL std::wostream* echo_stream() const;
				// Function returning whether echoing is enabled.
				ATHENA_DLL bool echo() const;
				// Function returning the error tag.
				ATHENA_DLL std::wstring error_tag() const;
				// Function returning the warning tag.
				ATHENA_DLL std::wstring warning_tag() const;
				// Function returning the message tag.
				ATHENA_DLL std::wstring message_tag() const;
				// Function returning the type separator.
				ATHENA_DLL std::wstring type_separator() const;
				// Function returning the message separator.
				ATHENA_DLL std::wstring message_separator() const;
				// Function returning the timestamp separator.
				ATHENA_DLL std::wstring timestamp_separator() const;
				// Function returning the size of the log.
				ATHENA_DLL unsigned int log_size() const;
				// Function returning the entry at the given index. If the index is greater than the size of the log the last entry is returned.
				ATHENA_DLL LogEntryW entry( const unsigned int index = INFINITE ) const;
				// Function returning the desired number of entries from the given start.
				ATHENA_DLL std::deque<LogEntryW> entries( const unsigned int number, const unsigned int start = 0 ) const;
		};

	} /* io */

} /* athena */


#include "logManager.inl"



#endif /* ATHENA_IO_LOGMANAGER_HPP */


