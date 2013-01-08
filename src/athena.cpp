#include "athena.hpp"
#include <mutex>
#include "core/threadPool.hpp"
#include "core/eventManager.hpp"
#include "display/renderManager.hpp"
#include "io/logManager.hpp"
#include "io/inputManager.hpp"



namespace athena
{
	static const unsigned int manager_count = 5;
	static std::mutex status_lock;
	static bool athena_manager_initialisation[manager_count] = {
																	false,
																	false,
																	false,
																	false,
																	false
																};


	// Function responsible of initialising the specified managers.
	bool init( const AthenaManagers& managers )
	{
		bool return_value = true;


		status_lock.lock();

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

		if ( ( managers & THREAD_POOL ) != 0  ) )
		{

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			return_value &= core::ThreadPool::init();
			athena_manager_initialisation[0] = true;

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		return_value &= core::EventManager::init();
		athena_manager_initialisation[1] = true;


		if ( ( managers & RENDER_MANAGER ) != 0 )
		{
			return_value &= display::RenderManager::init();
			athena_manager_initialisation[2] = true;
		}

		if (  ( managers & INPUT_MANAGER ) != 0 )
		{
			return_value &= io::InputManager::init();
			athena_manager_initialisation[3] = true;
		}

		if ( ( managers & LOG_MANAGER ) != 0 )
		{
			return_value &= io::LogManager::init();
			athena_manager_initialisation[4] = true;
		}


		if ( !return_value )
		{
			for ( unsigned int i = manager_count-1;  i < manager_count;  --i )
			{
				if ( athena_manager_initialisation[i] )
				{
					switch ( i )
					{
						case 0:
							
							core::ThreadPool::deinit();
							break;

						case 1:
							
							core::EventManager::deinit();
							break;

						case 2:
							
							display::RenderManager::deinit();
							break;

						case 3:
							
							io::InputManager::deinit();
							break;

						case 4:
							
							io::LogManager::deinit();
							break;

						default:

							break;
					}

					athena_manager_initialisation[i] = false;
				}
			}
		}

		status_lock.unlock();


		return return_value;
	}
	
	// Function responsible of starting the functionality of the specified managers.
	bool startup( const AthenaManagers& managers )
	{
		bool return_value = true;


		status_lock.lock();

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

		if ( ( managers & THREAD_POOL ) != 0 )
		{

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			if ( athena_manager_initialisation[0] )
			{
				core::ThreadPool* pool = core::ThreadPool::get();


				if ( pool != NULL )
					return_value &= pool->startup();
			}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		if ( athena_manager_initialisation[1] )
		{
			core::EventManager*  event_manager = core::EventManager::get();


			if ( event_manager != NULL )
				return_value &= event_manager->startup();
		}

		if ( ( managers & RENDER_MANAGER ) != 0  &&  athena_manager_initialisation[2] )
		{
			display::RenderManager* render_manager = display::RenderManager::get();


			if ( render_manager != NULL )
				return_value &= render_manager->startup();
		}

		if (  ( managers & INPUT_MANAGER ) != 0  &&  athena_manager_initialisation[3] )
		{
			io::InputManager* input_manager = io::InputManager::get();


			if ( input_manager != NULL )
				return_value &= input_manager->startup();
		}

		if ( ( managers & LOG_MANAGER ) != 0  &&  athena_manager_initialisation[4] )
		{
			io::LogManager* log_manager = io::LogManager::get();


			if ( log_manager != NULL )
				return_value &= log_manager->startup();
		}

		status_lock.unlock();


		return return_value;
	}
	
	// Function responsible of deinitialising the specified managers.
	void deinit( const AthenaManagers& managers )
	{
		status_lock.lock();

		if ( ( managers & RENDER_MANAGER ) != 0  &&  athena_manager_initialisation[2] )
		{
			display::RenderManager* render_manager = display::RenderManager::get();


			if ( render_manager != NULL )
				render_manager->terminate();

			display::RenderManager::deinit();
			athena_manager_initialisation[2] = false;
		}

		if (  ( managers & INPUT_MANAGER ) != 0  &&  athena_manager_initialisation[3] )
		{
			io::InputManager* input_manager = io::InputManager::get();


			if ( input_manager != NULL )
				input_manager->terminate();

			io::InputManager::deinit();
			athena_manager_initialisation[3] = false;
		}

		if ( ( managers & LOG_MANAGER ) != 0  &&  athena_manager_initialisation[4] )
		{
			io::LogManager* log_manager = io::LogManager::get();


			if ( log_manager != NULL )
				log_manager->terminate();

			io::LogManager::deinit();
			athena_manager_initialisation[4] = false;
		}

		if ( athena_manager_initialisation[1] )
		{
			core::EventManager*  event_manager = core::EventManager::get();


			if ( event_manager != NULL )
				event_manager->terminate();

			core::EventManager::deinit();
			athena_manager_initialisation[1] = false;
		}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

		if ( ( managers & THREAD_POOL ) != 0 )
		{

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			if ( athena_manager_initialisation[0] )
			{
				core::ThreadPool* pool = core::ThreadPool::get();


				if ( pool != NULL )
					pool->terminate();

				core::ThreadPool::deinit();
				athena_manager_initialisation[0] = false;
			}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		status_lock.unlock();
	}


	// The main loop of the engine.
	void mainLoop()
	{

	}

	// A function that triggers the terminating sequence of the engine.
	void terminate()
	{
		// Get a pointer to the event manager.
		core::EventManager* manager = core::EventManager::get();


		// If the manager is initialised.
		if ( manager != NULL )
			manager->trigger_event(core::Event(EVENT_EXIT)); // Trigger the exit event.
	}


	// A function that triggers an event. Allias for the direct call from the manager.
	void triger_event( const core::Event& event )
	{
		// Get a pointer to the event manager.
		core::EventManager* manager = core::EventManager::get();


		// If the manager is initialised.
		if ( manager != NULL )
			manager->trigger_event(event); // Trigger the given event.
	}

	// A function that registers an event for periodic triggering. Allias for the direct call from the manager.
	void trigger_event_periodically( core::Event& event , const utility::TimerValueType& period )
	{
		// Get a pointer to the event manager.
		core::EventManager* manager = core::EventManager::get();


		// If the manager is initialised.
		if ( manager != NULL )
			manager->triger_event_periodically(event,period); // Register the periodic triggering.
	}

	// A function that unregisters an event from periodic triggering. Allias for the direct call from the manager.
	void stop_triggering_event_periodically( const core::EventCode& code )
	{
		// Get a pointer to the event manager.
		core::EventManager* manager = core::EventManager::get();


		// If the manager is initialised.
		if ( manager != NULL )
			manager->stop_triggerring_event_periodically(code); // Stop the periodic triggering.
	}

} /* athena */