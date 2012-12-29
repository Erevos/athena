#ifndef ATHENA_IO_LOGENTRY_HPP
#define ATHENA_IO_LOGENTRY_HPP

#include "../athenaDefinitions.hpp"
#include <string>



namespace athena
{

	namespace io
	{

		/*
			The various types of entries.
		*/
		enum ATHENA_DLL LogEntryType
		{
			Error = 0 , 
			Warning , 
			Message
		};


		/*
			A class representing and handling a ASCII log entry.
		*/
		class LogEntryA
		{
			private:

				// A string holding the timestamp of the entry.
				std::string _timestamp;
				// A string holding the message of the entry.
				std::string _message;
				// A variable holding the type of the entry.
				LogEntryType _type;


			public:

				// The constructor of the class.
				explicit ATHENA_DLL LogEntryA( const LogEntryType& type = Message , const std::string& message = "" , const std::string& timestamp = "" );
				// The destructor of the class.
				ATHENA_DLL virtual ~LogEntryA();


				// Function responsible of setting the timestamp of the entry.
				ATHENA_DLL void timestamp( const std::string& timestamp );
				// Function responsible of setting the message of the entry.
				ATHENA_DLL void message( const std::string& message );
				// Function responsible of setting the type of the entry.
				ATHENA_DLL void type( const LogEntryType& type );
					

				// Function returning the timestamp of the entry.
				ATHENA_DLL std::string timestamp() const;
				// Function returning the message of the entry.
				ATHENA_DLL std::string message() const;
				// Function returning the type of the entry.
				ATHENA_DLL LogEntryType type() const;
		};


		/*
			A class representing and handling a UTF log entry.
		*/
		class LogEntryW
		{
			private:

				// A string holding the timestamp of the entry.
				std::wstring _timestamp;
				// A string holding the message of the entry.
				std::wstring _message;
				// A variable holding the type of the entry.
				LogEntryType _type;


			public:

				// The constructor of the class.
				explicit ATHENA_DLL LogEntryW( const LogEntryType& type = Message , const std::wstring& message = L"" , const std::wstring& timestamp = L"" );
				// The destructor of the class.
				ATHENA_DLL virtual ~LogEntryW();


				// Function responsible of setting the timestamp of the entry.
				ATHENA_DLL void timestamp( const std::wstring& timestamp );
				// Function responsible of setting the message of the entry.
				ATHENA_DLL void message( const std::wstring& message );
				// Function responsible of setting the type of the entry.
				ATHENA_DLL void type( const LogEntryType& type );
					

				// Function returning the timestamp of the entry.
				ATHENA_DLL std::wstring timestamp() const;
				// Function returning the message of the entry.
				ATHENA_DLL std::wstring message() const;
				// Function returning the type of the entry.
				ATHENA_DLL LogEntryType type() const;
		};

	} /* io */

} /* athena */


#include "logEntry.inl"



#endif /* ATHENA_IO_LOGENTRY_HPP */
