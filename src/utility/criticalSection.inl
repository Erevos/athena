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


			// Function obtaining the critical section. The critical section does not support shared functionality.
			inline void CriticalSection::lock( const bool )
			{
				// If the lock is initialised.
				if ( _initialised )
				{
					#ifdef _WIN32
						EnterCriticalSection(&_lock);
					#else
						pthread_mutex_lock(&_lock);
					#endif /* _WIN32 */
				}
			};

			// Function trying to obtain the critical section. The critical section does not support shared functionality.
			inline bool CriticalSection::try_lock( const bool )
			{
				bool return_value = false;


				// If the lock is initialised.
				if ( _initialised )
				{
					#ifdef _WIN32
						return_value = ( TryEnterCriticalSection(&_lock) != FALSE );
					#else
						return_value = ( pthread_mutex_trylokc(&_lock) == 0 );
					#endif /* _WIN32 */
				}


				return false;
			};

			// Function releasing the critical section. 
			inline void CriticalSection::unlock()
			{
				// If the lock is initialised.
				if ( _initialised )
				{
					#ifdef _WIN32
						LeaveCriticalSection(&_lock);
					#else
						pthread_mutex_unlock(&_lock);
					#endif /* _WIN32 */
				}
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_UTILITY_CRITICALSECTION_INL */