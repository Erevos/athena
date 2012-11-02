#include "logManager.hpp"
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
		LogManagerA* LogManagerA::_instance = NULL;
		// The loc that is used to handle concurrency issues regarding the instance of the class.
		utility::ReadersWriterLock LogManagerA::_instance_lock;
		// The maximum possible size for the buffer that is used in the functions of the class.
		const unsigned int LogManagerA::_MAX_BUFFER_SIZE = 1024;


		// The constructor of the class.
		LogManagerA::LogManagerA() : 
			_log(0) , 
			_lock() , 
			_auto_dump_filestream() , 
			_auto_dump_filename("App.log") , 
			_error_tag("Error:") , 
			_warning_tag("Warning:") , 
			_message_tag("Message:") , 
			_echo_stream(&std::cout) , 
			_max_log_size(10000) , 
			_auto_purge_threshold(10000) , 
			_auto_dump_threshold(10000) , 
			_auto_dump_count(0) , 
			_auto_dump_offset(0) , 
			_auto_dump_file_open_mode(std::ofstream::app) , 
			_type_separator(" ") ,
			_message_separator(" ") ,
			_timestamp_separator(" ") , 
			_auto_purge(false) , 
			_auto_dump(false) , 
			_echo(false)
		{
		};

		// The destructor of the class.
		LogManagerA::~LogManagerA()
		{
			if ( _auto_dump_filestream.is_open() )
				_auto_dump_filestream.close();
		};


		// Function responsible of updating the timestamps of the log with the new separator.
		void LogManagerA::_update_timestamps( const std::string& new_separator )
		{
			// For every entry in the log
			for ( std::deque<LogEntryA>::iterator log_iterator = _log.begin();  log_iterator != _log.begin();  ++log_iterator )
			{
				// Get the timestamp of the entry.
				std::string timestamp(log_iterator->timestamp());
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

		// Function dumping the contents from the log to the given stream starting from offset.
		bool LogManagerA::_dump_log( std::ostream* stream , const unsigned int offset )
		{
			bool return_value = false;


			// If the output stream is in a good condition.
			if ( stream->good() )
			{
				// Output the log to the stream, starting from the given offset.
				for ( std::deque<LogEntryA>::iterator log_iterator = _log.begin()+offset;  log_iterator != _log.end();  ++log_iterator )
					_post_entry(stream,(*log_iterator));

				// If the stream is in a good condition at the end of the output.
				if ( stream->good() )
					return_value = true;	// The log was dumped successfully.
			}


			return return_value;
		};

		// Function responsible of parsing the parameters that were given to the log functions.
		std::string LogManagerA::_parse_parameters( const char* input , va_list parameters )
		{
			char buffer[_MAX_BUFFER_SIZE+1];
			

			// Set the buffer to 0.
			memset(buffer,'\0',_MAX_BUFFER_SIZE+1);
			
			// Output the formatted string to the buffer.
			#ifdef _WIN32
				vsnprintf_s(buffer,_MAX_BUFFER_SIZE,input,parameters);
			#else
				vsnprintf(buffer,_MAX_BUFFER_SIZE,input,parameters);
			#endif


			return buffer;
		};



		/*
			UTF Log Manager definitions.
		*/

		// The single instance of the class.
		LogManagerW* LogManagerW::_instance = NULL;
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

	} /* io */

} /* athena */