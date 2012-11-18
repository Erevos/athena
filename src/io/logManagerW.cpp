#include "logManager.hpp"
#include <iostream>
#include <cwchar>



namespace athena
{

	namespace io
	{

		// The single instance of the class.
		LogManagerW* LogManagerW::_instance = INVALID_POINTER;
		// The loc that is used to handle concurrency issues regarding the instance of the class.
		utility::ReadersWriterLock LogManagerW::_instance_lock;
		// The maximum possible size for the buffer that is used in the functions of the class.
		const unsigned int LogManagerW::_MAX_BUFFER_SIZE = 1024;


		// The constructor of the class.
		LogManagerW::LogManagerW() : 
			_log(0) , 
			_lock() , 
			_auto_dump_filestream() , 
			_auto_dump_filename(L"App.log") , 
			_error_tag(L"Error:") , 
			_warning_tag(L"Warning:") , 
			_message_tag(L"Message:") , 
			_echo_stream(&std::wcout) , 
			_max_log_size(10000) , 
			_auto_purge_threshold(10000) , 
			_auto_dump_threshold(10000) , 
			_auto_dump_count(0) , 
			_auto_dump_offset(0) , 
			_auto_dump_file_open_mode(std::ofstream::app) , 
			_type_separator(L" ") ,
			_message_separator(L" ") ,
			_timestamp_separator(L" ") , 
			_auto_purge(false) , 
			_auto_dump(false) , 
			_echo(false)
		{
		};

		// The destructor of the class.
		LogManagerW::~LogManagerW()
		{
			if ( _auto_dump_filestream.is_open() )
				_auto_dump_filestream.close();
		};


		// Function responsible of generating a string with the current timestamp.
		inline std::wstring LogManagerW::_generate_timestamp()
		{
			// The current time.
			time_t now = time(INVALID_POINTER);
			// The struct that is used to store the date and time.
			struct tm date;
			// The stream that is used to buffer the output stream.
			std::wstringstream return_value_buffer;


			// Setting the date struct to zero.
			memset(&date,'\0',sizeof(date));

			// Get the current local time and date from the time_t value.

			#ifdef _WIN32
				localtime_s(&date,&now);
			#else
				memcpy(&date,localtime(&now),sizeof(tm));
			#endif /* _WIN32 */

			// Append a zero to the day of the month if it is less than 10.
			if ( date.tm_mday < 10 )
				return_value_buffer << '0';

			// Output the day of the month.
			return_value_buffer	<< date.tm_mday << '/';

			// Append a zero to the month if it is less than 9.
			if ( date.tm_mon < 9 )
				return_value_buffer << '0';

			// Output the month of the year.
			return_value_buffer << (date.tm_mon + 1) << '/';
			// Output the year and the separator.
			return_value_buffer << (date.tm_year+1900) << _timestamp_separator;
				
			// Append a zero to the hour if they are less than 10.
			if ( date.tm_hour < 10 )
				return_value_buffer << '0';

			// Output the hours.
			return_value_buffer << date.tm_hour << ':';

			// Append a zero to the minutes if they are less than 10.
			if ( date.tm_min < 10 )
				return_value_buffer << '0';

			// Output the minutes
			return_value_buffer << date.tm_min << ':';

			// Append a zero to the seconds if they are less than 10.
			if ( date.tm_sec < 10 )
				return_value_buffer << '0';

			// Output the seconds.
			return_value_buffer << date.tm_sec;


			// Output the buffer as a string.
			return return_value_buffer.str();
		};

		// Function responsible of updating the timestamps of the log with the new separator.
		void LogManagerW::_update_timestamps( const std::wstring& new_separator )
		{
			// For every entry in the log
			for ( std::deque<LogEntryW>::iterator log_iterator = _log.begin();  log_iterator != _log.begin();  ++log_iterator )
			{
				// Get the timestamp of the entry.
				std::wstring timestamp(log_iterator->timestamp());
				size_t pos = 0;


				// While the current timestamp separator can be found in the timestamp.
				while ( pos != timestamp.npos )
				{
					// Find the first occurrence of the current timestamp.
					pos = timestamp.find_first_of(_timestamp_separator,pos);

					// If the current timestamp was found.
					if ( pos != timestamp.npos )
					{
						// Replace the current timestamp with the new one.
						timestamp.replace(pos,_timestamp_separator.size(),new_separator);
						// Update the search counter.
						pos += new_separator.size();
					}
				}

				// Set the timestamp of the entry to the updated timestamp.
				log_iterator->timestamp(timestamp);
			}
		};

