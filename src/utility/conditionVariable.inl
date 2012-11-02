#ifndef ATHENA_UTILITY_CONDITIONVARIABLE_INL
	#define ATHENA_UTILITY_CONDITIONVARIABLE_INL



	namespace athena
	{

		namespace utility
		{

			// Wake a single thread that is in a wait state.
			inline void ConditionVariable::wake()
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
			inline void ConditionVariable::wake_all()
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



#endif /* ATHENA_UTILITY_CONDITIONVARIABLE_INL */