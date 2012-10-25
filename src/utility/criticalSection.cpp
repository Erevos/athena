#include	"criticalSection.hpp"



namespace athena
{

	namespace utility
	{

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


				if ( pthread_mutexattr_init(&attributes) == 0 )
				{
					if ( pthread_mutexattr_settype(&attributes,PTHREAD_MUTEX_RECURSIVE) == 0 )
					{
						_initialised = ( pthread_mutex_init(&_lock,&attributes) == 0 )
						pthread_mutexattr_destroy(&attributes);
					}
				}

			#endif /* _WIN32 */
		};

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