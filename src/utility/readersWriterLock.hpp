#ifndef ATHENA_PARALLEL_NONREENTRANTLOCK_HPP
	#define ATHENA_PARALLEL_NONREENTRANTLOCK_HPP

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

			class ReadersWriterLock : public Lock
			{
				private:

					#ifdef _WIN32
						SRWLOCK _lock;
					#else
						pthread_rwlock_t _lock;
					#endif /* _WIN32 */

					volatile bool _initialised;
					volatile bool _shared_lock;


				protected:

					friend class ConditionVariable;


					#ifdef _WIN32
						ATHENA_DLL SRWLOCK& ATHENA_CALL lock_ref();
					#else
						ATHENA_DLL pthread_rwlock_t& ATHENA_CALL lock_ref();
					#endif /* _WIN32 */

									
				public:

					ATHENA_DLL ATHENA_CALL ReadersWriterLock();
					ATHENA_DLL ATHENA_CALL ~ReadersWriterLock();


					ATHENA_DLL void ATHENA_CALL lock( const bool shared );
					ATHENA_DLL bool ATHENA_CALL try_lock( const bool shared );
					ATHENA_DLL void ATHENA_CALL unlock();
			};

		} /* utility */

	} /* athena */


	#include "readersWriterLock.inl"



#endif /* ATHENA_PARALLEL_NONREENTRANT_LOCK_HPP */