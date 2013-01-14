#include "eventManager.hpp"
#include "threadPool.hpp"
#include "eventCodes.hpp"

#ifdef __unix
	#include <unistd.h>
#endif /* __UNIX */



namespace athena
{
	
	namespace core
	{

		// The single instance of the class.
		EventManager* EventManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex EventManager::s_instance_lock;

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		#else
						
			/// The amount of time the spawned thread will sleep if there is nothing to do.
			#ifdef _WIN32
				const DWORD EventManager::s_sleep_time = 1;
			#else
				const unsigned int EventManager::s_sleep_time = 1000;
			#endif /* _WIN32 */

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

			// The static function that is used by the thread in order to perform the needed functionality.
			int EventManager::thread_function( void* parameter )
			{
				EventManager* manager = static_cast<EventManager*>(parameter);
				
				
				if ( manager != NULL )
				{
					bool run = true;


					while ( run )
					{
						manager->m_initialisation_lock.lock();
						run = manager->m_running;
						manager->m_initialisation_lock.unlock();

						if ( run )
							manager->actual_operate();
					}
				}


				return 0;
			}

			// The static function that is used to perform the callback functionality for the spawned thread.
			void EventManager::thread_callback_function( const int , void* parameter )
			{
				EventManager* manager = static_cast<EventManager*>(parameter);
				
				
				if ( manager != NULL )
					manager->m_thread_condition_variable.notify_all();
			}

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		// The constructor of the class.
		EventManager::EventManager() :
			m_event_list() ,
			m_listener_list() ,
			m_event_queue(0) ,
			m_periodic_event_list(0) ,
			m_pending_operation_queue(0) ,
			m_pending_event_removals(0) ,
			m_pending_listener_removals(0) ,
			m_lock() ,
			m_initialisation_lock() ,
			m_timer() ,

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

				m_thread_condition_variable() ,
				m_thread_mutex() ,
				m_running(false) ,

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			m_initialised(false)
		{
		}

		// The destructor of the class.
		EventManager::~EventManager()
		{
			// Perform cleanup if it has not been done already.
			cleanup();
		}


		// A function responsible of queuing an event to be removed.
		void EventManager::queue_listener_removal( Listener* listener )
		{
			std::vector<Listener*>::iterator listener_iterator(m_pending_listener_removals.begin());


			// Check the listener removal queue if the listener is already queued for removal.
			while ( listener_iterator != m_pending_listener_removals.end() )
			{
				// If the listener is already queue stop the loop.
				if ( (*listener_iterator) == listener )
					break;

				++listener_iterator;
			}

			// If the listener is not queued for removal, add it to the queue.
			if  ( listener_iterator == m_pending_listener_removals.end() )
				m_pending_listener_removals.push_back(listener);
		}

		// A function responsible of queuing an event to be removed.
		void EventManager::queue_event_removal( const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(m_pending_event_removals.begin());


			// Check the event removal queue if the event is already queued for removal.
			while ( event_iterator != m_pending_event_removals.end() )
			{
				// If the event is already queued for removal stop the loop.
				if ( (*event_iterator) == code )
					break;

				++event_iterator;
			}

			// If the event is not queued for removal, add it to the queue.
			if  ( event_iterator == m_pending_event_removals.end() )
				m_pending_event_removals.push_back(code);
		}

		// A function responsible of performing any queued removals.
		void EventManager::perform_removals()
		{
			// For every listener in the listener removal queue.
			for (
					std::vector<Listener*>::iterator listener_iterator = m_pending_listener_removals.begin();
					listener_iterator != m_pending_listener_removals.end();
					++listener_iterator
				)
			{
				// Find the listener in the listener list.
				std::map<Listener*,std::vector<EventCode> >::iterator listener_list_iterator(m_listener_list.find((*listener_iterator)));


				// If the listener exists in the list, remove it from the list.
				if ( listener_list_iterator != m_listener_list.end() )
					m_listener_list.erase(listener_list_iterator);
			}

			// For every event in the event removal queue.
			for (
					std::vector<EventCode>::iterator event_iterator = m_pending_event_removals.begin();
					event_iterator != m_pending_event_removals.end();
					++event_iterator
				)
			{
				// Find the event in the event list.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find((*event_iterator)));


				// If the event exists in the list, remove it from the list.
				if ( event_list_iterator != m_event_list.end() )
					m_event_list.erase(event_list_iterator);
			}

			// Clear the removal queues.
			m_pending_listener_removals.clear();
			m_pending_event_removals.clear();
		}

