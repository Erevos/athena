#ifndef ATHENA_UTILITY_CONDITIONVARIABLE_INL
	#define ATHENA_UTILITY_CONDITIONVARIABLE_INL



	namespace athena
	{

		namespace utility
		{

			ATHENA_DLL void ATHENA_CALL ConditionVariable::wake()
			{
				if ( _initialised )
				{
					#ifdef _WIN32
						WakeConditionVariable(&_variable);
					#else
						pthread_cond_signal(&_variable);
					#endif /* _WIN32 */
				}
			};

			ATHENA_DLL void ATHENA_CALL ConditionVariable::wake_all()
			{
				if ( _initialised )
				{
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