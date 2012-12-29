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

		// A definition of the type of the wait values.
		#ifdef _WIN32
			typedef DWORD WaitValueType;
		#else
			typedef unsigned long WaitValueType;
		#endif /* _WIN32 */

		// A definition of the type of the flag values.
		#ifdef _WIN32
			typedef ULONG FlagValueType;
		#else
			typedef unsigned long FlagValueType;
		#endif /* _WIN32 */


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
				ATHENA_DLL ConditionVariable();
				// The destructor of the class.
				ATHENA_DLL ~ConditionVariable();


				/*
					Wait functions to be used with critical sections and reader-writers locks respectively.
					Due to the fact that the Windows API used DWORD variables and Unix systems are using
					integers there is a difference in the function APIs ( therefore an unsigned long was used
					although a DWORD is usually represented by an unsigned long ). The millisecond variable refers to 
					the amount of millisecond the lock should wait.
					The pthread API does not support condition variables for readers-writer whereas the Windows API does, 
					therefore for portability issues the best approach would be to use critical sections with condition variables.
				*/
				ATHENA_DLL void wait( CriticalSection& lock , const WaitValueType milliseconds = INFINITE );
				ATHENA_DLL void wait( ReadersWriterLock& lock , const WaitValueType milliseconds = INFINITE , const FlagValueType flags = 0 );
				// Wake a single thread that is in a wait state.
				ATHENA_DLL void wake();
				// Wake all the threads that are in a  wait state on that condition variable.
				ATHENA_DLL void wake_all();
		};

	} /* utility */

} /* athena */



#endif /* ATHENA_PARALLEL_CONDITIONVARIABLE_HPP */