		// A function responsible of adding notification regarding a single event for a listener.
		void EventManager::add_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(listener_iterator->second.begin());


			// Check the listener's event list if the event has already been added.
			while ( event_iterator != listener_iterator->second.end() )
			{
				// If the event has already been added, exit the loop.
				if ( (*event_iterator) == code )
					break;

				++event_iterator;
			}

			// If the event has not been added already.
			if ( event_iterator == listener_iterator->second.end() )
			{
				// Check if the event has been added to the event list.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find(code));


				// If the event has been added to the event list.
				if ( event_list_iterator != m_event_list.end() )
				{
					std::vector<Listener*>::iterator event_list_listener_iterator(event_list_iterator->second.begin());
					

					// Check if the listener has been added to the event's list. (A bit redundant could be skipped for a performance boost).
					while ( event_list_listener_iterator != event_list_iterator->second.end() )
					{
						// If the listener is already in the event's list, exit the loop.
						if ( (*event_list_listener_iterator) == listener_iterator->first )
							break;

						++event_list_listener_iterator;
					}

					// If the listener is not in the event's list, add it.
					if ( event_list_listener_iterator == event_list_iterator->second.end() )
						event_list_iterator->second.push_back(listener_iterator->first);
				}
				else // If the event has not been added, add it to the event list and add the listener to the event's list.
					m_event_list.insert(std::pair<EventCode,std::vector<Listener*> >(code,std::vector<Listener*>(1,listener_iterator->first)));

				// Add the event to the listener's event list.
				listener_iterator->second.push_back(code);
			}
		}

		// A function responsible of removing notification regarding a single event for a listener.
		void EventManager::remove_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(listener_iterator->second.begin());


			// Check if the event exists in the listener's event list.
			while ( event_iterator != listener_iterator->second.end() )
			{
				// If the event exists in the list.
				if ( (*event_iterator) == code )
				{
					// Find the event in the event list.
					std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find(code));


					// If the event exists in the event list.
					if ( event_list_iterator != m_event_list.end() )
					{
						std::vector<Listener*>::iterator event_list_listener_iterator(event_list_iterator->second.begin());
					

						// Find the listener is in the event's listener list.
						while ( event_list_listener_iterator != event_list_iterator->second.end() )
						{
							// When the listener is found.
							if ( (*event_list_listener_iterator) == listener_iterator->first )
							{
								// Remove the listener from the event's list.
								event_list_iterator->second.erase(event_list_listener_iterator);

								// If the event's listener list is empty, queue the event for removal.
								if ( event_list_iterator->second.size() == 0 )
									queue_event_removal(code);

								// Exit the loop.
								break;
							}

							++event_list_listener_iterator;
						}
					}

					// Remove the event from the listener's event list and exit the loop.
					listener_iterator->second.erase(event_iterator);
					break;
				}

