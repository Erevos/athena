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

			/*
				A class representing and handling a critical section.
				It is a re-entrant lock.
			*/
			class CriticalSection : public Lock
			{
				private:

					// A variable representing the critical section.
					#ifdef _WIN32
						CRITICAL_SECTION _lock;
					#else
						pthread_mutex_t _lock;
					#endif /* _WIN32 */

					/*
						Variable that is used to check whether the critical section has been initialised.
						On Windows the initialisation can not fail (aka the function returns void ) , 
						however on Unit the pthread equivalent can possibly fail ( the function returns an int ).
					*/
					volatile bool _initialised;


				protected:

					// Declare the condition variable as a friend class in order to use the reference functions.
					friend class ConditionVariable;


					/*
						A function returning a reference to the critical section variable.
						For use with functions that need access to the variable.
					*/

					#ifdef _WIN32
						ATHENA_DLL CRITICAL_SECTION& ATHENA_PRECALL lock_ref() ATHENA_POSTCALL;
					#else
						ATHENA_DLL pthread_mutex_t& ATHENA_PRECALL lock_ref() ATHENA_POSTCALL;
					#endif /* _WIN32 */


				public:

					/*
						The constructor of the class, for Windows implementations you can have an optional
						parameter for the amount of "cycles" the critical section will try to obtain a lock.
					*/
					#ifdef _WIN32
						explicit ATHENA_DLL ATHENA_PRECALL CriticalSection( DWORD spincount = 0) ATHENA_POSTCALL;
					#else
						ATHENA_DLL ATHENA_PRECALL CriticalSection() ATHENA_POSTCALL;
					#endif /* _WIN32 */
					
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~CriticalSection() ATHENA_POSTCALL;


					// A function returning whether the critical section has been initialised.
					ATHENA_DLL bool ATHENA_PRECALL initialised() const ATHENA_POSTCALL;


					// Function obtaining the critical section. The critical section does not support shared functionality.
					ATHENA_DLL void ATHENA_PRECALL lock( const bool shared ) ATHENA_POSTCALL;
					/*
						Function trying to obtain the critical section.
						The critical section does not support shared functionality.
						Returns true on success.
					*/
					ATHENA_DLL bool ATHENA_PRECALL try_lock( const bool shared ) ATHENA_POSTCALL;
					// Function releasing the critical section. 
					ATHENA_DLL void ATHENA_PRECALL unlock() ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */


	#include "criticalSection.inl"



#endif /* ATHENA_PARALLEL_CRITICAL_SECTION_HPP */