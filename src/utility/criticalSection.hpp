#ifndef ATHENA_UTILITY_CRITICALSECTION_HPP
#define ATHENA_UTILITY_CRITICALSECTION_HPP

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
					ATHENA_DLL CRITICAL_SECTION& lock_ref();
				#else
					ATHENA_DLL pthread_mutex_t& lock_ref();
				#endif /* _WIN32 */


			public:

				/*
					The constructor of the class, for Windows implementations you can have an optional
					parameter for the amount of "cycles" the critical section will try to obtain a lock.
				*/
				#ifdef _WIN32
					explicit ATHENA_DLL CriticalSection( DWORD spincount = 0);
				#else
					ATHENA_DLL CriticalSection();
				#endif /* _WIN32 */
					
				// The destructor of the class.
				ATHENA_DLL ~CriticalSection();


				// A function returning whether the critical section has been initialised.
				ATHENA_DLL bool initialised() const;


				// Function obtaining the critical section. The critical section does not support shared functionality.
				ATHENA_DLL void lock( const bool shared );
				/*
					Function trying to obtain the critical section.
					The critical section does not support shared functionality.
					Returns true on success.
				*/
				ATHENA_DLL bool try_lock( const bool shared );
				// Function releasing the critical section. 
				ATHENA_DLL void unlock();
		};

	} /* utility */

} /* athena */


#include "criticalSection.inl"



#endif /* ATHENA_UTILITY_CRITICALSECTION_HPP */