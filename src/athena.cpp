#include "athena.hpp"
#include <mutex>
#include <condition_variable>
#include "windowsDefinitions.hpp"
#include <GL/freeglut.h>
#include "threadPool.hpp"
#include "eventManager.hpp"
#include "renderManager.hpp"
#include "audioManager.hpp"
#include "logManager.hpp"
#include "inputManager.hpp"

#ifdef _WIN32
	#pragma warning(disable:4505)
#endif /* _WIN32 */



namespace athena
{
	std::mutex status_lock;

	#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
		std::condition_variable_any exit_condition;
	#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

	bool glut_initialised = false;
	bool athena_manager_initialisation[ManagerIDs::FirstAvailableID] = {
		false,
		false,
		false,
		false,
		false
	};


	#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

		void exit_cleanup( const core::Event& )
		{
			exit_condition.notify_all();
		}

	#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


	// Function responsible of initialising the specified managers.
	bool init( const AthenaManagers& managers , int& argc , char**& argv )
	{
		bool return_value = true;


		status_lock.lock();

		if ( !glut_initialised )
		{
			glutInit(&argc,argv);
			glut_initialised = true;
		}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

		if ( ( managers & THREAD_POOL ) != 0  ) )
		{

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			return_value &= core::ThreadPool::init();
			athena_manager_initialisation[ManagerIDs::ThreadPool] = true;

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		return_value &= core::EventManager::init();
		athena_manager_initialisation[ManagerIDs::EventManagerID] = true;


		if ( ( managers & RENDER_MANAGER ) != 0 )
		{
			return_value &= display::RenderManager::init();
			athena_manager_initialisation[ManagerIDs::RenderManagerID] = true;
		}

		if ( ( managers & AUDIO_MANAGER ) != 0 )
		{
			return_value &= display::AudioManager::init();
			athena_manager_initialisation[ManagerIDs::AudioManagerID] = true;
		}

		if (  ( managers & INPUT_MANAGER ) != 0 )
		{
			return_value &= io::InputManager::init();
			athena_manager_initialisation[ManagerIDs::InputManagerID] = true;
		}

		if ( ( managers & LOG_MANAGER ) != 0 )
		{
			return_value &= io::LogManager::init();
			athena_manager_initialisation[ManagerIDs::LogManagerID] = true;
		}


		if ( !return_value )
		{
			for ( unsigned int i = ManagerIDs::FirstAvailableID-1;  i < ManagerIDs::FirstAvailableID;  --i )
			{
				if ( athena_manager_initialisation[i] )
				{
					switch ( i )
					{
						case ManagerIDs::ThreadPool:
							
							core::ThreadPool::deinit();
							break;

						case ManagerIDs::EventManagerID:
							
							core::EventManager::deinit();
							break;

						case ManagerIDs::RenderManagerID:
							
							display::RenderManager::deinit();
							break;

						case ManagerIDs::AudioManagerID:

							display::AudioManager::deinit();
							break;

						case ManagerIDs::InputManagerID:
							
							io::InputManager::deinit();
							break;

						case ManagerIDs::LogManagerID:
							
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

			if ( athena_manager_initialisation[ManagerIDs::ThreadPool] )
			{
				core::ThreadPool* pool = core::ThreadPool::get();


				if ( pool != NULL )
					return_value &= pool->startup();
			}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		if ( athena_manager_initialisation[ManagerIDs::EventManagerID] )
		{
			core::EventManager*  event_manager = core::EventManager::get();


			if ( event_manager != NULL )
				return_value &= event_manager->startup();
		}

		if ( ( managers & RENDER_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::RenderManagerID] )
		{
			display::RenderManager* render_manager = display::RenderManager::get();


			if ( render_manager != NULL )
				return_value &= render_manager->startup();
		}

		if ( ( managers & AUDIO_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::AudioManagerID] )
		{
			display::AudioManager* audio_manager = display::AudioManager::get();


			if ( audio_manager != NULL )
				return_value &= audio_manager->startup();
		}

		if (  ( managers & INPUT_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::InputManagerID] )
		{
			io::InputManager* input_manager = io::InputManager::get();


			if ( input_manager != NULL )
				return_value &= input_manager->startup();
		}

		if ( ( managers & LOG_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::LogManagerID] )
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

		if ( ( managers & RENDER_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::RenderManagerID] )
		{
			display::RenderManager* render_manager = display::RenderManager::get();


			if ( render_manager != NULL )
				render_manager->terminate();

			display::RenderManager::deinit();
			athena_manager_initialisation[ManagerIDs::RenderManagerID] = false;
		}

		if ( ( managers & AUDIO_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::AudioManagerID] )
		{
			display::AudioManager* audio_manager = display::AudioManager::get();


			if ( audio_manager != NULL )
				audio_manager->terminate();

			display::AudioManager::deinit();
			athena_manager_initialisation[ManagerIDs::AudioManagerID] = false;
		}

		if (  ( managers & INPUT_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::InputManagerID] )
		{
			io::InputManager* input_manager = io::InputManager::get();


			if ( input_manager != NULL )
				input_manager->terminate();

			io::InputManager::deinit();
			athena_manager_initialisation[ManagerIDs::InputManagerID] = false;
		}

		if ( ( managers & LOG_MANAGER ) != 0  &&  athena_manager_initialisation[ManagerIDs::LogManagerID] )
		{
			io::LogManager* log_manager = io::LogManager::get();


			if ( log_manager != NULL )
				log_manager->terminate();

			io::LogManager::deinit();
			athena_manager_initialisation[ManagerIDs::LogManagerID] = false;
		}

		if ( athena_manager_initialisation[ManagerIDs::EventManagerID] )
		{
			core::EventManager*  event_manager = core::EventManager::get();


			if ( event_manager != NULL )
				event_manager->terminate();

			core::EventManager::deinit();
			athena_manager_initialisation[ManagerIDs::EventManagerID] = false;
		}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

		if ( ( managers & THREAD_POOL ) != 0 )
		{

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			if ( athena_manager_initialisation[ManagerIDs::ThreadPool] )
			{
				core::ThreadPool* pool = core::ThreadPool::get();


				if ( pool != NULL )
					pool->terminate();

				core::ThreadPool::deinit();
				athena_manager_initialisation[ManagerIDs::ThreadPool] = false;
			}

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		}
		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		status_lock.unlock();
	}


	// A function responsible of performing any update operations.
	void operate()
	{
		glutMainLoopEvent();

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
			
			// Get a pointer to the event manager.
			core::EventManager* manager = core::EventManager::get();


			// If the manager is initialised.
			if ( manager != NULL )
				manager->operate();

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
	}

	// A function that triggers the terminating sequence of the engine and deinitialising any managers.
	void terminate()
	{
		// Get a pointer to the event manager.
		core::EventManager* manager = core::EventManager::get();


		// If the manager is initialised.
		if ( manager != NULL )
		{
			core::Event event(EVENT_EXIT);

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

				manager->trigger_event(event); // Trigger the exit event.
				manager->operate();
				deinit(ALL);

			#else
				
				event.cleanup_function(exit_cleanup);
				status_lock.lock();
				manager->trigger_event(event); // Trigger the exit event.
				exit_condition.wait(status_lock);
				status_lock.unlock();
				deinit(ALL);

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
		}
	}


	// A function that triggers an event. Allias for the direct call from the manager.
	void trigger_event( const core::Event& event )
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
			manager->trigger_event_periodically(event,period); // Register the periodic triggering.
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