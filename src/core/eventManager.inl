#ifndef ATHENA_CORE_EVENTMANAGER_INL
	#define ATHENA_CORE_EVENTMANAGER_INL



	namespace athena
	{

		namespace core
		{

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

				// A function responsible of performing the operation of the manager if single-threaded mode is enabled.
				inline void EventManager::operate()
				{
					_operate();
				}

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		} /* core */

	} /* athena */



#endif /* ATHENA_CORE_EVENTMANAGER_INL */