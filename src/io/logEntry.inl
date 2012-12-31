#ifndef ATHENA_IO_LOGENTRY_INL
#define ATHENA_IO_LOGENTRY_INL

#ifndef ATHENA_IO_LOGENTRY_HPP
	#error "logEntry.hpp must be included before logEntry.inl"
#endif /* ATHENA_IO_LOGENTRY_HPP */



namespace athena
{
		
	namespace io
	{
			
		// Function responsible of setting the timestamp of the entry.
		inline void LogEntryA::timestamp( const std::string& timestamp )
		{
			_timestamp = timestamp;
		}

		// Function responsible of setting the message of the entry.
		inline void LogEntryA::message( const std::string& message )
		{
			_message = message;
		}

		// Function responsible of setting the type of the entry.
		inline void LogEntryA::type( const LogEntryType& type )
		{
			_type = type;
		}
				
		// Function returning the timestamp of the entry.
		inline std::string LogEntryA::timestamp() const
		{
			return _timestamp;
		}

		// Function returning the message of the entry.
		inline std::string LogEntryA::message() const
		{
			return _message;
		}

		// Function returning the type of the entry.
		inline LogEntryType LogEntryA::type() const
		{
			return _type;
		}


		// Function responsible of setting the timestamp of the entry.
		inline void LogEntryW::timestamp( const std::wstring& timestamp )
		{
			_timestamp = timestamp;
		}

		// Function responsible of setting the message of the entry.
		inline void LogEntryW::message( const std::wstring& message )
		{
			_message = message;
		}

		// Function responsible of setting the type of the entry.
		inline void LogEntryW::type( const LogEntryType& type )
		{
			_type = type;
		}
			
		// Function returning the timestamp of the entry.
		inline std::wstring LogEntryW::timestamp() const
		{
			return _timestamp;
		}

		// Function returning the message of the entry.
		inline std::wstring LogEntryW::message() const
		{
			return _message;
		}

		// Function returning the type of the entry.
		inline LogEntryType LogEntryW::type() const
		{
			return _type;
		}

	} /* io */
		
} /* athena */	



#endif /* ATHENA_IO_LOGENTRY_INL */