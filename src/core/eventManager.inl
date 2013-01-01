#ifndef ATHENA_CORE_EVENTMANAGER_INL
#define ATHENA_CORE_EVENTMANAGER_INL

#ifndef ATHENA_CORE_EVENTMANAGER_HPP
	#error "eventManager.hpp must be included before eventManager.inl"
#endif /* ATHENA_CORE_EVENTAMANGER_HPP */



namespace athena
{

	namespace core
	{

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

			// A function responsible of performing the operation of the manager if single-threaded mode is enabled.
			inline void EventManager::operate()
			{
				f_operate();
			}

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

	} /* core */

} /* athena */



#endif /* ATHENA_CORE_EVENTMANAGER_INL */