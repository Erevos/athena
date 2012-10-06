#ifndef ATHENA_PARALLEL_CONDITIONVARIABLE_HPP
#define ATHENA_PARALLEL_CONDITIONVARIABLE_HPP

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

			class	ConditionVariable
			{
				private:



				public:


			};



			/*
				Function definitions.
			*/


			

		} /* utility */

	} /* athena */



#endif /* ATHENA_PARALLEL_CONDITIONVARIABLE_HPP */