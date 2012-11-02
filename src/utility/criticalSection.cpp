#include	"criticalSection.hpp"



namespace athena
{

	namespace utility
	{

		/*
			The constructor of the class, for Windows implementations you can have an optional
			parameter for the amount of "cycles" the critical section will try to obtain a lock.
		*/

		#ifdef _WIN32

			CriticalSection::CriticalSection( DWORD spincount ) : 
				_initialised(true) 

		#else

			CriticalSection::CriticalSection() : 
				_initialised(false)

		#endif /* _WIN32 */
		{
			#ifdef _WIN32
				InitializeCriticalSectionAndSpinCount(&_lock,spincount);
			#else

				pthread_mutexattr_t attributes;


				// Need to initialise the mutex attribute structure.
				if ( pthread_mutexattr_init(&attributes) == 0 )
				{
					// If the mutex attribute structure is initialsed, set the recursive attribute.
					if ( pthread_mutexattr_settype(&attributes,PTHREAD_MUTEX_RECURSIVE) == 0 )
					{
						// If the attribute is settup successfully, initialise the lock.
						_initialised = ( pthread_mutex_init(&_lock,&attributes) == 0 );
						// Deallocate any resources taken by the attribute structure.
						pthread_mutexattr_destroy(&attributes);
					}
				}

			#endif /* _WIN32 */
		};

		// The destructor of the class.
		CriticalSection::~CriticalSection()
		{
			#ifdef _WIN32
				DeleteCriticalSection(&_lock);
			#else
				pthread_mutex_destroy(&_lock);
			#endif /* _WIN32 */
		};

	} /* utility */

} /* athena */