#ifndef ATHENA_UTILITY_THREAD_HPP
#define ATHENA_UTILITY_THREAD_HPP

#include "../athenaDefinitions.hpp"
#include "readersWriterLock.hpp"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <pthread.h>
#endif /* _WIN32 */



namespace athena
{

	namespace utility
	{
			
		// A type definition that defines the return type that the thread functions use.
		#ifdef _WIN32
			typedef DWORD ThreadExitType;
		#else
			typedef int ThreadExitType;
		#endif /* _WIN32 */

		// Function that is used to determine the functionality of the thread.
		typedef ThreadExitType (*ThreadFunction)( void* );


		/*
			A class representing and handling a thread.
		*/
		class Thread
		{
			private:

				// Readers-Writer lock used to handle concurrency issues.
				mutable ReadersWriterLock _lock;


				// THe handle of the thread.
				#ifdef _WIN32
					HANDLE _id;
				#else
					pthread_t _id;
				#endif /* _WIN32 */

				// The exit code of the thread.
				ThreadExitType _exit_code;
				// A function pointer pointer to the thread functionality.
				ThreadFunction _function;
				// A pointer to the parameter of the thread.
				void* _parameter;
				// Variable holding the running status of the thread.
				bool _running;


				// The static function responsible of calling the functionality function.
				#ifdef _WIN32
					static ThreadExitType _stdcall _thread_function( void* );
				#else
					static ThreadExitType _thread_function( void* ) __attribute__(stdcall);
				#endif /* _WIN32 */


			public:

				// The constructor of the class.
				ATHENA_DLL Thread( ThreadFunction function = INVALID_POINTER , void* parameter = INVALID_POINTER  );
				// The destructor of the class.
				ATHENA_DLL ~Thread();


				// Function setting the functionality of the thread. Must be called before the create() function.
				ATHENA_DLL void functionality( ThreadFunction function );
				// Function setting the parameter of the thread. Must be called before the create() function.
				ATHENA_DLL void parameter( void* value );


				// Function returning the running status of the thread.
				ATHENA_DLL bool is_running() const;
				// Function returning the functionality function of the thread.
				ATHENA_DLL ThreadFunction functionality();
				// Function returning the parameter of the thread.
				ATHENA_DLL void* parameter();
				// Function returning the exit code of the thread. Should be called after destroy().
				ATHENA_DLL ThreadExitType exit_code() const;


				// Function creating the thread. Returns true on success, false on failure.
				ATHENA_DLL bool create();
				// Function destroying the thread.
				ATHENA_DLL void destroy();
		};

	} /* utility */

} /* athena */



#endif /* ATHENA_UTILITY_THREAD_HPP */