				++event_iterator;
			}

			// If the listener's event list is empty, queue the listener for removal.
			if ( listener_iterator->second.size() == 0 )
				queue_listener_removal(listener_iterator->first);
		}

		// A function responsible of removing notification regarding all events for a listener.
		void EventManager::remove_all_events( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const bool removable )
		{
			// For all the events in the listener's event list.
			for (
					std::vector<EventCode>::iterator event_iterator(listener_iterator->second.begin());
					event_iterator != listener_iterator->second.end();
					++event_iterator
				)
			{
				// Find the event in the event list.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find((*event_iterator)));


				// If the event is found.
				if ( event_list_iterator != m_event_list.end() )
				{
					std::vector<Listener*>::iterator event_list_listener_iterator(event_list_iterator->second.begin());
					

					// Search the event's listener list for the listener.
					while ( event_list_listener_iterator != event_list_iterator->second.end() )
					{
						// If the listener is found.
						if ( (*event_list_listener_iterator) == listener_iterator->first )
						{
							// Remove the listener from the event's listener list.
							event_list_iterator->second.erase(event_list_listener_iterator);

							// If the event's listener list is empty, queue the event for removal.
							if ( event_list_iterator->second.size() == 0 )
								queue_event_removal((*event_iterator));

							// Exit the loop.
							break;
						}

						++event_list_listener_iterator;
					}
				}
			}

			// If the listener should be removed from the listener list, queue the listener for removal.
			if ( removable )
				queue_listener_removal(listener_iterator->first);
		}

		// The function that is used to perform any listener operations.
		void EventManager::perform_listener_operation( ListenerOperation& operation )
		{
			// Locate the listener in the listener list.
			std::map<Listener*,std::vector<EventCode> >::iterator listener_iterator(m_listener_list.find(operation.m_listener));


			// Depending on the requested operation perform the necessary actions.
			switch ( operation.m_operation )
			{
				// If we want to add an event notification.
				case AddEvent:

					// If the listener is not in the listener list.
					if ( listener_iterator == m_listener_list.end() )
					{
						// Add the listener to the list
						m_listener_list.insert(std::pair<Listener*,std::vector<EventCode> >(operation.m_listener,std::vector<EventCode>(0,0)));
						listener_iterator = m_listener_list.find(operation.m_listener);
					}

					// If the listener is in the listener list, register the listener for notification of the specified event.
					if ( listener_iterator != m_listener_list.end() )
						add_event(listener_iterator,operation.m_code);
					
					break;

				// If we want to add notification for all events.
				case AddAllEvents:

					// If the listener is not in the listener list.
					if ( listener_iterator == m_listener_list.end() )
					{
						// Add the listener to the list
						m_listener_list.insert(std::pair<Listener*,std::vector<EventCode> >(operation.m_listener,std::vector<EventCode>(0,0)));
						listener_iterator = m_listener_list.find(operation.m_listener);
					}
					else // If the listener is in the listener list.
						remove_all_events(listener_iterator,false);	// Unregister all events associated with the listener since we will be notifying for all events anyway.

					// If the listener is in the listener list, register the listener for notification of the virtual EVENT_ALL event.
					if ( listener_iterator != m_listener_list.end() )
						add_event(listener_iterator,operation.m_code);

					break;

				// If we want to remove notification for an event.
				case RemoveEvent:

					// Remove the event from the listener's list.
					if ( listener_iterator != m_listener_list.end() )
						remove_event(listener_iterator,operation.m_code);

					break;

				// If we want to remove notification for all events.
				case RemoveAllEvents:

					// Remove all events and the listeners from the lists.
					if ( listener_iterator != m_listener_list.end() )
						remove_all_events(listener_iterator,true);

					break;
			}
		}

		// The function that is used to perform the operation of the event manager.
		void EventManager::actual_operate()
		{
			/*
				A queue that is used to "buffer" the pending event list.
				By using this technique, more CPU power is utilized to copy the queue, however the
				size of the critical section that is needed is minimized.
			*/
			std::deque<Event*> pending_events(0);
			// A variable that is used to get the current time value.
			utility::TimerValueType current_time = 0;

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

				// Whether to sleep or not at the end of the function in order to avoid busy loops.
				bool sleep = false;

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


			m_lock.lock();
			// Get any pending events from the event queue and "buffer" them in the 
			pending_events.insert(pending_events.end(),m_event_queue.begin(),m_event_queue.end());
			// Clear the event queue.
			m_event_queue.clear();
			// Get the current time.
			current_time = m_timer.milliseconds();
			m_lock.unlock();

			// For all events in the pending queue
			for (
					std::deque<Event*>::iterator event_iterator = pending_events.begin();
					event_iterator != pending_events.end();
					++event_iterator
				)
			{
				m_lock.lock();

				// For all listeners in the pending operation queue.
				for (
						std::deque<ListenerOperation*>::iterator operation_iterator = m_pending_operation_queue.begin();
						operation_iterator != m_pending_operation_queue.end();
						++operation_iterator
					)
				{
					// Perform any pending listener operation.
					perform_listener_operation((*(*operation_iterator)));
					delete (*operation_iterator);
				}

				// Clear the pending operation queue.
				m_pending_operation_queue.clear();
				m_lock.unlock();


				// Find the virtual event EVENT_ALL in the event list that is used to notify the listeners that want to be notified for all events.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find(EVENT_ALL));
				
				
				// If there is an ALL_EVENT entry in the event list.
				if ( event_list_iterator != m_event_list.end() )
				{
					// For all the listeners in the event's listener list.
					for (
							std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
							event_list_listener_iterator != event_list_iterator->second.end();
							++event_list_listener_iterator
						)
					{
						// Call the on_event function of the listener.
						(*event_list_listener_iterator)->on_event((*(*event_iterator)));
					}
				}

				// Find the triggered event's entry in the entry list.
				event_list_iterator = m_event_list.find((*event_iterator)->code());

				// If the entry exists in the event list.
				if ( event_list_iterator != m_event_list.end() )
				{
					// For all the listeners in the event's listener list.
					for (
							std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
							event_list_listener_iterator != event_list_iterator->second.end();
							++event_list_listener_iterator
						)
					{
						// Call the on_event function of the listener.
						(*event_list_listener_iterator)->on_event((*(*event_iterator)));
					}
				}

				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				(*event_iterator)->cleanup();
				delete (*event_iterator);
			}

			m_lock.lock();

			// For all the periodic events.
			for (
					std::vector<PeriodicEventInfo*>::iterator event_iterator = m_periodic_event_list.begin();
					event_iterator != m_periodic_event_list.end();
					++event_iterator
				)
			{
				// Calculate the time difference between the current time and the last trigger of the vent.
				utility::TimerValueType difference = (current_time - (*event_iterator)->m_last_trigger);


				// If the time difference is greater than the period of the event, trigger the event.
				if ( difference >= (*event_iterator)->m_period )
				{
					// Find the virtual event EVENT_ALL in the event list that is used to notify the listeners that want to be notified for all events.
					std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(m_event_list.find(EVENT_ALL));
					// Get the first parameter of the event.
					const Parameter* parameter = (*event_iterator)->m_event.parameter(0);
				
					
					// Set the value of the first parameter of the event to the time that has passed since the last call.
					*(static_cast<utility::TimerValueType*>(parameter->data())) = difference;

					// For all listeners in the pending operation queue.
					for (
							std::deque<ListenerOperation*>::iterator operation_iterator = m_pending_operation_queue.begin();
							operation_iterator != m_pending_operation_queue.end();
							++operation_iterator
						)
					{
						// Perform any pending listener operation.
						perform_listener_operation((*(*operation_iterator)));
						delete (*operation_iterator);
					}
					
					// Clear the pending operation queue.
					m_pending_operation_queue.clear();


					// If there is an ALL_EVENT entry in the event list.
					if ( event_list_iterator != m_event_list.end() )
					{
						// For all the listeners in the event's listener list.
						for (
								std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
								event_list_listener_iterator != event_list_iterator->second.end();
								++event_list_listener_iterator
							)
						{
							// Call the on_event function of the listener.
							(*event_list_listener_iterator)->on_event((*event_iterator)->m_event);
						}
					}

					// Find the triggered event's entry in the entry list.
					event_list_iterator = m_event_list.find((*event_iterator)->m_event.code());

					// If the entry exists in the event list.
					if ( event_list_iterator != m_event_list.end() )
					{
						// For all the listeners in the event's listener list.
						for (
								std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
								event_list_listener_iterator != event_list_iterator->second.end();
								++event_list_listener_iterator
							)
						{
							// Call the on_event function of the listener.
							(*event_list_listener_iterator)->on_event((*event_iterator)->m_event);
						}
					}

					// Update the time the periodic event was last triggered.
					(*event_iterator)->m_last_trigger = current_time;
				}
			}

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

				// If no events were triggered, sleep for the desired time in order to avoid busy loops.
				if ( pending_events.size() == 0  && m_periodic_event_list.size() == 0 )
					sleep = true;

			#endif /* #ifndef ATHENA_EVENTMANAGER_SINGLETHREADED */


			// Perform any event and listener removals.
			perform_removals();
			m_lock.unlock();


			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED

				// If we should sleep
				if ( sleep )
				{
					// Sleep
					#ifdef _WIN32 
						Sleep(s_sleep_time);
					#elif __unix
						usleep(s_sleep_time);
					#else
						#error "Unsupported platform"
					#endif /* WIN32 */
				}

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
		}

		// A function responsible of performing cleanup.
		void EventManager::cleanup()
		{
			// For every periodic event.
			for (
					std::vector<PeriodicEventInfo*>::iterator periodic_event_iterator = m_periodic_event_list.begin();
					periodic_event_iterator != m_periodic_event_list.end();
					++periodic_event_iterator
				)
			{
				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				(*periodic_event_iterator)->m_event.cleanup();
				delete (*periodic_event_iterator);
			}

			m_periodic_event_list.clear();

			// For every event in the event queue.
			for ( 
					std::deque<Event*>::iterator event_iterator = m_event_queue.begin();
					event_iterator != m_event_queue.end();
					++event_iterator
				)
			{
				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				(*event_iterator)->cleanup();
				delete (*event_iterator);
			}

			m_event_queue.clear();

			// For every pending listener operation.
			for (
					std::deque<ListenerOperation*>::iterator operation_iterator = m_pending_operation_queue.begin();
					operation_iterator != m_pending_operation_queue.end();
					++operation_iterator
				)
			{
				delete (*operation_iterator);
			}

			m_pending_operation_queue.clear();
		}


		// A function responsible of initialising the single instance of the class.
		bool EventManager::init()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) EventManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void EventManager::deinit()
		{
			s_instance_lock.lock();

			if ( s_instance != NULL )
			{
				s_instance->terminate();
				delete s_instance;
				s_instance = NULL;
			}

			s_instance_lock.unlock();
		}


		// A function responsible of commencing the functionality of the event system.
		bool EventManager::startup()
		{
			bool return_value = false;


			m_initialisation_lock.lock();

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED

				// (Re)Start the timer.
				//m_timer.reset();
				m_timer.start();
				m_initialised = true;
				return_value = true;

			#else
				
				ThreadPool* thread_pool = ThreadPool::get();


				if ( thread_pool != NULL )
				{
					// Startup the thread.
					if ( thread_pool->add_task(thread_function,static_cast<void*>(this),thread_callback_function,static_cast<void*>(this)) )
					{
						// (Re)Start the timer.
						//m_timer.reset();
						m_timer.start();
						m_running = true;
						m_initialised = true;
						return_value = true;
					}
				}

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			m_initialisation_lock.unlock();


			return return_value;
		}

		// A function responsible of terminating the functionality of the event system.
		void EventManager::terminate()
		{
			m_initialisation_lock.lock();

			if ( m_initialised )
			{
				#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
				#else

					m_running = false;
					m_initialisation_lock.unlock();

					m_thread_mutex.lock();
					m_thread_condition_variable.wait(m_thread_mutex);
					m_thread_mutex.unlock();
					m_initialisation_lock.lock();

				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

				m_lock.lock();
				// Perform cleanup.
				cleanup();
				// Clear the lists and the queues.
				m_event_list.clear();
				m_listener_list.clear();
				m_initialised = false;
				m_lock.unlock();
			}

			m_initialisation_lock.unlock();
		}


		// A function responsible of returning a single instance of the class.
		EventManager* EventManager::get()
		{
			EventManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// A function responsible of triggering an event with the given parameters and id code.
		void EventManager::trigger_event( const Event& event )
		{
			// If the event is not the virtual EVENT_ALL event.
			if ( event.code() != EVENT_ALL )
			{
				bool initialised = false;


				m_initialisation_lock.lock();
				initialised = m_initialised;
				m_initialisation_lock.unlock();

				if ( initialised )
				{
					Event* new_event = new (std::nothrow) Event(event);


					if ( new_event != NULL )
					{
						m_lock.lock();
						// Queue the event for triggering.
						m_event_queue.push_back(new_event);
						m_lock.unlock();
					}
				}
			}
		}

		/*
			A function responsible of registering an event to be triggered periodically.
			The parameter at index 0 will be used to set the time that has passed since last trigger.
			If the parameter is not the proper type or if there are no parameter the event manager will 
			add the needed parameter.
		*/
		void EventManager::trigger_event_periodically( Event& event , const utility::TimerValueType& period )
		{
			// If the event is not the virtual EVENT_ALL event.
			if ( event.code() != EVENT_ALL )
			{
				bool initialised = false;


				m_initialisation_lock.lock();
				initialised = m_initialised;
				m_initialisation_lock.unlock();

				if ( initialised )
				{
					std::vector<PeriodicEventInfo*>::iterator periodic_event_iterator;


					m_lock.lock();
					periodic_event_iterator = m_periodic_event_list.begin();
			
					// Check if the event is already being triggered periodically.
					while ( periodic_event_iterator != m_periodic_event_list.end() )
					{
						// If the event is already being triggered.
						if ( (*periodic_event_iterator)->m_event.code() == event.code() )
						{
							// Update the period of the event to the newly given value.
							(*periodic_event_iterator)->m_period = period;
							// Exit the loop.
							break;
						}

						++periodic_event_iterator;
					}

					// If the event is not in the periodic event list.
					if ( periodic_event_iterator == m_periodic_event_list.end() )
					{
						bool done = false;


						// Check if we have at least one(1) parameter for the time difference value.
						if ( event.parameter_count() > 0 )
						{
							// Ger the first parameter.
							const Parameter* parameter = event.parameter(0);


							if ( parameter != NULL )
							{
								// If the parameter is properly allocated and of valid type, we can add the event to the periodic event list.
								if ( parameter->type() == DoubleReal  &&  parameter->data() != NULL )
									done = true;
							}
						}
						else // If the event does not have any parameters.
						{
							// Allocate a new variable to be used for the time difference value.
							double* value = new (std::nothrow) double;


							// If the allocation was successful.
							if ( value != NULL )
							{
								// Set the value to 0.
								(*value) = 0;
								// Add the parameter to the event.
								event.parameter(0,DoubleReal,value);

								if ( event.parameter_count() > 0 )
									done = true;
							}
						}

						// If we can add the event to the periodic event list.
						if ( done )
						{
							PeriodicEventInfo* event_info = new (std::nothrow) PeriodicEventInfo(event,period,0);


							if ( event_info != NULL )
								m_periodic_event_list.push_back(event_info); // Add the event to the periodic event list.
							else
								event.cleanup();
						}
						else // If we cannot add the event.
							event.cleanup(); // Call the cleanup function of the event.
					}

					m_lock.unlock();
				}
			}
		}

		// A function responsible of unregistering an event from being triggered periodically.
		void EventManager::stop_triggerring_event_periodically( const EventCode& code )
		{
			// If the event is not the virtual EVENT_ALL event.
			if ( code != EVENT_ALL )
			{
				bool initialised = false;


				m_initialisation_lock.lock();
				initialised = m_initialised;
				m_initialisation_lock.unlock();

				if ( initialised )
				{
					m_lock.lock();
			
					// Check the periodic event list.
					for (
							std::vector<PeriodicEventInfo*>::iterator periodic_event_iterator = m_periodic_event_list.begin();
							periodic_event_iterator != m_periodic_event_list.end();
							++periodic_event_iterator
						)
					{
						// If we find the event we are looking for.
						if ( (*periodic_event_iterator)->m_event.code() == code )
						{
							// Call the cleanup function of the event.
							(*periodic_event_iterator)->m_event.cleanup();
							delete (*periodic_event_iterator);
							// Remove the event from the periodic event list.
							m_periodic_event_list.erase(periodic_event_iterator);
							// Exit the loop.
							break;
						}
					}

					m_lock.unlock();
				}
			}
		}

		// A function responsible of registering a listener for notification of the specified event code.
		void EventManager::register_event( Listener* listener , const EventCode& code )
		{
			// If the event is not the virtual EVENT_ALL event.
			if ( code != EVENT_ALL )
			{
				bool initialised = false;


				m_initialisation_lock.lock();
				initialised = m_initialised;
				m_initialisation_lock.unlock();

				if ( initialised )
				{
					// Create the operation to be performed.
					ListenerOperation* operation = new (std::nothrow) ListenerOperation(AddEvent,code,listener);


					if ( operation != NULL )
					{
						m_lock.lock();
						// Add the operation to the operation queue.
						m_pending_operation_queue.push_back(operation);
						m_lock.unlock();
					}
				}
			}
		}

		/*
			A function responsible of registering all events for the listener.
			This function will unregister all previously registered events.
			Additionally, events cannot be unregistered on an individual basis, instead a call
			to unregister_all_events must be made.
		*/
		void EventManager::register_all_events( Listener* listener )
		{
			bool initialised = false;


			m_initialisation_lock.lock();
			initialised = m_initialised;
			m_initialisation_lock.unlock();

			if ( initialised )
			{
				// Create the operation to be performed. Notice the virtual EVENT_ALL event that is being used.
				ListenerOperation* operation = new (std::nothrow) ListenerOperation(AddAllEvents,EVENT_ALL,listener);


				if ( operation != NULL )
				{
					m_lock.lock();
					// Add the operation to the operation queue.
					m_pending_operation_queue.push_back(operation);
					m_lock.unlock();
				}
			}
		}
			
		// A function responsible of unregistering a listener from notification of the specified event code.
		void EventManager::unregister_event( Listener* listener , const EventCode& code )
		{
			bool initialised = false;


			m_initialisation_lock.lock();
			initialised = m_initialised;
			m_initialisation_lock.unlock();

			if ( initialised )
			{
				// If the event is not the virtual EVENT_ALL event.
				if ( code != EVENT_ALL )
				{
					// Create the operation to be performed.
					ListenerOperation* operation = new (std::nothrow) ListenerOperation(RemoveEvent,code,listener);


					if ( operation != NULL )
					{
						m_lock.lock();
						// Add the operation to the operation queue.
						m_pending_operation_queue.push_back(operation);
						m_lock.unlock();
					}
				}
			}
		}

		// A function responsible of unregistering a listener from notification of all events.
		void EventManager::unregister_all_events( Listener* listener )
		{
			bool initialised = false;


			m_initialisation_lock.lock();
			initialised = m_initialised;
			m_initialisation_lock.unlock();

			if ( initialised )
			{
				// Create the operation to be performed.
				ListenerOperation* operation = new (std::nothrow) ListenerOperation(RemoveAllEvents,EVENT_ALL,listener);


				if ( operation != NULL )
				{
					m_lock.lock();
					// Add the operation to the operation queue.
					m_pending_operation_queue.push_back(operation);
					m_lock.unlock();
				}
			}
		}

	} /* core */

} /* athena */