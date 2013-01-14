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
			m_timestamp = timestamp;
		}

		// Function responsible of setting the message of the entry.
		inline void LogEntryA::message( const std::string& message )
		{
			m_message = message;
		}

		// Function responsible of setting the type of the entry.
		inline void LogEntryA::type( const LogEntryType& type )
		{
			m_type = type;
		}
		
		// Function returning the timestamp of the entry.
		inline std::string LogEntryA::timestamp() const
		{
			return m_timestamp;
		}

		// Function returning the message of the entry.
		inline std::string LogEntryA::message() const
		{
			return m_message;
		}

		// Function returning the type of the entry.
		inline LogEntryType LogEntryA::type() const
		{
			return m_type;
		}


		// Function responsible of setting the timestamp of the entry.
		inline void LogEntryW::timestamp( const std::wstring& timestamp )
		{
			m_timestamp = timestamp;
		}

		// Function responsible of setting the message of the entry.
		inline void LogEntryW::message( const std::wstring& message )
		{
			m_message = message;
		}

		// Function responsible of setting the type of the entry.
		inline void LogEntryW::type( const LogEntryType& type )
		{
			m_type = type;
		}
		
		// Function returning the timestamp of the entry.
		inline std::wstring LogEntryW::timestamp() const
		{
			return m_timestamp;
		}

		// Function returning the message of the entry.
		inline std::wstring LogEntryW::message() const
		{
			return m_message;
		}

		// Function returning the type of the entry.
		inline LogEntryType LogEntryW::type() const
		{
			return m_type;
		}

	} /* io */
		
} /* athena */	



#endif /* ATHENA_IO_LOGENTRY_INL */