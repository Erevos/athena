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

		// The constructor of the class.
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
				
				if ( pthread_cond_init(&_variable,INVALID_POINTER) == 0 )
					_initialised = true;

			#endif /* _WIN32 */
		};

		// The destructor of the class.
		ConditionVariable::~ConditionVariable()
		{
			#ifdef _WIN32
			#else
				pthread_cond_destroy(&_variable);
			#endif /* _WIN32 */
		};


		/*
			Wait functions to be used with critical sections and reader-writers locks respectively.
			Due to the fact that the Windows API used DWORD variables and Unix systems are using
			integers there is a difference in the function APIs ( therefore an unsigned long was used
			although a DWORD is usually represented by an unsigned long ). The millisecond variable refers to 
			the amount of millisecond the lock should wait.
			The pthread API does not support condition variables for readers-writer whereas the Windows API does, 
			therefore for portability issues the best approach would be to use critical sections with condition variables.
		*/
		void ConditionVariable::wait( CriticalSection& lock , const WaitValueType milliseconds )
		{
			#ifdef _WIN32 
				SleepConditionVariableCS(&_variable,&(lock.lock_ref()),milliseconds);
			#else
				
				// If the wait time is infinite
				if ( time == INFINITE ) // Put the calling thread on wait status.
					pthread_cond_wait(&_variable,&(lock.lock_ref()));
				else // If the timer is not infinite.
				{
					timespec timer;


					// Transform the given millisecond wait to a timespec struct.
					memset(&timer,'\0',sizeof(timer));
					timer.tn_nsec = milliseconds*1000000;
					// And put the calling thread on wait status.
					pthread_cond_timedwait*&_variable,&(lock.lock_ref()),&timer);
				}

			#endif /* _WIN32 */
		};

		void ConditionVariable::wait( ReadersWriterLock& lock , const WaitValueType milliseconds , const FlagValueType flags )
		{
			/*
				The pthread implementation does not support condition variables for readers-writer locks , 
				whereas the Windows API implementation does, therefore for portability issues the best approach 
				would be to use critical sections with condition variables.
			*/

			#ifdef _WIN32 
				SleepConditionVariableSRW(&_variable,&(lock.lock_ref()),milliseconds,flags);
			#else	
			#endif /* _WIN32 */
		};

		// Wake a single thread that is in a wait state.
		void ConditionVariable::wake()
		{
			// If the condition variable is initialised.
			if ( _initialised )
			{
				// Wake the calling thread.

				#ifdef _WIN32
					WakeConditionVariable(&_variable);
				#else
					pthread_cond_signal(&_variable);
				#endif /* _WIN32 */
			}
		};

		// Wake all the threads that are in a  wait state on that condition variable.
		void ConditionVariable::wake_all()
		{
			// If the condition variable is initialised.
			if ( _initialised )
			{
				// Wake all the threads that are currently waiting on the condition variable.
				#ifdef _WIN32
					WakeAllConditionVariable(&_variable);
				#else
					pthread_cond_broadcast(&_variable);
				#endif /* _WIN32 */
			}
		};

	} /* utility */

} /* athena */