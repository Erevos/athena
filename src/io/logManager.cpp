#include "logManager.hpp"
#include <cstring>
#include <iostream>
#include <cwchar>



namespace athena
{

	namespace io
	{

		/*
			ASCII Log Manager definitions.
		*/


		// The single instance of the class.
		LogManagerA* LogManagerA::s_instance = NULL;
		// The loc that is used to handle concurrency issues regarding the instance of the class.
		std::mutex LogManagerA::s_instance_lock;
		// The maximum possible size for the buffer that is used in the functions of the class.
		const unsigned int LogManagerA::s_MAX_BUFFER_SIZE = 1024;


		// The constructor of the class.
		LogManagerA::LogManagerA() :
			m_log(0),
			m_lock(),
			m_auto_dump_filestream(),
			m_auto_dump_filename("App.log"),
			m_error_tag("Error:"),
			m_warning_tag("Warning:"),
			m_message_tag("Message:"),
			m_type_separator(" "),
			m_message_separator(" "),
			m_timestamp_separator(" "),
			m_auto_dump_file_open_mode(std::ofstream::app),
			m_echo_stream(&std::cout),
			m_max_log_size(10000),
			m_auto_purge_threshold(10000),
			m_auto_dump_threshold(10000),
			m_auto_dump_count(0),
			m_auto_dump_offset(0),
			m_auto_purge(false),
			m_auto_dump(false),
			m_echo(false)
		{
		}

		// The destructor of the class.
		LogManagerA::~LogManagerA()
		{
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close();

			f_cleanup();
		}


		// Function responsible of generating a string with the current timestamp.
		inline std::string LogManagerA::f_generate_timestamp()
		{
			// The current time.
			time_t now = time(INVALID_POINTER);
			// The struct that is used to store the date and time.
			struct tm date;
			// The stream that is used to buffer the output stream.
			std::stringstream return_value_buffer;


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
			return_value_buffer << (date.tm_year+1900) << m_timestamp_separator;

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
		}

		// Function responsible of updating the timestamps of the log with the new separator.
		void LogManagerA::f_update_timestamps( const std::string& new_separator )
		{
			// For every entry in the log
			for ( std::deque<LogEntryA*>::iterator log_iterator = m_log.begin();  log_iterator != m_log.begin();  ++log_iterator )
			{
				// Get the timestamp of the entry.
				std::string timestamp((*log_iterator)->timestamp());
				size_t pos = 0;


				// While the current timestamp separator can be found in the timestamp.
				while ( pos != timestamp.npos )
				{
					// Find the first occurrence of the current timestamp.
					pos = timestamp.find_first_of(m_timestamp_separator,pos);

					// If the current timestamp was found.
					if ( pos != timestamp.npos )
					{
						// Replace the current timestamp with the new one.
						timestamp.replace(pos,m_timestamp_separator.size(),new_separator);
						// Update the search counter.
						pos += new_separator.size();
					}
				}

				// Set the timestamp of the entry to the updated timestamp.
				(*log_iterator)->timestamp(timestamp);
			}
		}

		// Function posting an entry to the given stream.
		void LogManagerA::f_post_entry( std::ostream* stream , const LogEntryA& entry )
		{
			// Output to the stream the timestamp of the entry and the type separator.
			(*stream) << entry.timestamp() << m_type_separator;

			// Depending on the type of the entry output the proper tag.
			switch ( entry.type() )
			{
				case Error:

					(*stream) << m_error_tag;
					break;

				case Warning:

					(*stream) << m_warning_tag;
					break;

				case Message:

					(*stream) << m_message_tag;
					break;
			}

			// Output the message separator and the message of the entry.
			(*stream) << m_message_separator << entry.message() << std::endl;
		}

		// Function dumping the contents from the log to the given stream starting from offset.
		bool LogManagerA::f_dump_log( std::ostream* stream , const unsigned int offset )
		{
			bool return_value = false;


			// If the output stream is in a good condition.
			if ( stream->good() )
			{
				// Output the log to the stream, starting from the given offset.
				for ( std::deque<LogEntryA*>::iterator log_iterator = m_log.begin()+offset;  log_iterator != m_log.end();  ++log_iterator )
					f_post_entry(stream,(*(*log_iterator)));

				// If the stream is in a good condition at the end of the output.
				if ( stream->good() )
					return_value = true;	// The log was dumped successfully.
			}


			return return_value;
		}

		// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
		void LogManagerA::f_manage_log_size()
		{
			// If the auto dump functionality is enabled
			if ( m_auto_dump )
			{
				/*
					If the auto dump threshold has been reached or
					if the auto purge functionality is enabled and the size of the log has reached the 
					auto purge threshold.
				*/
				if ( m_auto_dump_count > m_auto_dump_threshold  || ( m_auto_purge  &&  m_log.size() > m_auto_purge_threshold ) )
				{
					// Dump the log to the auto dump filestream.
					if ( f_dump_log(&m_auto_dump_filestream,m_auto_dump_offset) )
					{
						// Reset the auto dump counter.
						m_auto_dump_count = 0;
						// Set the auto dump offset to the size of the log since the log has been dumped.
						m_auto_dump_offset = m_log.size();
					}
				}
			}

			// If the auto purge functionality is enabled.
			if ( m_auto_purge )
			{
				// If the size of the log is greater than the auto purge threshold.
				if ( m_log.size() > m_auto_purge_threshold )
				{
					// Reset the auto dump count.
					m_auto_dump_count = 0;
					// Reset the auto dump offset.
					m_auto_dump_offset = 0;
					// Clear the log.
					f_cleanup();
				}
			}

			// If the size of the log is greater than the maximum log size.
			if ( m_log.size() > m_max_log_size )
			{
				delete m_log[0];
				m_log.pop_front();	// Remove the first entry to the log in order to keep the size constant.
			}
		}

		// Function responsible of performing cleanup of the allocated log entries.
		void LogManagerA::f_cleanup()
		{
			for ( std::deque<LogEntryA*>::iterator log_iterator = m_log.begin();  log_iterator != m_log.end();  ++log_iterator )
			{
				delete (*log_iterator);
			}

			m_log.clear();
		}

		// Function responsible of handling the auto-dump filestream.
		void LogManagerA::f_manage_dump_filestream()
		{
			// If the auto dump file is open.
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close(); // Close the auto dump file.

			// Open the auto dump file.
			m_auto_dump_filestream.open(m_auto_dump_filename,m_auto_dump_filestream.in|m_auto_dump_file_open_mode);
		}

