#ifndef ATHENA_CORE_EVENT_MANAGER_HPP
	#define ATHENA_CORE_EVENT_MANAGER_HPP

	#include "../athenaDefinitions.hpp"
	#include "../utility/readersWriterLock.hpp"
	#include "event.hpp"
	#include <new>



	namespace athena
	{

		namespace core
		{

			class EventManager
			{
				private:

					static EventManager* _instance;
					static utility::ReadersWriterLock _instance_lock;


					ATHENA_PRECALL EventManager() ATHENA_POSTCALL;
					ATHENA_PRECALL ~EventManager() ATHENA_POSTCALL;


				public:

					ATHENA_DLL static bool ATHENA_PRECALL initialise() ATHENA_POSTCALL;
					ATHENA_DLL static void ATHENA_PRECALL deinitialise() ATHENA_POSTCALL;
					ATHENA_DLL static EventManager* ATHENA_PRECALL get() ATHENA_POSTCALL;

			};

		} /* core */

	} /* athena */


	#include "eventManager.inl"



#endif /* ATHENA_CORE_EVENT_MANAGER_HPP */