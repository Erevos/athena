#ifndef ATHENA_CORE_EVENTMANAGER_HPP
#define ATHENA_CORE_EVENTMANAGER_HPP

#include "../definitions.hpp"
#include <mutex>
#include <chrono>
	
#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

	#include <condition_variable>
	#include <thread>

#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

#include <map>
#include <vector>
#include <deque>
#include "../athena.hpp"
#include "event.hpp"
#include "listener.hpp"
#include "periodicEventInfo.hpp"



namespace athena
{

	namespace core
	{

		/*
			An enumeration holding the possible operations on a listener or
			the possible operations a listener can perform regarding an event.
		*/
		enum AvailableListenerOperations
		{
			AddEvent = 0 , 
			AddAllEvents , 
			RemoveEvent , 
			RemoveAllEvents
		};


		/*
			A struct that is used by the Event Manager in order to 
			manage any pending listener operations.
		*/
		struct ListenerOperation
		{
			AvailableListenerOperations m_operation;
			EventCode m_code;
			Listener* m_listener;

			ListenerOperation( const AvailableListenerOperations operation , const EventCode code , Listener* listener ) :
				m_operation(operation) , 
				m_code(code) , 
				m_listener(listener)
			{
			};
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
				static EventManager* s_instance;
				// A lock used to handle concurrency issues regarding the instance of the class.
				static std::mutex s_instance_lock;
					
				#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
				#else
						
					// The amount of time the spawned thread will sleep if there is nothing to do.
					#ifdef _WIN32
						static const unsigned long s_sleep_time;
					#else
						static const unsigned int s_sleep_time;
					#endif /* _WIN32 */

				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


				// The map that holds the registry of events and the entities that are registered for each event.
				std::map<EventCode,std::vector<Listener*> > m_event_list;
				// The map that holds the registry of entities and the events they are registered with.
				std::map<Listener*,std::vector<EventCode> > m_listener_list;
				// The event queue that is processed by the event manager.
				std::deque<Event*> m_event_queue;
				// The periodic event list that is triggered every specified interval.
				std::vector<PeriodicEventInfo*> m_periodic_event_list;
				/*
					The pending listener operation queue that is used to 
					insert or delete entities or event notifications from the event and listener list
				*/
				std::deque<ListenerOperation*> m_pending_operation_queue;
				// An array holding the events to be removed from the event list.
				std::vector<EventCode> m_pending_event_removals;
				// An array  holding the entities to be removed from the listener list.
				std::vector<Listener*> m_pending_listener_removals;
				// A lock that is used to handle concurrency issues.
				std::recursive_mutex m_lock;
				// A lock that is used to handle initialisation issues.
				std::recursive_mutex m_initialisation_lock;
				// A timer that is used for timing the elapsed time for periodic events.
				utility::Timer m_timer;

				#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

					/*
						The condition variable that is used to syncronise the
						functionality thread with the main thread when the 
						terminate function is called.
					*/
					std::condition_variable_any m_thread_condition_variable;
					// The mutes that is used with the condition variable.
					std::mutex m_thread_mutex;
					// A variable containing whether the thread should run or not.
					bool m_running;

				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

				// A variable holding whether the class has been initialised.
				bool m_initialised;


				#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
					
					// The static function that is used by the thread in order to perform the needed functionality.
					static int thread_function( void* parameter );
					// The static function that is used to perform the callback functionality for the spawned thread.
					static void thread_callback_function( const int exit_code , void* parameter );

				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

				// The constructor of the class.
				EventManager();
				// The destructor of the class.
				~EventManager();


				// A function responsible of queuing a listener to be removed.
				void queue_listener_removal( Listener* listener );
				// A function responsible of queuing an event to be removed.
				void queue_event_removal( const EventCode& code );
				// A function responsible of performing any queued removals.
				void perform_removals();
				// A function responsible of adding notification regarding a single event for a listener.
				void add_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code );
				// A function responsible of removing notification regarding a single event for a listener.
				void remove_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code );
				// A function responsible of removing notification regarding all events for a listener.
				void remove_all_events( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const bool removable );
				// A function responsible of performing any listener operations.
				void perform_listener_operation( ListenerOperation& operation );
				// A function responsible of performing the operation of the event manager.
				void actual_operate();
				// A function responsible of performing cleanup.
				void cleanup();


			protected:

				friend bool athena::init( const AthenaManagers& managers , int& argc , char**& argv );
				friend bool athena::startup( const AthenaManagers& managers );
				friend void athena::deinit( const AthenaManagers& managers );


				// A function responsible of initialising the single instance of the class.
				ATHENA_DLL static bool init();
				// A function responsible of deinitialising the single instance of the class.
				ATHENA_DLL static void deinit();


				// A function responsible of commencing the functionality of the event system.
				ATHENA_DLL bool startup();
				// A function responsible of terminating the functionality of the event system.
				ATHENA_DLL void terminate();


			public:

				// A function responsible of returning a single instance of the class.
				ATHENA_DLL static EventManager* get();


				// A function responsible of triggering an event with the given parameters and id code.
				ATHENA_DLL void trigger_event( const Event& event );
				/*
					A function responsible of registering an event to be triggered periodically.
					The parameter at index 0 will be used to set the time that has passed since last trigger.
					If the parameter is not the proper type or if there are no parameter the event manager will 
					add the needed parameter.
				*/
				ATHENA_DLL void trigger_event_periodically( Event& event , const utility::TimerValueType& period );
				// A function responsible of unregistering an event from being triggered periodically.
				ATHENA_DLL void stop_triggerring_event_periodically( const EventCode& code );
				// A function responsible of registering a listener for notification of the specified event code.
				ATHENA_DLL void register_event( Listener* listener , const EventCode& code );
				/*
					A function responsible of registering all events for the listener.
					This function will unregister all previously registered events.
					Additionally, events cannot be unregistered on an individual basis, instead a call
					to unregister_all_events must be made.
				*/
				ATHENA_DLL void register_all_events( Listener* listener );
				// A function responsible of unregistering a listener from notification of the specified event code.
				ATHENA_DLL void unregister_event( Listener* listener , const EventCode& code );
				// A function responsible of unregistering a listener from notification of all events.
				ATHENA_DLL void unregister_all_events( Listener* listener );

				#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

					// A function responsible of performing the operation of the manager if single-threaded mode is enabled.
					ATHENA_DLL void operate();

				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
		};

	} /* core */

} /* athena */


#include "eventManager.inl"



#endif /* ATHENA_CORE_EVENTMANAGER_HPP */