#include "readersWriterLock.hpp"



namespace athena
{
	
	namespace utility
	{

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
				_initialised = ( pthread_rwlock_init(&_lock,NULL) == 0 )
			#endif /* _WIN32 */
		};

		ReadersWriterLock::~ReadersWriterLock()
		{
			#ifdef _WIN32
			#else
				pthread_rwlock_destroy(&_lock);
			#endif /* _WIN32 */
		};

	} /* utility */

} /* athena */