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
					explicit ATHENA_DLL ATHENA_PRECALL LogEntryA( const LogEntryType& type = Message , const std::string& message = "" , const std::string& timestamp = "" ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL virtual ATHENA_PRECALL ~LogEntryA() ATHENA_POSTCALL;


					// Function responsible of setting the timestamp of the entry.
					ATHENA_DLL void ATHENA_PRECALL timestamp( const std::string& timestamp ) ATHENA_POSTCALL;
					// Function responsible of setting the message of the entry.
					ATHENA_DLL void ATHENA_PRECALL message( const std::string& message ) ATHENA_POSTCALL;
					// Function responsible of setting the type of the entry.
					ATHENA_DLL void ATHENA_PRECALL type( const LogEntryType& type ) ATHENA_POSTCALL;
					

					// Function returning the timestamp of the entry.
					ATHENA_DLL std::string ATHENA_PRECALL timestamp() const ATHENA_POSTCALL;
					// Function returning the message of the entry.
					ATHENA_DLL std::string ATHENA_PRECALL message() const ATHENA_POSTCALL;
					// Function returning the type of the entry.
					ATHENA_DLL LogEntryType ATHENA_PRECALL type() const ATHENA_POSTCALL;
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
					explicit ATHENA_DLL ATHENA_PRECALL LogEntryW( const LogEntryType& type = Message , const std::wstring& message = L"" , const std::wstring& timestamp = L"" ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL virtual ATHENA_PRECALL ~LogEntryW() ATHENA_POSTCALL;


					// Function responsible of setting the timestamp of the entry.
					ATHENA_DLL void ATHENA_PRECALL timestamp( const std::wstring& timestamp ) ATHENA_POSTCALL;
					// Function responsible of setting the message of the entry.
					ATHENA_DLL void ATHENA_PRECALL message( const std::wstring& message ) ATHENA_POSTCALL;
					// Function responsible of setting the type of the entry.
					ATHENA_DLL void ATHENA_PRECALL type( const LogEntryType& type ) ATHENA_POSTCALL;
					

					// Function returning the timestamp of the entry.
					ATHENA_DLL std::wstring ATHENA_PRECALL timestamp() const ATHENA_POSTCALL;
					// Function returning the message of the entry.
					ATHENA_DLL std::wstring ATHENA_PRECALL message() const ATHENA_POSTCALL;
					// Function returning the type of the entry.
					ATHENA_DLL LogEntryType ATHENA_PRECALL type() const ATHENA_POSTCALL;
			};

		} /* io */

	} /* athena */


	#include "logEntry.inl"



#endif /* ATHENA_IO_LOGENTRY_HPP */
