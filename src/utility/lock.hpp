#ifndef ATHENA_PARALLEL_LOCK_HPP
	#define ATHENA_PARALLEL_LOCK_HPP

	#include "../athenaDefinitions.hpp"



	namespace athena
	{

		namespace utility
		{

			class Lock
			{
				public:

					ATHENA_DLL ATHENA_CALL Lock();
					ATHENA_DLL virtual ATHENA_CALL ~Lock();


					ATHENA_DLL virtual void ATHENA_CALL lock( const bool shared = false ) = 0;
					ATHENA_DLL virtual bool ATHENA_CALL try_lock( const bool shared = false ) = 0;
					ATHENA_DLL virtual void ATHENA_CALL unlock() = 0;
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_PARALLEL_LOCK_HPP */