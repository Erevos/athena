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


				public:

					ATHENA_DLL ATHENA_CALL ReadersWriterLock();
					ATHENA_DLL ATHENA_CALL ~ReadersWriterLock();


					ATHENA_DLL bool ATHENA_CALL initialise();
					ATHENA_DLL void ATHENA_CALL deinitialise();
					ATHENA_DLL void ATHENA_CALL lock( const bool shared );
					ATHENA_DLL bool ATHENA_CALL try_lock( const bool shared );
					ATHENA_DLL void ATHENA_CALL unlock();
			};

			
			
			/*
				Function definitions.
			*/

			
			inline bool ReadersWriterLock::initialise()
			{
				return false;
			};

			inline void ReadersWriterLock::deinitialise()
			{
			};

			inline void ReadersWriterLock::lock( const bool shared )
			{
				shared;
			};

			inline bool ReadersWriterLock::try_lock( const bool shared )
			{
				shared;
				return false;
			};

			inline void ReadersWriterLock::unlock()
			{
			};

		} /* utility */

	} /* athena */


#endif /* ATHENA_PARALLEL_NONREENTRANT_LOCK_HPP */