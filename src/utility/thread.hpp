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
			
			// Function that is used to determine the functionality of the thread.
			#ifdef _WIN32
				typedef DWORD (*threadFunction)( void* );
			#else
				typedef int (*threadFunction)( void* );
			#endif /* _WIN32 */

			// A class representing and handling a thread.
			class Thread
			{
				private:

					// Readers-Writer lock used to handle concurrency issues.
					mutable ReadersWriterLock _lock;

					#ifdef _WIN32
						
						// THe handle of the thread.
						HANDLE _id;
						// The exit code of the thread.
						DWORD _exit_code;

					#else
						
						// THe handle of the thread.
						pthread_t _id;
						// The exit code of the thread.
						int _exit_code;

					#endif /* _WIN32 */

					// A function pointer pointer to the thread functionality.
					threadFunction _function;
					// A pointer to the parameter of the thread.
					void* _parameter;
					// Variable holding the running status of the thread.
					bool _running;


					// The static function responsible of calling the functionality function.
					#ifdef _WIN32
						static DWORD _stdcall _thread_function( void* );
					#else
						static int _thread_function( void* );
					#endif /* _WIN32 */


				public:

					// The constructor of the class.
					ATHENA_DLL ATHENA_PRECALL Thread( threadFunction function = NULL , void* paramter = NULL  ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~Thread() ATHENA_POSTCALL;


					// Function setting the functionality of the thread. Must be called before the create() function.
					ATHENA_DLL void ATHENA_PRECALL functionality( threadFunction function ) ATHENA_POSTCALL;
					// Function setting the parameter of the thread. Must be called before the create() function.
					ATHENA_DLL void ATHENA_PRECALL parameter( void* value ) ATHENA_POSTCALL;


					// Function returning the running status of the thread.
					ATHENA_DLL bool ATHENA_PRECALL is_running() const ATHENA_POSTCALL;
					// Function returning the functionality function of the thread.
					ATHENA_DLL threadFunction ATHENA_PRECALL functionality() ATHENA_POSTCALL;
					// Function returning the parameter of the thread.
					ATHENA_DLL void* ATHENA_PRECALL parameter() ATHENA_POSTCALL;
					
					// Function returning the exit code of the thread. Should be called after destroy().
					#ifdef _WIN32
						ATHENA_DLL DWORD ATHENA_PRECALL exit_code() const ATHENA_POSTCALL;
					#else
						ATHENA_DLL int ATHENA_PRECALL exit_code() const ATHENA_POSTCALL;
					#endif /* _WIN32 */


					// Function creating the thread. Returns true on success, false on failure.
					ATHENA_DLL bool ATHENA_PRECALL create() ATHENA_POSTCALL;
					// Function destroying the thread.
					ATHENA_DLL void ATHENA_PRECALL destroy() ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */


	#include "thread.inl"



#endif /* ATHENA_UTILITY_THREAD_HPP */