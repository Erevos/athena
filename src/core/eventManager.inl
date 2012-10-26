#ifndef ATHENA_CORE_EVENTMANAGER_INL
	#define ATHENA_CORE_EVENTMANAGER_INL



	namespace athena
	{

		namespace core
		{

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



#endif /* ATHENA_CORE_EVENTMANAGER_INL */