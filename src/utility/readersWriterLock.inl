#ifndef ATHENA_UTILITY_READERSWRITERLOCK_INL
	#define ATHENA_UTILITY_READERSWRITERLOCK_INL



	namespace athena
	{

		namespace utility
		{

			#ifdef _WIN32
				inline ATHENA_DLL SRWLOCK& ATHENA_CALL ReadersWriterLock::lock_ref()
			#else
				inline ATHENA_DLL pthread_rwlock_t& ATHENA_CALL ReadersWriterLock::lock_ref()
			#endif /* _WIN32 */
			{
				return _lock;
			};


			inline void ReadersWriterLock::lock( const bool shared )
			{
				if ( _initialised )
				{
					if ( shared )
					{
						#ifdef _WIN32
							AcquireSRWLockShared(&_lock);
						#else
							pthread_rwlock_rdlock(&_lock);
						#endif /* _WIN32 */

						_shared_lock = true;
					}
					else
					{
						#ifdef _WIN32
							AcquireSRWLockExclusive(&_lock);
						#else
							pthread_rwlock_wrlock(&_lock);
						#endif /* _WIN32 */

						_shared_lock = false;
					}
				}
			};

			inline bool ReadersWriterLock::try_lock( const bool shared )
			{
				bool return_value = false;


				if ( _initialised )
				{
					if ( shared )
					{
						#ifdef _WIN32
							return_value = ( TryAcquireSRWLockShared(&_lock) != FALSE );
						#else
							return_value = ( pthread_rwlock_tryrdlock(&_lock) == 0 );
						#endif /* _WIN32 */

						_shared_lock = true;
					}
					else
					{
						#ifdef _WIN32
							return_value = ( TryAcquireSRWLockExclusive(&_lock) != FALSE );
						#else
							return_value = ( pthread_rwlock_trywrlock(&_lock) == 0 );
						#endif /* _WIN32 */

						_shared_lock = false;
					}
				}

				
				return false;
			};

			inline void ReadersWriterLock::unlock()
			{
				if ( _initialised )
				{
					if ( _shared_lock )
					{
						#ifdef _WIN32
							ReleaseSRWLockShared(&_lock);
						#else
							pthread_rwlock_unlock(&_lock);
						#endif /* _WIN32 */
					}
					else
					{
						#ifdef _WIN32
							ReleaseSRWLockExclusive(&_lock);
						#else
							pthread_rwlock_unlock(&_lock);
						#endif /* _WIN32 */
					}
				}
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_UTILITY_READERSWRITERLOCK_INL */