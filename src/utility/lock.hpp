#ifndef ATHENA_PARALLEL_LOCK_HPP
	#define ATHENA_PARALLEL_LOCK_HPP

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
					ATHENA_DLL ATHENA_PRECALL Lock() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL virtual ATHENA_PRECALL ~Lock() ATHENA_POSTCALL;


					//  A function returning whether the lock has been initialised.
					ATHENA_DLL virtual bool ATHENA_PRECALL initialised() const = 0 ATHENA_POSTCALL;


					// A function acquiring the lock.
					ATHENA_DLL virtual void ATHENA_PRECALL lock( const bool shared = false ) = 0 ATHENA_POSTCALL;
					// A function trying to acquire the lock. Returns true on success.
					ATHENA_DLL virtual bool ATHENA_PRECALL try_lock( const bool shared = false ) = 0 ATHENA_POSTCALL;
					// A function releasing the lock.
					ATHENA_DLL virtual void ATHENA_PRECALL unlock() = 0 ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_PARALLEL_LOCK_HPP */