#include "conditionVariable.hpp"
#include "criticalSection.hpp"
#include "readersWriterLock.hpp"

#ifndef _WIN32
	#include <ctime>
#endif /* _WIN32 */



namespace athena
{

	namespace utility
	{

		ConditionVariable::ConditionVariable() : 
			
			#ifdef _WIN32	
				_initialised(true)
			#else
				_initialised(false)
			#endif /* _WIN32 */

		{
			#ifdef _WIN32
				InitializeConditionVariable(&_variable);
			#else
				
				if ( pthread_cond_init(&_variable,NULL) == 0 )
					_initialised = true;

			#endif /* _WIN32 */
		};

		ConditionVariable::~ConditionVariable()
		{
			#ifdef _WIN32
			#else
				pthread_cond_destroy(&_variable);
			#endif /* _WIN32 */
		};


		#ifdef _WIN32
			ATHENA_DLL void ATHENA_CALL ConditionVariable::wait( CriticalSection& lock , const DWORD milliseconds )
		#else
			ATHENA_DLL void ATHENA_CALL ConditionVariable::wait(  CriticalSection& lock , const unsigned long milliseconds )
		#endif /* _WIN32 */
		{
			#ifdef _WIN32 
				SleepConditionVariableCS(&_variable,&(lock.lock_ref()),milliseconds);
			#else
				
				if ( time == INFINITE )
					pthread_cond_wait(&_variable,&(lock.lock_ref()));
				else
				{
					timespec timer;


					memset(&timer,'\0',sizeof(timer));
					timer.tn_nsec = milliseconds*1000000;
					pthread_cond_timedwait*&_variable,&(lock.lock_ref()),&timer);
				}

			#endif /* _WIN32 */
		};

		#ifdef _WIN32
			ATHENA_DLL void ATHENA_CALL ConditionVariable::wait( ReadersWriterLock& lock , const DWORD milliseconds , const ULONG flags )
		#else
			ATHENA_DLL void ATHENA_CALL ConditionVariable::wait(  ReadersWriterLock& lock , const unsigned long milliseconds , const unsigned long flags )
		#endif /* _WIN32 */
		{
			#ifdef _WIN32 
				SleepConditionVariableSRW(&_variable,&(lock.lock_ref()),milliseconds,flags);
			#else	
			#endif /* _WIN32 */
		};

	} /* utility */

} /* athena */