		// Function posting an entry to the given stream.
		inline void LogManagerW::_post_entry( std::wostream* stream , const LogEntryW& entry )
		{
			// Output to the stream the timestamp of the entry and the type separator.
			(*stream) << entry.timestamp() << _type_separator;

			// Depending on the type of the entry output the proper tag.
			switch ( entry.type() )
			{
				case Error:

					(*stream) << _error_tag;
					break;

				case Warning:

					(*stream) << _warning_tag;
					break;

				case Message:

					(*stream) << _message_tag;
					break;
			};

			// Output the message separator and the message of the entry.
			(*stream) << _message_separator << entry.message() << std::endl;
		};

		// Function dumping the contents from the log to the given stream starting from offset.
		bool LogManagerW::_dump_log( std::wostream* stream , const unsigned int offset )
		{
			bool return_value = false;


			// If the output stream is in a good condition.
			if ( stream->good() )
			{
				// Output the log to the stream, starting from the given offset.
				for ( std::deque<LogEntryW>::iterator log_iterator = _log.begin()+offset;  log_iterator != _log.end();  ++log_iterator )
					_post_entry(stream,(*log_iterator));

				// If the stream is in a good condition at the end of the output.
				if ( stream->good() )
					return_value = true;	// The log was dumped successfully.
			}


			return return_value;
		};

		// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
		inline void LogManagerW::_manage_log_size()
		{
			// If the auto dump functionality is enabled
			if ( _auto_dump )
			{
				/*
					If the auto dump threshold has been reached or
					if the auto purge functionality is enabled and the size of the log has reached the 
					auto purge threshold.
				*/
				if ( _auto_dump_count > _auto_dump_threshold  || ( _auto_purge  &&  _log.size() > _auto_purge_threshold ) )
				{
					// Dump the log to the auto dump filestream.
					if ( _dump_log(&_auto_dump_filestream,_auto_dump_offset) )
					{
						// Reset the auto dump counter.
						_auto_dump_count = 0;
						// Set the auto dump offset to the size of the log since the log has been dumped.
						_auto_dump_offset = _log.size();
					}
				}
			}

			// If the auto purge functionality is enabled.
			if ( _auto_purge )
			{
				// If the size of the log is greater than the auto purge threshold.
				if ( _log.size() > _auto_purge_threshold )
				{
					// Reset the auto dump count.
					_auto_dump_count = 0;
					// Reset the auto dump offset.
					_auto_dump_offset = 0;
					// Clear the log.
					_log.clear();
				}
			}

			// If the size of the log is greater than the maximum log size.
			if ( _log.size() > _max_log_size )
				_log.pop_front();	// Remove the first entry to the log in order to keep the size constant.
		};

		// Function responsible of handling the auto-dump filestream.
		inline void LogManagerW::_manage_dump_filestream()
		{
			// If the auto dump file is open.
			if ( _auto_dump_filestream.is_open() )
				_auto_dump_filestream.close(); // Close the auto dump file.

			// Open the auto dump file.
			_auto_dump_filestream.open(_auto_dump_filename.c_str(),_auto_dump_filestream.in|_auto_dump_file_open_mode);
		};

