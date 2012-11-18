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

			/*
				A class representing and handling a Readers-Writer lock.
				It is NOT re-entrant.
			*/
			class ReadersWriterLock : public Lock
			{
				private:

					// A variable representing the Readers-Writer lock.
					#ifdef _WIN32
						SRWLOCK _lock;
					#else
						pthread_rwlock_t _lock;
					#endif /* _WIN32 */

					/*
						Variable that is used to check whether the critical section has been initialised.
						On Windows the initialisation can not fail (aka the function returns void ) , 
						however on Unit the pthread equivalent can possibly fail ( the function returns an int ).
					*/
					volatile bool _initialised;
					// A variable that is used to check if the lock is in shared mode or not.
					volatile bool _shared_lock;


				protected:

					// Declare the condition variable as a friend class in order to use the reference functions.
					friend class ConditionVariable;


					/*
						A function returning a reference to the critical section variable.
						For use with functions that need access to the variable.
					*/
					#ifdef _WIN32
						ATHENA_DLL SRWLOCK& ATHENA_PRECALL lock_ref() ATHENA_POSTCALL;
					#else
						ATHENA_DLL pthread_rwlock_t& ATHENA_PRECALL lock_ref() ATHENA_POSTCALL;
					#endif /* _WIN32 */

									
				public:

					// The constructor of the class.
					ATHENA_DLL ATHENA_PRECALL ReadersWriterLock() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~ReadersWriterLock() ATHENA_POSTCALL;


					// A function returning whether the critical section has been initialised.
					ATHENA_DLL bool ATHENA_PRECALL initialised() const ATHENA_POSTCALL;


					// Function obtaining the critical section. The Readers-Writer lock supports shared functionality.
					ATHENA_DLL void ATHENA_PRECALL lock( const bool shared ) ATHENA_POSTCALL;
					/*
						Function trying to obtain the critical section.
						The Readers-Writer lock supports shared functionality.
						Returns true on success.
					*/
					ATHENA_DLL bool ATHENA_PRECALL try_lock( const bool shared ) ATHENA_POSTCALL;
					// Function releasing the Readers-Writer lock. 
					ATHENA_DLL void ATHENA_PRECALL unlock() ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */


	#include "readersWriterLock.inl"



#endif /* ATHENA_PARALLEL_NONREENTRANT_LOCK_HPP */