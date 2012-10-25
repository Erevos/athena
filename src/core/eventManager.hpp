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


					ATHENA_CALL EventManager();
					ATHENA_CALL ~EventManager();


				public:

					ATHENA_DLL static bool ATHENA_CALL initialise();
					ATHENA_DLL static void ATHENA_CALL deinitialise();
					ATHENA_DLL static EventManager* ATHENA_CALL get();

			};

		} /* core */

	} /* athena */


	#include "eventManager.inl"



#endif /* ATHENA_CORE_EVENT_MANAGER_HPP */