		// Function responsible of parsing the parameters that were given to the log functions.
		std::wstring LogManagerW::_parse_parameters( const wchar_t* input , va_list parameters )
		{
			wchar_t buffer[_MAX_BUFFER_SIZE+1];
			

			// Set the buffer to 0.
			memset(buffer,'\0',_MAX_BUFFER_SIZE+1);
			
			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vswprintf(buffer,_MAX_BUFFER_SIZE,input,parameters);
			#else
				vswprintf(buffer,_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		};

		// Function responsible of adding a new entry of the given type and with the given message to the log.
		inline void LogManagerW::_log_entry( const LogEntryType& type , const std::wstring& message )
		{
			// Insert the new entry to the log.
			_log.push_back(LogEntryW(type,message,_generate_timestamp()));
			// Increase the auto dump counter.
			++_auto_dump_count;

			// If echoing is enabled.
			if ( _echo )
				_post_entry(_echo_stream,(*_log.rbegin())); // Output the new entry to the echo stream.
				
			// Manage the size of the log and perform the auto-dump and auto-purge functionality if needed.
			_manage_log_size();
		};


		// Function responsible of initialising the instance of the class. Returns true on success.
		inline bool LogManagerW::initialise()
		{
			bool return_value = false;


			_instance_lock.lock(false);

			if ( _instance == INVALID_POINTER )
			{
				_instance  = new (std::nothrow) LogManagerW();
				return_value = ( _instance != INVALID_POINTER );
			}

			_instance_lock.unlock();


			return return_value;
		};
			
		// Function responsible of deinitialising the instance of the class.
		inline void LogManagerW::deinitialise()
		{
			_instance_lock.lock(false);

			if ( _instance != INVALID_POINTER )
			{
				delete _instance;
				_instance = INVALID_POINTER;
			}

			_instance_lock.unlock();
		};
			
		// Function returning a pointer to the single instance of the class.
		inline LogManagerW* LogManagerW::get()
		{
			LogManagerW* return_value = INVALID_POINTER;


			_instance_lock.lock(true);
			return_value = _instance;
			_instance_lock.unlock();


			return return_value;
		};


		// Function responsible of setting the maximum size of the log.
		inline void LogManagerW::max_log_size( const unsigned int size )
		{
			_lock.lock(false);
			_max_log_size = size;
			_manage_log_size();
			_lock.unlock();
		};

		// Function responsible of enabling or disabling the auto purge functionality.
		inline void LogManagerW::auto_purge( const bool value )
		{
			_lock.lock(false);
			_auto_purge = value;
			_manage_log_size();
			_lock.unlock();
		};

		// Function responsible of setting the threshold of the auto-purge functionality.
		inline void LogManagerW::auto_purge_threshold( const unsigned int threshold )
		{
			_lock.lock(false);
			_auto_purge_threshold = threshold;
			_manage_log_size();
			_lock.unlock();
		};

		// Function responsible of setting the name of the file that is used for the auto dump filestream.
		inline void LogManagerW::auto_dump_filename( const std::wstring& filename )
		{
			_lock.lock(false);
			_auto_dump_filename = filename;
			_manage_dump_filestream();
			_lock.unlock();
		};

		// Function responsible of setting the open mode for the auto dump filestream.
		inline void LogManagerW::auto_dump_file_open_mode( const LogFileOpenMode mode )
		{
			_lock.lock(false);

			if ( mode == Truncate )
				_auto_dump_file_open_mode = _auto_dump_filestream.trunc;
			else
				_auto_dump_file_open_mode = _auto_dump_filestream.app;

			_manage_dump_filestream();
			_lock.unlock();
		};

		// Function responsible of setting the threshold for the auto dump functionality.
		inline void LogManagerW::auto_dump_threshold( const unsigned int threshold )
		{
			_lock.lock(false);
			_auto_dump_threshold = threshold;
			_manage_dump_filestream();
			_lock.unlock();
		};

		// Function responsible of enabling or disabling the auto dump functionality.
		inline void LogManagerW::auto_dump( const bool value )
		{
			_lock.lock(false);
			_auto_dump = value;
			_manage_dump_filestream();
			_manage_log_size();
			_lock.unlock();
		};

		// Function responsible of setting the stream that is used for the echo functionality.
		inline void LogManagerW::echo_stream( std::wostream* stream )
		{
			_lock.lock(false);

			if ( !_echo  ||  stream != INVALID_POINTER )
				_echo_stream = stream;
				
			_lock.unlock();
		};

		// Function responsible of enabling or disabling the echoing the contents of the log.
		inline void LogManagerW::echo( const bool value )
		{
			_lock.lock(false);

			if ( !(  _echo_stream == INVALID_POINTER  &&  value ) )
				_echo = value;

			_lock.unlock();
		};

		// Function responsible of setting the error tag.
		inline void LogManagerW::error_tag( const std::wstring& tag )
		{
			_lock.lock(false);
			_error_tag = tag;
			_lock.unlock();
		};

		// Function responsible of setting the warning tag.
		inline void LogManagerW::warning_tag( const std::wstring& tag )
		{
			_lock.lock(false);
			_warning_tag = tag;
			_lock.unlock();
		};

		// Function responsible of setting the message tag.
		inline void LogManagerW::message_tag( const std::wstring& tag )
		{
			_lock.lock(false);
			_message_tag = tag;
			_lock.unlock();
		};

		// Function responsible of setting the type separator.
		inline void LogManagerW::type_separator( const std::wstring& separator )
		{
			_lock.lock(false);
			_type_separator = separator;
			_lock.unlock();
		};

		// Function responsible of setting the message separator.
		inline void LogManagerW::message_separator( const std::wstring& separator )
		{
			_lock.lock(false);
			_message_separator = separator;
			_lock.unlock();
		};

		// Function responsible of setting the timestamp separator.
		inline void LogManagerW::timestamp_separator( const std::wstring& separator )
		{
			_lock.lock(false);
			_update_timestamps(separator);
			_lock.unlock();
		};

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::logError( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			_log_entry(Error,_parse_parameters(format,arguments));
			va_end(arguments);
		};

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::logWarning( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			_log_entry(Warning,_parse_parameters(format,arguments));
			va_end(arguments);
		};

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManagerW::logMessage( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			_log_entry(Message,_parse_parameters(format,arguments));
			va_end(arguments);
		};

		// Function responsible of dumping the log the file with the given filename.
		void LogManagerW::dump_log( const std::wstring& filename , const LogFileOpenMode mode )
		{
			std::wofstream stream(filename.c_str(),( mode == Append  ?  std::wofstream::ate : std::wofstream::trunc ));


			if ( stream.is_open() )
				_dump_log(&stream,0);
		};
			
		// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
		void LogManagerW::purge_log()
		{
			_lock.lock(false);
			_manage_log_size();
			_log.clear();
			_auto_dump_count = 0;
			_auto_dump_offset = 0;
			_lock.unlock();
		};


		// Function returning the maximum size of the log.
		unsigned int LogManagerW::max_log_size() const
		{
			unsigned int return_value = 0;


			_lock.lock(true);
			return_value = _max_log_size;
			_lock.unlock();


			return return_value;
		};

		// Function returning whether the auto purge functionality is enabled.
		bool LogManagerW::auto_purge() const
		{
			bool return_value = false;


			_lock.lock(true);
			return_value = _auto_purge;
			_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto purge functionality.
		unsigned int LogManagerW::auto_purge_threshold() const
		{
			unsigned int return_value = 0;


			_lock.lock(true);
			return_value = _auto_purge_threshold;
			_lock.unlock();


			return return_value;
		};

		// Function returning the name of the file that is used in the auto dump functionality.
		std::wstring LogManagerW::auto_dump_filename() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _auto_dump_filename;
			_lock.unlock();


			return return_value;
		};

		// Function returning the open mode of the auto dump file.
		LogFileOpenMode LogManagerW::auto_dump_file_open_mode() const
		{
			LogFileOpenMode return_value = Append;


			_lock.lock(true);

			if ( _auto_dump_file_open_mode == _auto_dump_filestream.trunc )
				return_value = Truncate;

			_lock.unlock();


			return return_value;
		};

		// Function returning the threshold of the auto dump functionality.
		unsigned int LogManagerW::auto_dump_threshold() const
		{
			unsigned int return_value = 0;


			_lock.lock(true);
			return_value = _auto_dump_threshold;
			_lock.unlock();


			return return_value;
		};

		// Function returning whether the auto dump functionality is enabled.
		bool LogManagerW::auto_dump() const
		{
			bool return_value = false;


			_lock.lock(true);
			return_value = _auto_dump;
			_lock.unlock();


			return return_value;
		};

		// Function returning a pointer to the stream that is used for echo operations.
		std::wostream* LogManagerW::echo_stream() const
		{
			std::wostream* return_value = INVALID_POINTER;


			_lock.lock(true);
			return_value = _echo_stream;
			_lock.unlock();


			return return_value;
		};

		// Function returning whether echoing is enabled.
		bool LogManagerW::echo() const
		{
			bool return_value = false;


			_lock.lock(true);
			return_value = _echo;
			_lock.unlock();


			return return_value;
		};

		// Function returning the error tag.
		std::wstring LogManagerW::error_tag() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _error_tag;
			_lock.unlock();


			return return_value;
		};

