#ifndef ATHENA_UTILITY_LOCK_HPP
#define ATHENA_UTILITY_LOCK_HPP

#include "../athenaDefinitions.hpp"



namespace athena
{

	namespace utility
	{

		/*
			An abstract class representing a lock.
			It is used as an interface for use by other lock classes.
		*/
		class Lock
		{
			public:

				// The constructor of the class.
				ATHENA_DLL Lock();
				// The destructor of the class.
				ATHENA_DLL virtual ~Lock();


				//  A function returning whether the lock has been initialised.
				ATHENA_DLL virtual bool initialised() const = 0;


				// A function acquiring the lock.
				ATHENA_DLL virtual void lock( const bool shared = false ) = 0;
				// A function trying to acquire the lock. Returns true on success.
				ATHENA_DLL virtual bool try_lock( const bool shared = false ) = 0;
				// A function releasing the lock.
				ATHENA_DLL virtual void unlock() = 0;
		};

	} /* utility */

} /* athena */



#endif /* ATHENA_UTILITY_LOCK_HPP */