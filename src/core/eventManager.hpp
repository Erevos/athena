#ifndef ATHENA_CORE_EVENT_MANAGER_HPP
	#define ATHENA_CORE_EVENT_MANAGER_HPP

	#include "../athenaDefinitions.hpp"
	#include "../utility/readersWriterLock.hpp"
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
			
			
			
			/*
				Function definitions.
			*/

			
			inline bool EventManager::initialise()
			{
				bool return_value = true;


				_instance_lock.lock(false);

				if ( _instance == NULL )
				{
					_instance = new (std::nothrow) EventManager();

					if ( _instance == NULL )
						return_value = false;
				}

				_instance_lock.unlock();


				return return_value;
			};

			inline void EventManager::deinitialise()
			{
				_instance_lock.lock(false);

				if ( _instance != NULL )
				{
					delete _instance;
					_instance = NULL;
				}

				_instance_lock.unlock();
			};

			inline EventManager* EventManager::get()
			{
				EventManager* return_value = NULL;


				_instance_lock.lock(true);
				return_value = _instance;
				_instance_lock.unlock();


				return return_value;
			};


		} /* core */

	} /* athena */



#endif /* ATHENA_CORE_EVENT_MANAGER_HPP */