		// Function returning the warning tag.
		std::wstring LogManagerW::warning_tag() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _warning_tag;
			_lock.unlock();


			return return_value;
		};

		// Function returning the message tag.
		std::wstring LogManagerW::message_tag() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _message_tag;
			_lock.unlock();


			return return_value;
		};

		// Function returning the type separator.
		std::wstring LogManagerW::type_separator() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _type_separator;
			_lock.unlock();


			return return_value;
		};

		// Function returning the message separator.
		std::wstring LogManagerW::message_separator() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _message_separator;
			_lock.unlock();


			return return_value;
		};

		// Function returning the timestamp separator.
		std::wstring LogManagerW::timestamp_separator() const
		{
			std::wstring return_value(L"");


			_lock.lock(true);
			return_value = _timestamp_separator;
			_lock.unlock();


			return return_value;
		};

		// Function returning the size of the log.
		unsigned int LogManagerW::log_size() const
		{
			unsigned int return_value = 0;


			_lock.lock(true);
			return_value = _log.size();
			_lock.unlock();


			return return_value;
		};

		// Function returning the entry at the given index. If the index is greater than the size of the log the last entry is returned.
		LogEntryW LogManagerW::entry( const unsigned int index ) const
		{
			LogEntryW return_value;


			_lock.lock(true);

			// If the index is valid.
			if ( index < _log.size() )
				return_value = _log[index]; // Return the entry at the desired index.
			else if ( _log.size() > 0 ) // Else return the last entry.
				return_value = (*_log.rbegin());

			_lock.unlock();


			return return_value;
		};
			
		// Function returning the desired number of entries from the given start.
		std::deque<LogEntryW> LogManagerW::entries( const unsigned int number, const unsigned int start ) const
		{
			std::deque<LogEntryW> return_value(0);
			std::deque<LogEntryW>::const_iterator start_position;


			_lock.lock(true);
			// Set the start position at the desired start position.
			start_position = _log.begin() + start;
			// Copy the log, from the start position, number entries.
			return_value.insert(return_value.begin(),start_position,start_position+number+1);
			_lock.unlock();


			return return_value;
		};


	} /* io */

} /* athena */