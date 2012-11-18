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
					ATHENA_PRECALL LogManagerA() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_PRECALL ~LogManagerA() ATHENA_POSTCALL;


					// Function responsible of generating a string with the current timestamp.
					std::string ATHENA_PRECALL _generate_timestamp() ATHENA_POSTCALL;
					// Function responsible of updating the timestamps of the log with the new separator.
					void ATHENA_PRECALL _update_timestamps( const std::string& new_separator ) ATHENA_POSTCALL;
					// Function posting an entry to the given stream.
					void ATHENA_PRECALL _post_entry( std::ostream* stream , const LogEntryA& entry ) ATHENA_POSTCALL;
					// Function dumping the contents from the log to the given stream starting from offset.
					bool ATHENA_PRECALL _dump_log( std::ostream* stream  , const unsigned int offset ) ATHENA_POSTCALL;
					// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
					void ATHENA_PRECALL _manage_log_size() ATHENA_POSTCALL;
					// Function responsible of handling the auto-dump filestream.
					void ATHENA_PRECALL _manage_dump_filestream() ATHENA_POSTCALL;
					// Function responsible of parsing the parameters that were given to the log functions.
					std::string ATHENA_PRECALL _parse_parameters( const char* input , va_list parameters ) ATHENA_POSTCALL;
					// Function responsible of adding a new entry of the given type and with the given message to the log.
					void ATHENA_PRECALL _log_entry( const LogEntryType& type , const std::string& message ) ATHENA_POSTCALL;


				public:

					// Function responsible of initialising the instance of the class. Returns true on success.
					ATHENA_DLL static bool ATHENA_PRECALL initialise() ATHENA_POSTCALL;
					// Function responsible of deinitialising the instance of the class.
					ATHENA_DLL static void ATHENA_PRECALL deinitialise() ATHENA_POSTCALL;
					// Function returning a pointer to the single instance of the class.
					ATHENA_DLL static LogManagerA* ATHENA_PRECALL get() ATHENA_POSTCALL;


					// Function responsible of setting the maximum size of the log.
					ATHENA_DLL void ATHENA_PRECALL max_log_size( const unsigned int size ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the auto purge functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_purge( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the threshold of the auto-purge functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_purge_threshold( const unsigned int threshold ) ATHENA_POSTCALL;
					// Function responsible of setting the name of the file that is used for the auto dump filestream.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_filename( const std::string& filename ) ATHENA_POSTCALL;
					// Function responsible of setting the open mode for the auto dump filestream.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_file_open_mode( const LogFileOpenMode mode ) ATHENA_POSTCALL;
					// Function responsible of setting the threshold for the auto dump functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_threshold( const unsigned int threshold ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the auto dump functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_dump( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the stream that is used for the echo functionality.
					ATHENA_DLL void ATHENA_PRECALL echo_stream( std::ostream* stream ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the echoing the contents of the log.
					ATHENA_DLL void ATHENA_PRECALL echo( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the error tag.
					ATHENA_DLL void ATHENA_PRECALL error_tag( const std::string& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the warning tag.
					ATHENA_DLL void ATHENA_PRECALL warning_tag( const std::string& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the message tag.
					ATHENA_DLL void ATHENA_PRECALL message_tag( const std::string& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the type separator.
					ATHENA_DLL void ATHENA_PRECALL type_separator( const std::string& separator ) ATHENA_POSTCALL;
					// Function responsible of setting the message separator.
					ATHENA_DLL void ATHENA_PRECALL message_separator( const std::string& separator ) ATHENA_POSTCALL;
					// Function responsible of setting the timestamp separator.
					ATHENA_DLL void ATHENA_PRECALL timestamp_separator( const std::string& separator ) ATHENA_POSTCALL;
					// Function responsible of logging an error with the given message.
					ATHENA_DLL void ATHENA_PRECALL logError( const std::string& message ) ATHENA_POSTCALL;
					// Function responsible of logging an error with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logError( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logError( const char* format , ... ) ATHENA_POSTCALL;
					// Function responsible of logging a warning with the given message.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const std::string& message ) ATHENA_POSTCALL;
					// Function responsible of logging a warning with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const char* message , ... ) ATHENA_POSTCALL;
					// Function responsible of logging a message with the given message.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const std::string& message ) ATHENA_POSTCALL;
					// Function responsible of logging a message with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const char* message , ... ) ATHENA_POSTCALL;
					// Function responsible of dumping the log the file with the given filename.
					ATHENA_DLL void ATHENA_PRECALL dump_log( const std::string& filename , const LogFileOpenMode mode = Append );
					// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
					ATHENA_DLL void ATHENA_PRECALL purge_log() ATHENA_POSTCALL;


					// Function returning the maximum size of the log.
					ATHENA_DLL unsigned int ATHENA_PRECALL max_log_size() const ATHENA_POSTCALL;
					// Function returning whether the auto purge functionality is enabled.
					ATHENA_DLL bool ATHENA_PRECALL auto_purge() const ATHENA_POSTCALL;
					// Function returning the threshold of the auto purge functionality.
					ATHENA_DLL unsigned int ATHENA_PRECALL auto_purge_threshold() const ATHENA_POSTCALL;
					// Function returning the name of the file that is used in the auto dump functionality.
					ATHENA_DLL std::string ATHENA_PRECALL auto_dump_filename() const ATHENA_POSTCALL;
					// Function returning the open mode of the auto dump file.
					ATHENA_DLL LogFileOpenMode ATHENA_PRECALL auto_dump_file_open_mode() const ATHENA_POSTCALL;
					// Function returning the threshold of the auto dump functionality.
					ATHENA_DLL unsigned int ATHENA_PRECALL auto_dump_threshold() const ATHENA_POSTCALL;
					// Function returning whether the auto dump functionality is enabled.
					ATHENA_DLL bool ATHENA_PRECALL auto_dump() const ATHENA_POSTCALL;
					// Function returning a pointer to the stream that is used for echo operations.
					ATHENA_DLL std::ostream* ATHENA_PRECALL echo_stream() const ATHENA_POSTCALL;
					// Function returning whether echoing is enabled.
					ATHENA_DLL bool ATHENA_PRECALL echo() const ATHENA_POSTCALL;
					// Function returning the error tag.
					ATHENA_DLL std::string ATHENA_PRECALL error_tag() const ATHENA_POSTCALL;
					// Function returning the warning tag.
					ATHENA_DLL std::string ATHENA_PRECALL warning_tag() const ATHENA_POSTCALL;
					// Function returning the message tag.
					ATHENA_DLL std::string ATHENA_PRECALL message_tag() const ATHENA_POSTCALL;
					// Function returning the type separator.
					ATHENA_DLL std::string ATHENA_PRECALL type_separator() const ATHENA_POSTCALL;
					// Function returning the message separator.
					ATHENA_DLL std::string ATHENA_PRECALL message_separator() const ATHENA_POSTCALL;
					// Function returning the timestamp separator.
					ATHENA_DLL std::string ATHENA_PRECALL timestamp_separator() const ATHENA_POSTCALL;
					// Function returning the size of the log.
					ATHENA_DLL unsigned int ATHENA_PRECALL log_size() const ATHENA_POSTCALL;
					// Function returning the entry at the given index. If the index is greater than the size of the log the last entry is returned.
					ATHENA_DLL LogEntryA ATHENA_PRECALL entry( const unsigned int index = INFINITE ) const ATHENA_POSTCALL;
					// Function returning the desired number of entries from the given start.
					ATHENA_DLL std::deque<LogEntryA> ATHENA_PRECALL entries( const unsigned int number, const unsigned int start = 0 ) const ATHENA_POSTCALL;
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
					ATHENA_PRECALL LogManagerW() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_PRECALL ~LogManagerW() ATHENA_POSTCALL;


					// Function responsible of generating a string with the current timestamp.
					std::wstring ATHENA_PRECALL _generate_timestamp() ATHENA_POSTCALL;
					// Function responsible of updating the timestamps of the log with the new separator.
					void ATHENA_PRECALL _update_timestamps( const std::wstring& new_separator ) ATHENA_POSTCALL;
					// Function posting an entry to the given stream.
					void ATHENA_PRECALL _post_entry( std::wostream* stream , const LogEntryW& entry ) ATHENA_POSTCALL;
					// Function dumping the contents from the log to the given stream starting from offset.
					bool ATHENA_PRECALL _dump_log( std::wostream* stream  , const unsigned int offset ) ATHENA_POSTCALL;
					// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
					void ATHENA_PRECALL _manage_log_size() ATHENA_POSTCALL;
					// Function responsible of handling the auto-dump filestream.
					void ATHENA_PRECALL _manage_dump_filestream() ATHENA_POSTCALL;
					// Function responsible of parsing the parameters that were given to the log functions.
					std::wstring ATHENA_PRECALL _parse_parameters( const wchar_t* input , va_list parameters ) ATHENA_POSTCALL;
					// Function responsible of adding a new entry of the given type and with the given message to the log.
					void ATHENA_PRECALL _log_entry( const LogEntryType& type , const std::wstring& message ) ATHENA_POSTCALL;


				public:

					// Function responsible of initialising the instance of the class. Returns true on success.
					ATHENA_DLL static bool ATHENA_PRECALL initialise() ATHENA_POSTCALL;
					// Function responsible of deinitialising the instance of the class.
					ATHENA_DLL static void ATHENA_PRECALL deinitialise() ATHENA_POSTCALL;
					// Function returning a pointer to the single instance of the class.
					ATHENA_DLL static LogManagerW* ATHENA_PRECALL get() ATHENA_POSTCALL;


					// Function responsible of setting the maximum size of the log.
					ATHENA_DLL void ATHENA_PRECALL max_log_size( const unsigned int size ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the auto purge functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_purge( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the threshold of the auto-purge functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_purge_threshold( const unsigned int threshold ) ATHENA_POSTCALL;
					// Function responsible of setting the name of the file that is used for the auto dump filestream.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_filename( const std::wstring& filename ) ATHENA_POSTCALL;
					// Function responsible of setting the open mode for the auto dump filestream.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_file_open_mode( const LogFileOpenMode mode ) ATHENA_POSTCALL;
					// Function responsible of setting the threshold for the auto dump functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_dump_threshold( const unsigned int threshold ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the auto dump functionality.
					ATHENA_DLL void ATHENA_PRECALL auto_dump( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the stream that is used for the echo functionality.
					ATHENA_DLL void ATHENA_PRECALL echo_stream( std::wostream* stream ) ATHENA_POSTCALL;
					// Function responsible of enabling or disabling the echoing the contents of the log.
					ATHENA_DLL void ATHENA_PRECALL echo( const bool value ) ATHENA_POSTCALL;
					// Function responsible of setting the error tag.
					ATHENA_DLL void ATHENA_PRECALL error_tag( const std::wstring& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the warning tag.
					ATHENA_DLL void ATHENA_PRECALL warning_tag( const std::wstring& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the message tag.
					ATHENA_DLL void ATHENA_PRECALL message_tag( const std::wstring& tag ) ATHENA_POSTCALL;
					// Function responsible of setting the type separator.
					ATHENA_DLL void ATHENA_PRECALL type_separator( const std::wstring& separator ) ATHENA_POSTCALL;
					// Function responsible of setting the message separator.
					ATHENA_DLL void ATHENA_PRECALL message_separator( const std::wstring& separator ) ATHENA_POSTCALL;
					// Function responsible of setting the timestamp separator.
					ATHENA_DLL void ATHENA_PRECALL timestamp_separator( const std::wstring& separator ) ATHENA_POSTCALL;
					// Function responsible of logging an error with the given message.
					ATHENA_DLL void ATHENA_PRECALL logError( const std::wstring& message ) ATHENA_POSTCALL;
					// Function responsible of logging an error with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logError( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logError( const wchar_t* format , ... ) ATHENA_POSTCALL;
					// Function responsible of logging a warning with the given message.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const std::wstring& message ) ATHENA_POSTCALL;
					// Function responsible of logging a warning with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logWarning( const wchar_t* message , ... ) ATHENA_POSTCALL;
					// Function responsible of logging a message with the given message.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const std::wstring& message ) ATHENA_POSTCALL;
					// Function responsible of logging a message with the contents of the given exception.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const std::exception& exception ) ATHENA_POSTCALL;
					// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
					ATHENA_DLL void ATHENA_PRECALL logMessage( const wchar_t* message , ... ) ATHENA_POSTCALL;
					// Function responsible of dumping the log the file with the given filename.
					ATHENA_DLL void ATHENA_PRECALL dump_log( const std::wstring& filename , const LogFileOpenMode mode = Append );
					// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
					ATHENA_DLL void ATHENA_PRECALL purge_log() ATHENA_POSTCALL;


					// Function returning the maximum size of the log.
					ATHENA_DLL unsigned int ATHENA_PRECALL max_log_size() const ATHENA_POSTCALL;
					// Function returning whether the auto purge functionality is enabled.
					ATHENA_DLL bool ATHENA_PRECALL auto_purge() const ATHENA_POSTCALL;
					// Function returning the threshold of the auto purge functionality.
					ATHENA_DLL unsigned int ATHENA_PRECALL auto_purge_threshold() const ATHENA_POSTCALL;
					// Function returning the name of the file that is used in the auto dump functionality.
					ATHENA_DLL std::wstring ATHENA_PRECALL auto_dump_filename() const ATHENA_POSTCALL;
					// Function returning the open mode of the auto dump file.
					ATHENA_DLL LogFileOpenMode ATHENA_PRECALL auto_dump_file_open_mode() const ATHENA_POSTCALL;
					// Function returning the threshold of the auto dump functionality.
					ATHENA_DLL unsigned int ATHENA_PRECALL auto_dump_threshold() const ATHENA_POSTCALL;
					// Function returning whether the auto dump functionality is enabled.
					ATHENA_DLL bool ATHENA_PRECALL auto_dump() const ATHENA_POSTCALL;
					// Function returning a pointer to the stream that is used for echo operations.
					ATHENA_DLL std::wostream* ATHENA_PRECALL echo_stream() const ATHENA_POSTCALL;
					// Function returning whether echoing is enabled.
					ATHENA_DLL bool ATHENA_PRECALL echo() const ATHENA_POSTCALL;
					// Function returning the error tag.
					ATHENA_DLL std::wstring ATHENA_PRECALL error_tag() const ATHENA_POSTCALL;
					// Function returning the warning tag.
					ATHENA_DLL std::wstring ATHENA_PRECALL warning_tag() const ATHENA_POSTCALL;
					// Function returning the message tag.
					ATHENA_DLL std::wstring ATHENA_PRECALL message_tag() const ATHENA_POSTCALL;
					// Function returning the type separator.
					ATHENA_DLL std::wstring ATHENA_PRECALL type_separator() const ATHENA_POSTCALL;
					// Function returning the message separator.
					ATHENA_DLL std::wstring ATHENA_PRECALL message_separator() const ATHENA_POSTCALL;
					// Function returning the timestamp separator.
					ATHENA_DLL std::wstring ATHENA_PRECALL timestamp_separator() const ATHENA_POSTCALL;
					// Function returning the size of the log.
					ATHENA_DLL unsigned int ATHENA_PRECALL log_size() const ATHENA_POSTCALL;
					// Function returning the entry at the given index. If the index is greater than the size of the log the last entry is returned.
					ATHENA_DLL LogEntryW ATHENA_PRECALL entry( const unsigned int index = INFINITE ) const ATHENA_POSTCALL;
					// Function returning the desired number of entries from the given start.
					ATHENA_DLL std::deque<LogEntryW> ATHENA_PRECALL entries( const unsigned int number, const unsigned int start = 0 ) const ATHENA_POSTCALL;
			};

		} /* io */

	} /* athena */


	#include "logManager.inl"



#endif /* ATHENA_IO_LOGMANAGER_HPP */