		// Function responsible of parsing the parameters that were given to the log functions.
		std::string LogManagerA::f_parse_parameters( const char* input , va_list parameters )
		{
			char buffer[s_MAX_BUFFER_SIZE+1];


			// Set the buffer to 0.
			memset(buffer,'\0',sizeof(char)*(s_MAX_BUFFER_SIZE+1));

			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vsnprintf_s(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#else
				vsnprintf(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		}

		// Function responsible of parsing the parameters that were given to the log functions.
		std::wstring LogManagerA::f_parse_wide_parameters( const wchar_t* input , va_list parameters )
		{
			wchar_t buffer[s_MAX_BUFFER_SIZE+1];


			// Set the buffer to 0.
			memset(buffer,L'\0',sizeof(wchar_t)*(s_MAX_BUFFER_SIZE+1));

			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vswprintf_s(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#else
				vswprintf(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		}

		// Function responsible of adding a new entry of the given type and with the given message to the log.
		void LogManagerA::f_log_entry( const LogEntryType& type , const std::string& message )
		{
			LogEntryA* entry = new (std::nothrow) LogEntryA(type,message,f_generate_timestamp());


			if ( entry != NULL )
			{
				// Insert the new entry to the log.
				m_log.push_back(entry);
				// Increase the auto dump counter.
				++m_auto_dump_count;

				// If echoing is enabled.
				if ( m_echo )
					f_post_entry(m_echo_stream,(*entry)); // Output the new entry to the echo stream.

				// Manage the size of the log and perform the auto-dump and auto-purge functionality if needed.
				f_manage_log_size();
			}
		}


		// Function responsible of initialising the instance of the class. Returns true on success.
		bool LogManagerA::initialise()
		{
			bool return_value = false;


			s_instance_lock.lock();

			if ( s_instance == INVALID_POINTER )
			{
				s_instance  = new (std::nothrow) LogManagerA();
				return_value = ( s_instance != INVALID_POINTER );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// Function responsible of deinitialising the instance of the class.
		void LogManagerA::deinitialise()
		{
			s_instance_lock.lock();

			if ( s_instance != INVALID_POINTER )
			{
				delete s_instance;
				s_instance = INVALID_POINTER;
			}

			s_instance_lock.unlock();
		}

		// Function returning a pointer to the single instance of the class.
		LogManagerA* LogManagerA::get()
		{
			LogManagerA* return_value = INVALID_POINTER;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}

		// Function responsible of setting the maximum size of the log.
		void LogManagerA::max_log_size( const unsigned int size )
		{
			m_lock.lock();
			m_max_log_size = size;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto purge functionality.
		void LogManagerA::auto_purge( const bool value )
		{
			m_lock.lock();
			m_auto_purge = value;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold of the auto-purge functionality.
		void LogManagerA::auto_purge_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_purge_threshold = threshold;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the name of the file that is used for the auto dump filestream.
		void LogManagerA::auto_dump_filename( const std::string& filename )
		{
			m_lock.lock();
			m_auto_dump_filename = filename;
			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the open mode for the auto dump filestream.
		void LogManagerA::auto_dump_file_open_mode( const LogFileOpenMode mode )
		{
			m_lock.lock();

			if ( mode == Truncate )
				m_auto_dump_file_open_mode = m_auto_dump_filestream.trunc;
			else
				m_auto_dump_file_open_mode = m_auto_dump_filestream.app;

			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold for the auto dump functionality.
		void LogManagerA::auto_dump_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_dump_threshold = threshold;
			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto dump functionality.
		void LogManagerA::auto_dump( const bool value )
		{
			m_lock.lock();
			m_auto_dump = value;
			f_manage_dump_filestream();
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the stream that is used for the echo functionality.
		void LogManagerA::echo_stream( std::ostream* stream )
		{
			m_lock.lock();

			if ( !m_echo  ||  stream != INVALID_POINTER )
				m_echo_stream = stream;

			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the echoing the contents of the log.
		void LogManagerA::echo( const bool value )
		{
			m_lock.lock();

			if ( !( m_echo_stream == INVALID_POINTER  &&  value ) )
				m_echo = value;

			m_lock.unlock();
		}

		// Function responsible of setting the error tag.
		void LogManagerA::error_tag( const std::string& tag )
		{
			m_lock.lock();
			m_error_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the warning tag.
		void LogManagerA::warning_tag( const std::string& tag )
		{
			m_lock.lock();
			m_warning_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the message tag.
		void LogManagerA::message_tag( const std::string& tag )
		{
			m_lock.lock();
			m_message_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the type separator.
		void LogManagerA::type_separator( const std::string& separator )
		{
			m_lock.lock();
			m_type_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the message separator.
		void LogManagerA::message_separator( const std::string& separator )
		{
			m_lock.lock();
			m_message_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the timestamp separator.
		void LogManagerA::timestamp_separator( const std::string& separator )
		{
			m_lock.lock();
			f_update_timestamps(separator);
			m_lock.unlock();
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerA::log_error( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Error,f_parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerA::log_error( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Error,utility::wide_string_to_string(f_parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerA::log_warning( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Warning,f_parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerA::log_warning( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Warning,utility::wide_string_to_string(f_parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManagerA::log_message( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Message,f_parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManagerA::log_message( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Message,utility::wide_string_to_string(f_parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of dumping the log the file with the given filename.
		void LogManagerA::dump_log( const std::string& filename , const LogFileOpenMode mode )
		{
			std::ofstream stream(filename,( mode == Append  ?  std::ofstream::ate : std::ofstream::trunc ));


			if ( stream.is_open() )
				f_dump_log(&stream,0);
		}

		// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
		void LogManagerA::purge_log()
		{
			m_lock.lock();
			f_manage_log_size();
			f_cleanup();
			m_auto_dump_count = 0;
			m_auto_dump_offset = 0;
			m_lock.unlock();
		}

		// Function returning the maximum size of the log.
		unsigned int LogManagerA::max_log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_max_log_size;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto purge functionality is enabled.
		bool LogManagerA::auto_purge() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_purge;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto purge functionality.
		unsigned int LogManagerA::auto_purge_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_purge_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the name of the file that is used in the auto dump functionality.
		std::string LogManagerA::auto_dump_filename() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_auto_dump_filename;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the open mode of the auto dump file.
		LogFileOpenMode LogManagerA::auto_dump_file_open_mode() const
		{
			LogFileOpenMode return_value = Append;


			m_lock.lock();

			if ( m_auto_dump_file_open_mode == m_auto_dump_filestream.trunc )
				return_value = Truncate;

			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto dump functionality.
		unsigned int LogManagerA::auto_dump_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_dump_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto dump functionality is enabled.
		bool LogManagerA::auto_dump() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_dump;
			m_lock.unlock();


			return return_value;
		}

		// Function returning a pointer to the stream that is used for echo operations.
		std::ostream* LogManagerA::echo_stream() const
		{
			std::ostream* return_value = INVALID_POINTER;


			m_lock.lock();
			return_value = m_echo_stream;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether echoing is enabled.
		bool LogManagerA::echo() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_echo;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the error tag.
		std::string LogManagerA::error_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_error_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the warning tag.
		std::string LogManagerA::warning_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_warning_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message tag.
		std::string LogManagerA::message_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_message_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the type separator.
		std::string LogManagerA::type_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_type_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message separator.
		std::string LogManagerA::message_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_message_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the timestamp separator.
		std::string LogManagerA::timestamp_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_timestamp_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the size of the log.
		unsigned int LogManagerA::log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_log.size();
			m_lock.unlock();


			return return_value;
		}

		// Function returning the entry at the given index. Returns true on success.
		bool LogManagerA::entry( const unsigned int index , LogEntryA& value ) const
		{
			bool return_value = false;


			m_lock.lock();

			// If the index is valid.
			if ( index < m_log.size() )
			{
				value = *(m_log[index]); // Return the entry at the desired index.
				return_value = true;
			}

			m_lock.unlock();


			return return_value;
		}

		// Function returning the desired number of entries from the given start. Returns true on success.
		bool LogManagerA::entries( const unsigned int number, const unsigned int start , std::deque<LogEntryA>& values ) const
		{
			bool return_value = false;


			m_lock.lock();

			if ( start < m_log.size() )
			{
				std::deque<LogEntryA*>::const_iterator log_iterator(m_log.begin() + start);
				unsigned int i = 0;


				values.clear();

				while ( i < number  &&  log_iterator != m_log.end() )
				{
					values.push_back((*(*log_iterator)));
					++i;
					++log_iterator;
				}
				
				return_value = true;
			}
			
			m_lock.unlock();


			return return_value;
		}



		/*
			UTF Log Manager definitions.
		*/


		// The single instance of the class.
		LogManagerW* LogManagerW::s_instance = NULL;
		// The loc that is used to handle concurrency issues regarding the instance of the class.
		std::mutex LogManagerW::s_instance_lock;
		// The maximum possible size for the buffer that is used in the functions of the class.
		const unsigned int LogManagerW::s_MAX_BUFFER_SIZE = 1024;


		// The constructor of the class.
		LogManagerW::LogManagerW() :
			m_log(0),
			m_lock(),
			m_auto_dump_filestream(),
			m_auto_dump_filename("App.log"),
			m_error_tag(L"Error:"),
			m_warning_tag(L"Warning:"),
			m_message_tag(L"Message:"),
			m_type_separator(L" "),
			m_message_separator(L" "),
			m_timestamp_separator(L" "),
			m_auto_dump_file_open_mode(std::wofstream::app),
			m_echo_stream(&std::wcout),
			m_max_log_size(10000),
			m_auto_purge_threshold(10000),
			m_auto_dump_threshold(10000),
			m_auto_dump_count(0),
			m_auto_dump_offset(0),
			m_auto_purge(false),
			m_auto_dump(false),
			m_echo(false)
		{
		}

		// The destructor of the class.
		LogManagerW::~LogManagerW()
		{
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close();

			f_cleanup();
		}


		// Function responsible of generating a string with the current timestamp.
		inline std::wstring LogManagerW::f_generate_timestamp()
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
			return_value_buffer << (date.tm_year+1900) << m_timestamp_separator;

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
		}

		// Function responsible of updating the timestamps of the log with the new separator.
		void LogManagerW::f_update_timestamps( const std::wstring& new_separator )
		{
			// For every entry in the log
			for ( std::deque<LogEntryW*>::iterator log_iterator = m_log.begin();  log_iterator != m_log.begin();  ++log_iterator )
			{
				// Get the timestamp of the entry.
				std::wstring timestamp((*log_iterator)->timestamp());
				size_t pos = 0;


				// While the current timestamp separator can be found in the timestamp.
				while ( pos != timestamp.npos )
				{
					// Find the first occurrence of the current timestamp.
					pos = timestamp.find_first_of(m_timestamp_separator,pos);

					// If the current timestamp was found.
					if ( pos != timestamp.npos )
					{
						// Replace the current timestamp with the new one.
						timestamp.replace(pos,m_timestamp_separator.size(),new_separator);
						// Update the search counter.
						pos += new_separator.size();
					}
				}

				// Set the timestamp of the entry to the updated timestamp.
				(*log_iterator)->timestamp(timestamp);
			}
		}

		// Function posting an entry to the given stream.
		void LogManagerW::f_post_entry( std::wostream* stream , const LogEntryW& entry )
		{
			// Output to the stream the timestamp of the entry and the type separator.
			(*stream) << entry.timestamp() << m_type_separator;

			// Depending on the type of the entry output the proper tag.
			switch ( entry.type() )
			{
				case Error:

					(*stream) << m_error_tag;
					break;

				case Warning:

					(*stream) << m_warning_tag;
					break;

				case Message:

					(*stream) << m_message_tag;
					break;
			}

			// Output the message separator and the message of the entry.
			(*stream) << m_message_separator << entry.message() << std::endl;
		}

		// Function dumping the contents from the log to the given stream starting from offset.
		bool LogManagerW::f_dump_log( std::wostream* stream , const unsigned int offset )
		{
			bool return_value = false;


			// If the output stream is in a good condition.
			if ( stream->good() )
			{
				// Output the log to the stream, starting from the given offset.
				for ( std::deque<LogEntryW*>::iterator log_iterator = m_log.begin()+offset;  log_iterator != m_log.end();  ++log_iterator )
					f_post_entry(stream,(*(*log_iterator)));

				// If the stream is in a good condition at the end of the output.
				if ( stream->good() )
					return_value = true;	// The log was dumped successfully.
			}


			return return_value;
		}

		// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
		void LogManagerW::f_manage_log_size()
		{
			// If the auto dump functionality is enabled
			if ( m_auto_dump )
			{
				/*
					If the auto dump threshold has been reached or
					if the auto purge functionality is enabled and the size of the log has reached the 
					auto purge threshold.
				*/
				if ( m_auto_dump_count > m_auto_dump_threshold  || ( m_auto_purge  &&  m_log.size() > m_auto_purge_threshold ) )
				{
					// Dump the log to the auto dump filestream.
					if ( f_dump_log(&m_auto_dump_filestream,m_auto_dump_offset) )
					{
						// Reset the auto dump counter.
						m_auto_dump_count = 0;
						// Set the auto dump offset to the size of the log since the log has been dumped.
						m_auto_dump_offset = m_log.size();
					}
				}
			}

			// If the auto purge functionality is enabled.
			if ( m_auto_purge )
			{
				// If the size of the log is greater than the auto purge threshold.
				if ( m_log.size() > m_auto_purge_threshold )
				{
					// Reset the auto dump count.
					m_auto_dump_count = 0;
					// Reset the auto dump offset.
					m_auto_dump_offset = 0;
					// Clear the log.
					f_cleanup();
				}
			}

			// If the size of the log is greater than the maximum log size.
			if ( m_log.size() > m_max_log_size )
			{
				delete m_log[0];
				m_log.pop_front();	// Remove the first entry to the log in order to keep the size constant.
			}
		}

		// Function responsible of performing cleanup of the allocated log entries.
		void LogManagerW::f_cleanup()
		{
			for ( std::deque<LogEntryW*>::iterator log_iterator = m_log.begin();  log_iterator != m_log.end();  ++log_iterator )
			{
				delete (*log_iterator);
			}

			m_log.clear();
		}

		// Function responsible of handling the auto-dump filestream.
		void LogManagerW::f_manage_dump_filestream()
		{
			// If the auto dump file is open.
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close(); // Close the auto dump file.

			// Open the auto dump file.
			m_auto_dump_filestream.open(m_auto_dump_filename,m_auto_dump_filestream.in|m_auto_dump_file_open_mode);
		}

		// Function responsible of parsing the parameters that were given to the log functions.
		std::string LogManagerW::f_parse_parameters( const char* input , va_list parameters )
		{
			char buffer[s_MAX_BUFFER_SIZE+1];

			// Set the buffer to 0.
			memset(buffer,'\0',sizeof(char)*(s_MAX_BUFFER_SIZE+1));

			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vsnprintf_s(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#else
				vsnprintf(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		}

		// Function responsible of parsing the parameters that were given to the log functions.
		std::wstring LogManagerW::f_parse_wide_parameters( const wchar_t* input , va_list parameters )
		{
			wchar_t buffer[s_MAX_BUFFER_SIZE+1];

			// Set the buffer to 0.
			memset(buffer,L'\0',sizeof(wchar_t)*(s_MAX_BUFFER_SIZE+1));

			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vswprintf_s(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#else
				vswprintf(buffer,s_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		}

		// Function responsible of adding a new entry of the given type and with the given message to the log.
		void LogManagerW::f_log_entry( const LogEntryType& type , const std::wstring& message )
		{
			LogEntryW* entry = new (std::nothrow) LogEntryW(type,message,f_generate_timestamp());


			if ( entry != NULL )
			{
				// Insert the new entry to the log.
				m_log.push_back(entry);
				// Increase the auto dump counter.
				++m_auto_dump_count;

				// If echoing is enabled.
				if ( m_echo )
					f_post_entry(m_echo_stream,(*entry)); // Output the new entry to the echo stream.

				// Manage the size of the log and perform the auto-dump and auto-purge functionality if needed.
				f_manage_log_size();
			}
		}


		// Function responsible of initialising the instance of the class. Returns true on success.
		bool LogManagerW::initialise()
		{
			bool return_value = false;


			s_instance_lock.lock();

			if ( s_instance == INVALID_POINTER )
			{
				s_instance  = new (std::nothrow) LogManagerW();
				return_value = ( s_instance != INVALID_POINTER );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// Function responsible of deinitialising the instance of the class.
		void LogManagerW::deinitialise()
		{
			s_instance_lock.lock();

			if ( s_instance != INVALID_POINTER )
			{
				delete s_instance;
				s_instance = INVALID_POINTER;
			}

			s_instance_lock.unlock();
		}

		// Function returning a pointer to the single instance of the class.
		LogManagerW* LogManagerW::get()
		{
			LogManagerW* return_value = INVALID_POINTER;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}

		// Function responsible of setting the maximum size of the log.
		void LogManagerW::max_log_size( const unsigned int size )
		{
			m_lock.lock();
			m_max_log_size = size;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto purge functionality.
		void LogManagerW::auto_purge( const bool value )
		{
			m_lock.lock();
			m_auto_purge = value;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold of the auto-purge functionality.
		void LogManagerW::auto_purge_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_purge_threshold = threshold;
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the name of the file that is used for the auto dump filestream.
		void LogManagerW::auto_dump_filename( const std::string& filename )
		{
			m_lock.lock();
			m_auto_dump_filename = filename;
			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the open mode for the auto dump filestream.
		void LogManagerW::auto_dump_file_open_mode( const LogFileOpenMode mode )
		{
			m_lock.lock();

			if ( mode == Truncate )
				m_auto_dump_file_open_mode = m_auto_dump_filestream.trunc;
			else
				m_auto_dump_file_open_mode = m_auto_dump_filestream.app;

			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold for the auto dump functionality.
		void LogManagerW::auto_dump_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_dump_threshold = threshold;
			f_manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto dump functionality.
		void LogManagerW::auto_dump( const bool value )
		{
			m_lock.lock();
			m_auto_dump = value;
			f_manage_dump_filestream();
			f_manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the stream that is used for the echo functionality.
		void LogManagerW::echo_stream( std::wostream* stream )
		{
			m_lock.lock();

			if ( !m_echo  ||  stream != INVALID_POINTER )
				m_echo_stream = stream;

			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the echoing the contents of the log.
		void LogManagerW::echo( const bool value )
		{
			m_lock.lock();

			if ( !( m_echo_stream == INVALID_POINTER  &&  value ) )
				m_echo = value;

			m_lock.unlock();
		}

		// Function responsible of setting the error tag.
		void LogManagerW::error_tag( const std::wstring& tag )
		{
			m_lock.lock();
			m_error_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the warning tag.
		void LogManagerW::warning_tag( const std::wstring& tag )
		{
			m_lock.lock();
			m_warning_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the message tag.
		void LogManagerW::message_tag( const std::wstring& tag )
		{
			m_lock.lock();
			m_message_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the type separator.
		void LogManagerW::type_separator( const std::wstring& separator )
		{
			m_lock.lock();
			m_type_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the message separator.
		void LogManagerW::message_separator( const std::wstring& separator )
		{
			m_lock.lock();
			m_message_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the timestamp separator.
		void LogManagerW::timestamp_separator( const std::wstring& separator )
		{
			m_lock.lock();
			f_update_timestamps(separator);
			m_lock.unlock();
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::log_error( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Error,utility::string_to_wide_string(f_parse_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::log_error( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Error,f_parse_wide_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::log_warning( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Warning,utility::string_to_wide_string(f_parse_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		inline void LogManagerW::log_warning( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Warning,f_parse_wide_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManagerW::log_message( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Message,utility::string_to_wide_string(f_parse_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManagerW::log_message( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			f_log_entry(Message,f_parse_wide_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of dumping the log the file with the given filename.
		void LogManagerW::dump_log( const std::string& filename , const LogFileOpenMode mode )
		{
			std::wofstream stream(filename,( mode == Append  ?  std::wofstream::ate : std::wofstream::trunc ));


			if ( stream.is_open() )
				f_dump_log(&stream,0);
		}

		// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
		void LogManagerW::purge_log()
		{
			m_lock.lock();
			f_manage_log_size();
			f_cleanup();
			m_auto_dump_count = 0;
			m_auto_dump_offset = 0;
			m_lock.unlock();
		}

		// Function returning the maximum size of the log.
		unsigned int LogManagerW::max_log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_max_log_size;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto purge functionality is enabled.
		bool LogManagerW::auto_purge() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_purge;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto purge functionality.
		unsigned int LogManagerW::auto_purge_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_purge_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the name of the file that is used in the auto dump functionality.
		std::string LogManagerW::auto_dump_filename() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_auto_dump_filename;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the open mode of the auto dump file.
		LogFileOpenMode LogManagerW::auto_dump_file_open_mode() const
		{
			LogFileOpenMode return_value = Append;


			m_lock.lock();

			if ( m_auto_dump_file_open_mode == m_auto_dump_filestream.trunc )
				return_value = Truncate;

			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto dump functionality.
		unsigned int LogManagerW::auto_dump_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_dump_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto dump functionality is enabled.
		bool LogManagerW::auto_dump() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_dump;
			m_lock.unlock();


			return return_value;
		}

		// Function returning a pointer to the stream that is used for echo operations.
		std::wostream* LogManagerW::echo_stream() const
		{
			std::wostream* return_value = INVALID_POINTER;


			m_lock.lock();
			return_value = m_echo_stream;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether echoing is enabled.
		bool LogManagerW::echo() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_echo;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the error tag.
		std::wstring LogManagerW::error_tag() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_error_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the warning tag.
		std::wstring LogManagerW::warning_tag() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_warning_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message tag.
		std::wstring LogManagerW::message_tag() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_message_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the type separator.
		std::wstring LogManagerW::type_separator() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_type_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message separator.
		std::wstring LogManagerW::message_separator() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_message_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the timestamp separator.
		std::wstring LogManagerW::timestamp_separator() const
		{
			std::wstring return_value(L"");


			m_lock.lock();
			return_value = m_timestamp_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the size of the log.
		unsigned int LogManagerW::log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_log.size();
			m_lock.unlock();


			return return_value;
		}

		// Function returning the entry at the given index. Returns true on success.
		bool LogManagerW::entry( const unsigned int index , LogEntryW& value ) const
		{
			bool return_value = false;


			m_lock.lock();

			// If the index is valid.
			if ( index < m_log.size() )
			{
				value = *(m_log[index]); // Return the entry at the desired index.
				return_value = true;
			}

			m_lock.unlock();


			return return_value;
		}

		// Function returning the desired number of entries from the given start. Returns true on success.
		bool LogManagerW::entries( const unsigned int number, const unsigned int start , std::deque<LogEntryW>& values ) const
		{
			bool return_value = false;


			m_lock.lock();

			if ( start < m_log.size() )
			{
				std::deque<LogEntryW*>::const_iterator log_iterator(m_log.begin() + start);
				unsigned int i = 0;


				values.clear();

				while ( i < number  &&  log_iterator != m_log.end() )
				{
					values.push_back((*(*log_iterator)));
					++i;
					++log_iterator;
				}
				
				return_value = true;
			}
			
			m_lock.unlock();


			return return_value;
		}

	} /* io */

} /* athena */
