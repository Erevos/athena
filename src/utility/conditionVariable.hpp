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

			class CriticalSection;
			class ReadersWriterLock;


			class ConditionVariable
			{
				private:

					#ifdef _WIN32
						CONDITION_VARIABLE _variable;
					#else
						pthread_cond_t _variable;
					#endif /* _WIN32 */

					bool _initialised;


				public:

					ATHENA_DLL ATHENA_CALL ConditionVariable();
					ATHENA_DLL ATHENA_CALL ~ConditionVariable();


					#ifdef _WIN32

						ATHENA_DLL void ATHENA_CALL wait( CriticalSection& lock , const DWORD milliseconds = INFINITE );
						ATHENA_DLL void ATHENA_CALL wait( ReadersWriterLock& lock , const DWORD milliseconds = INFINITE , const ULONG flags = 0 );

					#else

						ATHENA_DLL void ATHENA_CALL wait( CriticalSection& lock , const unsigned long milliseconds = INFINITE );
						ATHENA_DLL void ATHENA_CALL wait( ReadersWriterLock& lock , const unsigned long milliseconds = INFINITE , const unsigned long flags = 0 );

					#endif /* _WIN32 */

					ATHENA_DLL void ATHENA_CALL wake();
					ATHENA_DLL void ATHENA_CALL wake_all();
			};

		} /* utility */

	} /* athena */


	#include "conditionVariable.inl"



#endif /* ATHENA_PARALLEL_CONDITIONVARIABLE_HPP */