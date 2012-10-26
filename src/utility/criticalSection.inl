#ifndef ATHENA_UTILITY_CRITICALSECTION_INL
	#define ATHENA_UTILITY_CRITICALSECTION_INL



	namespace athena
	{

		namespace utility
		{

			#ifdef _WIN32
				inline ATHENA_DLL CRITICAL_SECTION& ATHENA_CALL CriticalSection::lock_ref()
			#else
				inline ATHENA_DLL pthread_mutex_t& ATHENA_CALL CriticalSection::lock_ref()
			#endif /* _WIN32 */
			{
				return _lock;
			};

			inline void CriticalSection::lock( const bool )
			{
				if ( _initialised )
				{
					#ifdef _WIN32
						EnterCriticalSection(&_lock);
					#else
						pthread_mutex_lock(&_lock);
					#endif /* _WIN32 */
				}
			};

			inline bool CriticalSection::try_lock( const bool )
			{
				bool return_value = false;


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

			inline void CriticalSection::unlock()
			{
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