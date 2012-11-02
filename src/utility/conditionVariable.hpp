#ifndef ATHENA_PARALLEL_CONDITIONVARIABLE_HPP
#define ATHENA_PARALLEL_CONDITIONVARIABLE_HPP

	#include "../athenaDefinitions.hpp"

	#ifdef _WIN32
		#include <Windows.h>
	#else
		
		#ifndef INFINITE
			#define INFINITE 0xFFFFFFFF
		#endif /* INFINITE */
		
		#include <pthread.h>

	#endif /* _WIN32 */



	namespace athena
	{

		namespace utility
		{

			/*
				Forward declarations.
			*/
			class CriticalSection;
			class ReadersWriterLock;


			/*
				Class handling a condition variable
				for use with readers-writer locks and critical sections.
			*/
			class ConditionVariable
			{
				private:

					// The variable tha holds the condition variable.
					#ifdef _WIN32
						CONDITION_VARIABLE _variable;
					#else
						pthread_cond_t _variable;
					#endif /* _WIN32 */

					// Variable holding whether the condition variable has been initialised.
					bool _initialised;


				public:

					// The constructor of the class.
					ATHENA_DLL ATHENA_PRECALL ConditionVariable() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~ConditionVariable() ATHENA_POSTCALL;


					/*
						Wait functions to be used with critical sections and reader-writers locks respectively.
						Due to the fact that the Windows API used DWORD variables and Unix systems are using
						integers there is a difference in the function APIs ( therefore an unsigned long was used
						although a DWORD is usually represented by an unsigned long ). The millisecond variable refers to 
						the amount of millisecond the lock should wait.
						The pthread API does not support condition variables for readers-writer whereas the Windows API does, 
						therefore for portability issues the best approach would be to use critical sections with condition variables.
					*/

					#ifdef _WIN32

						ATHENA_DLL void ATHENA_PRECALL wait( CriticalSection& lock , const DWORD milliseconds = INFINITE ) ATHENA_POSTCALL;
						ATHENA_DLL void ATHENA_PRECALL wait( ReadersWriterLock& lock , const DWORD milliseconds = INFINITE , const ULONG flags = 0 ) ATHENA_POSTCALL;

					#else

						ATHENA_DLL void ATHENA_PRECALL wait( CriticalSection& lock , const unsigned long milliseconds = INFINITE ) ATHENA_POSTCALL;
						ATHENA_DLL void ATHENA_PRECALL wait( ReadersWriterLock& lock , const unsigned long milliseconds = INFINITE , const unsigned long flags = 0 ) ATHENA_POSTCALL;

					#endif /* _WIN32 */

					// Wake a single thread that is in a wait state.
					ATHENA_DLL void ATHENA_PRECALL wake() ATHENA_POSTCALL;
					// Wake all the threads that are in a  wait state on that condition variable.
					ATHENA_DLL void ATHENA_PRECALL wake_all() ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */


	#include "conditionVariable.inl"



#endif /* ATHENA_PARALLEL_CONDITIONVARIABLE_HPP */