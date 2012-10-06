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



				public:

					ATHENA_DLL ATHENA_CALL CriticalSection();
					ATHENA_DLL ATHENA_CALL ~CriticalSection();


					ATHENA_DLL bool ATHENA_CALL initialise();
					ATHENA_DLL void ATHENA_CALL deinitialise();
					ATHENA_DLL void ATHENA_CALL lock( const bool shared );
					ATHENA_DLL bool ATHENA_CALL try_lock( const bool shared );
					ATHENA_DLL void ATHENA_CALL unlock();
			};
			
			
			
			/*
				Function definitions.
			*/

			
			inline bool CriticalSection::initialise()
			{
				return false;
			};

			inline void CriticalSection::deinitialise()
			{
			};

			inline void CriticalSection::lock( const bool )
			{
			};

			inline bool CriticalSection::try_lock( const bool )
			{
				return false;
			};

			inline void CriticalSection::unlock()
			{
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_PARALLEL_CRITICAL_SECTION_HPP */