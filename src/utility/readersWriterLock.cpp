#include "readersWriterLock.hpp"



namespace athena
{
	
	namespace utility
	{

		// The constructor of the class.
		ReadersWriterLock::ReadersWriterLock() : 
			
			#ifdef _WIN32	
				_initialised(true) , 
			#else
				_initialised(false) , 
			#endif /* _WIN32 */

			_shared_lock(false)
		{
			// Initialise the lock.
			#ifdef _WIN32
				InitializeSRWLock(&_lock);
			#else
				_initialised = ( pthread_rwlock_init(&_lock,INVALID_POINTER) == 0 )
			#endif /* _WIN32 */
		}

		// The destructor of the class.
		ReadersWriterLock::~ReadersWriterLock()
		{
			#ifdef _WIN32
			#else
				pthread_rwlock_destroy(&_lock);
			#endif /* _WIN32 */
		}


		// Function obtaining the critical section. The Readers-Writer lock supports shared functionality.
		void ReadersWriterLock::lock( const bool shared )
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
		}

		/*
			Function trying to obtain the critical section.
			The Readers-Writer lock supports shared functionality.
			Returns true on success.
		*/
		bool ReadersWriterLock::try_lock( const bool shared )
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
		}

		// Function releasing the Readers-Writer lock. 
		void ReadersWriterLock::unlock()
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
		}

	} /* utility */

} /* athena */