#ifndef ATHENA_CORE_EVENT_MANAGER_HPP
	#define ATHENA_CORE_EVENT_MANAGER_HPP

	#include "../athenaDefinitions.hpp"
	#include "../utility/criticalSection.hpp"
	#include "../utility/timer.hpp"
	#include "event.hpp"
	#include "entity.hpp"
	#include "periodicEventInfo.hpp"
	
	#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
		#include "../utility/thread.hpp"
	#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

	#include <map>
	#include <vector>
	#include <deque>



	namespace athena
	{

		namespace core
		{

			/*
				An enumeration holding the possible operations on an entity or
				the possible operations an entity can perform regarding an event.
			*/
			enum AvailableEntityOperations
			{
				AddEvent = 0 , 
				RemoveEvent , 
				RemoveAllEvents , 
			};


			/*
				A struct that is used by the Event Manager in order to 
				manage any pending entity operations.
			*/
			struct EntityOperation
			{
				AvailableEntityOperations operation;
				EventCode code;
				Entity* entity;
			};


			/*
				A singleton class representing and handling the event system of the engine.
				The event system forms the main message parsing functionality of the engine, 
				informing entities of any triggered events.
				If the macro ATHENA_EVENTMANAGER_SINGLETHREADED is defined the event manager performs
				its functionality when the operate() function is called. If the macro is not defined, the
				event manager spawns a thread which is responsible of performing the operation of the manager.
				When an event is passed to the event system a SHALLOW copy is performed. Therefore the caller should
				make sure the parameters of the event do not go out of scope. Once the event has been successfully 
				triggered a callback function will be called in order to perform any necessary cleanup.
			*/
			class EventManager
			{
				private:

					// The single instance of the class.
					static EventManager* _instance;
					// A lock used to handle concurrency issues regarding the instance of the class.
					static utility::ReadersWriterLock _instance_lock;
					
					#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
					#else
						
						/// The amount of time the spawned thread will sleep if there is nothing to do.
						#ifdef _WIN32
							static const unsigned long _sleep_time;
						#else
							static const unsigned int _sleep_time;
						#endif /* _WIN32 */

					#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


					// The map that holds the registry of events and the entities that are registered for each event.
					std::map<EventCode,std::vector<Entity*> > _event_list;
					// The map that holds the registry of entities and the events they are registered with.
					std::map<Entity*,std::vector<EventCode> > _entity_list;
					// The event queue that is processed by the event manager.
					std::deque<Event> _event_queue;
					// The periodic event list that is triggered every specified interval.
					std::vector<PeriodicEventInfo> _periodic_event_list;
					/*
						The pending entity operation queue that is used to 
						insert or delete entities or event notifications from the event and entity list
					*/
					std::deque<EntityOperation> _pending_operation_queue;
					// An array holding the events to be removed from the event list.
					std::vector<EventCode> _pending_event_removals;
					// An array  holding the entities to be removed from the entity list.
					std::vector<Entity*> _pending_entity_removals;
					// A lock that is used to handle concurrency issues.
					utility::CriticalSection _lock;
					// A lock that is used to handle initialisation issues.
					utility::ReadersWriterLock _initialisation_lock;
					// A timer that is used for timing the elapsed time for periodic events.
					utility::Timer _timer;

					#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
					#else
						
						utility::Thread _thread;


						// The static function that is used by the thread in order to perform the needed functionality.
						static utility::ThreadExitType ATHENA_PRECALL _thread_function( void* parameter ) ATHENA_POSTCALL;

					#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


					// The constructor of the class.
					ATHENA_PRECALL EventManager() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_PRECALL ~EventManager() ATHENA_POSTCALL;


					// A function responsible of queuing a entity to be removed.
					void ATHENA_PRECALL _queue_entity_removal( Entity* entity ) ATHENA_POSTCALL;
					// A function responsible of queuing an event to be removed.
					void ATHENA_PRECALL _queue_event_removal( const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of performing any queued removals.
					void ATHENA_PRECALL _perform_removals() ATHENA_POSTCALL;
					// A function responsible of adding notification regarding a single event for an entity.
					void ATHENA_PRECALL _add_event( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator , const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of removing notification regarding a single event for an entity.
					void ATHENA_PRECALL _remove_event( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator , const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of removing notification regarding all events for an entity.
					void ATHENA_PRECALL _remove_all_events( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator ) ATHENA_POSTCALL;
					// A function responsible of performing any entity operations.
					void ATHENA_PRECALL _perform_entity_operation( EntityOperation& operation ) ATHENA_POSTCALL;
					// A function responsible of performing the operation of the event manager.
					void ATHENA_PRECALL _operate() ATHENA_POSTCALL;
					// A function responsible of performing cleanup.
					void ATHENA_PRECALL _cleanup() ATHENA_POSTCALL;


				public:

					// A function responsible of initialising the single instance of the class.
					ATHENA_DLL static bool ATHENA_PRECALL initialise() ATHENA_POSTCALL;
					// A function responsible of deinitialising the single instance of the class.
					ATHENA_DLL static void ATHENA_PRECALL deinitialise() ATHENA_POSTCALL;
					// A function responsible of returning a single instance of the class.
					ATHENA_DLL static EventManager* ATHENA_PRECALL get() ATHENA_POSTCALL;


					// A function responsible of commencing the functionality of the event system.
					ATHENA_DLL bool ATHENA_PRECALL startup() ATHENA_POSTCALL;
					// A function responsible of terminating the functionality of the event system.
					ATHENA_DLL void ATHENA_PRECALL terminate() ATHENA_POSTCALL;
					// A function responsible of triggering an event with the given parameters and id code.
					ATHENA_DLL void ATHENA_PRECALL trigger_event( const Event& event ) ATHENA_POSTCALL;
					/*
						A function responsible of registering an event to be triggered periodically.
						The parameter at index 0 will be used to set the time that has passed since last trigger.
						If the parameter is not the proper type or if there are no parameter the event manager will 
						add the needed parameter.
					*/
					ATHENA_DLL void ATHENA_PRECALL triger_event_periodically( Event& event , const double& period ) ATHENA_POSTCALL;
					// A function responsible of unregistering an event from being triggered periodically.
					ATHENA_DLL void ATHENA_PRECALL stop_triggerring_event_periodically( const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of registering an entity for notification of the specified event code.
					ATHENA_DLL void ATHENA_PRECALL register_event( Entity* entity , const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of unregistering an entity from notification of the specified event code.
					ATHENA_DLL void ATHENA_PRECALL unregister_event( Entity* entity , const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of unregistering an entity from notification of all events.
					ATHENA_DLL void ATHENA_PRECALL unregister_all_events( Entity* entity ) ATHENA_POSTCALL;

					#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

						// A function responsible of performing the operation of the manager if single-threaded mode is enabled.
						ATHENA_DLL void ATHENA_PRECALL operate() ATHENA_POSTCALL;

					#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			};

		} /* core */

	} /* athena */


	#include "eventManager.inl"



#endif /* ATHENA_CORE_EVENT_MANAGER_HPP */