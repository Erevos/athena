#ifndef ATHENA_PARALLEL_CRITICAL_SECTION_HPP
	#define ATHENA_PARALLEL_CRITICAL_SECTION_HPP

	#include "../athenaDefinitions.hpp"
	#include "lock.hpp"

	#ifdef _WIN32
		#include <Windows.h>
	#else
		#include <pthread.h>
	#endif /* _WIN32 */



	namespace athena
	{
	
		namespace utility 
		{

			class CriticalSection : public Lock
			{
				private:

					#ifdef _WIN32
						CRITICAL_SECTION _lock;
					#else
						pthread_mutex_t _lock;
					#endif /* _WIN32 */

					volatile bool _initialised;


				protected:

					friend class ConditionVariable;


					#ifdef _WIN32
						ATHENA_DLL CRITICAL_SECTION& ATHENA_CALL lock_ref();
					#else
						ATHENA_DLL pthread_mutex_t& ATHENA_CALL lock_ref();
					#endif /* _WIN32 */


				public:

					#ifdef _WIN32
						ATHENA_DLL ATHENA_CALL CriticalSection( DWORD spincount = 0);
					#else
						ATHENA_DLL ATHENA_CALL CriticalSection();
					#endif /* _WIN32 */
					
					ATHENA_DLL ATHENA_CALL ~CriticalSection();


					ATHENA_DLL void ATHENA_CALL lock( const bool shared );
					ATHENA_DLL bool ATHENA_CALL try_lock( const bool shared );
					ATHENA_DLL void ATHENA_CALL unlock();
			};

		} /* utility */

	} /* athena */


	#include "criticalSection.inl"



#endif /* ATHENA_PARALLEL_CRITICAL_SECTION_HPP */