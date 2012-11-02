#ifndef ATHENA_UTILITY_READERSWRITERLOCK_INL
	#define ATHENA_UTILITY_READERSWRITERLOCK_INL



	namespace athena
	{

		namespace utility
		{

			/*
				A function returning a reference to the critical section variable.
				For use with functions that need access to the variable.
			*/

			#ifdef _WIN32
				inline ATHENA_DLL SRWLOCK& ReadersWriterLock::lock_ref()
			#else
				inline ATHENA_DLL pthread_rwlock_t& ReadersWriterLock::lock_ref()
			#endif /* _WIN32 */
			{
				return _lock;
			};


			// A function returning whether the critical section has been initialised.
			inline bool ReadersWriterLock::initialised() const
			{
				return _initialised;
			};


			// Function obtaining the critical section. The Readers-Writer lock supports shared functionality.
			inline void ReadersWriterLock::lock( const bool shared )
			{
				// If the lock is initialised.
				if ( _initialised )
				{
					// If the lock should be acquired in shared mode.
					if ( shared )
					{
						// Acquire the lock in shared mode.

						#ifdef _WIN32
							AcquireSRWLockShared(&_lock);
						#else
							pthread_rwlock_rdlock(&_lock);
						#endif /* _WIN32 */

						// Set the shared variable.
						_shared_lock = true;
					}
					else
					{
						// Acquire the lock in exclusive mode.

						#ifdef _WIN32
							AcquireSRWLockExclusive(&_lock);
						#else
							pthread_rwlock_wrlock(&_lock);
						#endif /* _WIN32 */

						// Reset the shared variable.
						_shared_lock = false;
					}
				}
			};

			/*
				Function trying to obtain the critical section.
				The Readers-Writer lock supports shared functionality.
				Returns true on success.
			*/
			inline bool ReadersWriterLock::try_lock( const bool shared )
			{
				bool return_value = false;


				// If the lock is initialised.
				if ( _initialised )
				{
					// If the lock should be acquired in shared mode.
					if ( shared )
					{
						// Try acquiring the lock in shared mode.

						#ifdef _WIN32
							return_value = ( TryAcquireSRWLockShared(&_lock) != FALSE );
						#else
							return_value = ( pthread_rwlock_tryrdlock(&_lock) == 0 );
						#endif /* _WIN32 */

						if ( return_value )	// If the lock has been acquired
							_shared_lock = true;	// Set the shared variable.
					}
					else
					{
						// Try acquiring the lock in exclusive mode.
						#ifdef _WIN32
							return_value = ( TryAcquireSRWLockExclusive(&_lock) != FALSE );
						#else
							return_value = ( pthread_rwlock_trywrlock(&_lock) == 0 );
						#endif /* _WIN32 */

						if ( return_value )	// If the lock has been acquired
							_shared_lock = false;	// Reset the shared variable.
					}
				}

				
				return false;
			};

			// Function releasing the Readers-Writer lock. 
			inline void ReadersWriterLock::unlock()
			{
				// If the lock has been initialised.
				if ( _initialised )
				{
					// If the lock is in shared mode.
					if ( _shared_lock )
					{
						// Release the shared lock.

						#ifdef _WIN32
							ReleaseSRWLockShared(&_lock);
						#else
							pthread_rwlock_unlock(&_lock);
						#endif /* _WIN32 */
					}
					else
					{
						// Release the exclusive lock.

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