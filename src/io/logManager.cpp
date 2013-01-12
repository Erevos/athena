#include "logManager.hpp"
#include <cstring>
#include <iostream>
#include <cwchar>
#include "../athena.hpp"
#include "../eventCodes.hpp"



namespace athena
{

	namespace io
	{

		/*
			ASCII Log Manager definitions.
		*/


		// The single instance of the class.
		LogManager* LogManager::s_instance = NULL;
		// The loc that is used to handle concurrency issues regarding the instance of the class.
		std::mutex LogManager::s_instance_lock;
		// The maximum possible size for the buffer that is used in the functions of the class.
		const unsigned int LogManager::s_MAX_BUFFER_SIZE = 1024;


		// The constructor of the class.
		LogManager::LogManager() :
			Listener(athena::LogManagerID) ,
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
		LogManager::~LogManager()
		{
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close();

			cleanup();
		}


		// Function responsible of generating a string with the current timestamp.
		std::string LogManager::generate_timestamp()
		{
			// The current time.
			time_t now = time(NULL);
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
		void LogManager::update_timestamps( const std::string& new_separator )
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
		void LogManager::post_entry( std::ostream* stream , const LogEntryA& entry )
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
		bool LogManager::dump_log( std::ostream* stream , const unsigned int offset )
		{
			bool return_value = false;


			// If the output stream is in a good condition.
			if ( stream->good() )
			{
				// Output the log to the stream, starting from the given offset.
				for ( std::deque<LogEntryA*>::iterator log_iterator = m_log.begin()+offset;  log_iterator != m_log.end();  ++log_iterator )
					post_entry(stream,(*(*log_iterator)));

				// If the stream is in a good condition at the end of the output.
				if ( stream->good() )
					return_value = true;	// The log was dumped successfully.
			}


			return return_value;
		}

		// Function responsible of managing the size of the log and performing the auto-dump and auto-purge functionalities.
		void LogManager::manage_log_size()
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
					if ( dump_log(&m_auto_dump_filestream,m_auto_dump_offset) )
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
					cleanup();
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
		void LogManager::cleanup()
		{
			for ( std::deque<LogEntryA*>::iterator log_iterator = m_log.begin();  log_iterator != m_log.end();  ++log_iterator )
			{
				delete (*log_iterator);
			}

			m_log.clear();
		}

		// Function responsible of handling the auto-dump filestream.
		void LogManager::manage_dump_filestream()
		{
			// If the auto dump file is open.
			if ( m_auto_dump_filestream.is_open() )
				m_auto_dump_filestream.close(); // Close the auto dump file.

			// Open the auto dump file.
			m_auto_dump_filestream.open(m_auto_dump_filename,m_auto_dump_filestream.in|m_auto_dump_file_open_mode);
		}

		// Function responsible of parsing the parameters that were given to the log functions.
		std::string LogManager::parse_parameters( const char* input , va_list parameters )
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
		std::wstring LogManager::parse_wide_parameters( const wchar_t* input , va_list parameters )
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
		void LogManager::log_entry( const LogEntryType& type , const std::string& message )
		{
			LogEntryA* entry = new (std::nothrow) LogEntryA(type,message,generate_timestamp());


			if ( entry != NULL )
			{
				// Create the event that will be fired when a new entry is inserted.
				core::Event event(EVENT_LOG_NEW_ENTRY);


				// Insert the new entry to the log.
				m_log.push_back(entry);
				// Increase the auto dump counter.
				++m_auto_dump_count;

				// If echoing is enabled.
				if ( m_echo )
					post_entry(m_echo_stream,(*entry)); // Output the new entry to the echo stream.

				// Manage the size of the log and perform the auto-dump and auto-purge functionality if needed.
				manage_log_size();

				// Set the first parameter of the event to the new entry.
				event.parameter(0,core::ParameterType::Pointer,static_cast<void*>(entry));
				// Trigger the event.
				athena::trigger_event(event);
			}
		}


		// Function responsible of performing any actions needed on startup.
		bool LogManager::startup()
		{
			return true;
		}

		// Function responsible of perfoming any actions needed on termination.
		void LogManager::terminate()
		{
			unregister_all_events();
		}


		// Function responsible of initialising the instance of the class. Returns true on success.
		bool LogManager::init()
		{
			bool return_value = false;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance  = new (std::nothrow) LogManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// Function responsible of deinitialising the instance of the class.
		void LogManager::deinit()
		{
			s_instance_lock.lock();

			if ( s_instance != NULL )
			{
				delete s_instance;
				s_instance = NULL;
			}

			s_instance_lock.unlock();
		}

		// Function returning a pointer to the single instance of the class.
		LogManager* LogManager::get()
		{
			LogManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// Function responsible of setting the maximum size of the log.
		void LogManager::max_log_size( const unsigned int size )
		{
			m_lock.lock();
			m_max_log_size = size;
			manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto purge functionality.
		void LogManager::auto_purge( const bool value )
		{
			m_lock.lock();
			m_auto_purge = value;
			manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold of the auto-purge functionality.
		void LogManager::auto_purge_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_purge_threshold = threshold;
			manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the name of the file that is used for the auto dump filestream.
		void LogManager::auto_dump_filename( const std::string& filename )
		{
			m_lock.lock();
			m_auto_dump_filename = filename;
			manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the open mode for the auto dump filestream.
		void LogManager::auto_dump_file_open_mode( const LogFileOpenMode mode )
		{
			m_lock.lock();

			if ( mode == Truncate )
				m_auto_dump_file_open_mode = m_auto_dump_filestream.trunc;
			else
				m_auto_dump_file_open_mode = m_auto_dump_filestream.app;

			manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of setting the threshold for the auto dump functionality.
		void LogManager::auto_dump_threshold( const unsigned int threshold )
		{
			m_lock.lock();
			m_auto_dump_threshold = threshold;
			manage_dump_filestream();
			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the auto dump functionality.
		void LogManager::auto_dump( const bool value )
		{
			m_lock.lock();
			m_auto_dump = value;
			manage_dump_filestream();
			manage_log_size();
			m_lock.unlock();
		}

		// Function responsible of setting the stream that is used for the echo functionality.
		void LogManager::echo_stream( std::ostream* stream )
		{
			m_lock.lock();

			if ( !m_echo  ||  stream != NULL )
				m_echo_stream = stream;

			m_lock.unlock();
		}

		// Function responsible of enabling or disabling the echoing the contents of the log.
		void LogManager::echo( const bool value )
		{
			m_lock.lock();

			if ( !( m_echo_stream == NULL  &&  value ) )
				m_echo = value;

			m_lock.unlock();
		}

		// Function responsible of setting the error tag.
		void LogManager::error_tag( const std::string& tag )
		{
			m_lock.lock();
			m_error_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the warning tag.
		void LogManager::warning_tag( const std::string& tag )
		{
			m_lock.lock();
			m_warning_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the message tag.
		void LogManager::message_tag( const std::string& tag )
		{
			m_lock.lock();
			m_message_tag = tag;
			m_lock.unlock();
		}

		// Function responsible of setting the type separator.
		void LogManager::type_separator( const std::string& separator )
		{
			m_lock.lock();
			m_type_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the message separator.
		void LogManager::message_separator( const std::string& separator )
		{
			m_lock.lock();
			m_message_separator = separator;
			m_lock.unlock();
		}

		// Function responsible of setting the timestamp separator.
		void LogManager::timestamp_separator( const std::string& separator )
		{
			m_lock.lock();
			update_timestamps(separator);
			m_lock.unlock();
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_error( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Error,parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging an error formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_error( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Error,utility::wide_string_to_string(parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_warning( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Warning,parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a warning formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_warning( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Warning,utility::wide_string_to_string(parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_message( const char* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Message,parse_parameters(format,arguments));
			va_end(arguments);
		}

		// Function responsible of logging a message formatted as the given string. The format has the same functionality as printf.
		void LogManager::log_message( const wchar_t* format , ... )
		{
			va_list arguments;


			va_start(arguments,format);
			log_entry(Message,utility::wide_string_to_string(parse_wide_parameters(format,arguments)));
			va_end(arguments);
		}

		// Function responsible of dumping the log the file with the given filename.
		void LogManager::dump_log( const std::string& filename , const LogFileOpenMode mode )
		{
			std::ofstream stream(filename,( mode == Append  ?  std::ofstream::ate : std::ofstream::trunc ));


			if ( stream.is_open() )
				dump_log(&stream,0);
		}

		// Function responsible of purging the log. If the auto dump mode is enabled, the contents of the log are dumped to the auto-dump file.
		void LogManager::purge_log()
		{
			m_lock.lock();
			manage_log_size();
			cleanup();
			m_auto_dump_count = 0;
			m_auto_dump_offset = 0;
			m_lock.unlock();
		}

		// Function returning the maximum size of the log.
		unsigned int LogManager::max_log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_max_log_size;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto purge functionality is enabled.
		bool LogManager::auto_purge() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_purge;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto purge functionality.
		unsigned int LogManager::auto_purge_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_purge_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the name of the file that is used in the auto dump functionality.
		std::string LogManager::auto_dump_filename() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_auto_dump_filename;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the open mode of the auto dump file.
		LogFileOpenMode LogManager::auto_dump_file_open_mode() const
		{
			LogFileOpenMode return_value = Append;


			m_lock.lock();

			if ( m_auto_dump_file_open_mode == m_auto_dump_filestream.trunc )
				return_value = Truncate;

			m_lock.unlock();


			return return_value;
		}

		// Function returning the threshold of the auto dump functionality.
		unsigned int LogManager::auto_dump_threshold() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_auto_dump_threshold;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether the auto dump functionality is enabled.
		bool LogManager::auto_dump() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_auto_dump;
			m_lock.unlock();


			return return_value;
		}

		// Function returning a pointer to the stream that is used for echo operations.
		std::ostream* LogManager::echo_stream() const
		{
			std::ostream* return_value = NULL;


			m_lock.lock();
			return_value = m_echo_stream;
			m_lock.unlock();


			return return_value;
		}

		// Function returning whether echoing is enabled.
		bool LogManager::echo() const
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_echo;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the error tag.
		std::string LogManager::error_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_error_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the warning tag.
		std::string LogManager::warning_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_warning_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message tag.
		std::string LogManager::message_tag() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_message_tag;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the type separator.
		std::string LogManager::type_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_type_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the message separator.
		std::string LogManager::message_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_message_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the timestamp separator.
		std::string LogManager::timestamp_separator() const
		{
			std::string return_value("");


			m_lock.lock();
			return_value = m_timestamp_separator;
			m_lock.unlock();


			return return_value;
		}

		// Function returning the size of the log.
		unsigned int LogManager::log_size() const
		{
			unsigned int return_value = 0;


			m_lock.lock();
			return_value = m_log.size();
			m_lock.unlock();


			return return_value;
		}

		// Function returning the entry at the given index. Returns true on success.
		bool LogManager::entry( const unsigned int index , LogEntryA& value ) const
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
		bool LogManager::entries( const unsigned int number, const unsigned int start , std::deque<LogEntryA>& values ) const
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


		// Function responsible of responding to a triggered event.
		void LogManager::on_event( const core::Event& )
		{
		}

	} /* io */

} /* athena */
