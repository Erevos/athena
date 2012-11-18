#ifndef ATHENA_UTILITY_CRITICALSECTION_INL
	#define ATHENA_UTILITY_CRITICALSECTION_INL



	namespace athena
	{

		namespace utility
		{

			/*
				A function returning a reference to the critical section variable.
				For use with functions that need access to the variable.
			*/

			#ifdef _WIN32
				inline CRITICAL_SECTION& CriticalSection::lock_ref()
			#else
				inline pthread_mutex_t& CriticalSection::lock_ref()
			#endif /* _WIN32 */
			{
				return _lock;
			};


			// A function returning whether the critical section has been initialised.
			inline bool CriticalSection::initialised() const
			{
				return _initialised;
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_UTILITY_CRITICALSECTION_INL */