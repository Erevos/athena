#include "eventManager.hpp"



namespace athena
{
	
	namespace core
	{

		// The single instance of the class.
		EventManager* EventManager::_instance = INVALID_POINTER;
		// A lock used to handle concurrency issues regarding the instance of the class.
		utility::ReadersWriterLock EventManager::_instance_lock;

		#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
		#else
						
			/// The amount of time the spawned thread will sleep if there is nothing to do.
			#ifdef _WIN32
				const DWORD EventManager::_sleep_time = 1;
			#else
				const unsigned int EventManager::_sleep_time = 1000;
			#endif /* _WIN32 */

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		// TODO make exceptions
		// The static function that is used by the thread in order to perform the needed functionality.
		#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				
			utility::ThreadExitType EventManager::_thread_function( void* parameter )
			{
				EventManager* manager = static_cast<EventManager*>(parameter);
				utility::ThreadExitType return_value = 0;


				if ( manager != INVALID_POINTER )
				{
					try
					{
						while ( manager->_thread.is_running() )
							manager->_operate();
					}
					catch ( std::exception& e )
					{
						e;
						return_value = 1;
					}
					catch ( ... )
					{
						return_value = 2;
					}
				}


				return return_value;
			};

		#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */


		// The constructor of the class.
		EventManager::EventManager() : 
			_event_list() , 
			_listener_list() , 
			_event_queue(0) , 
			_pending_operation_queue(0) , 
			_periodic_event_list(0) , 
			_pending_event_removals(0) , 
			_pending_listener_removals(0) , 
			_lock() , 
			_initialisation_lock() ,
			_timer() ,

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
			#else
				_thread(INVALID_POINTER,INVALID_POINTER)
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

		{
		};

		// The destructor of the class.
		EventManager::~EventManager()
		{
			// Perform cleanup if it has not been done already.
			_cleanup();
		};


		// A function responsible of queuing an event to be removed.
		void EventManager::_queue_listener_removal( Listener* listener )
		{
			std::vector<Listener*>::iterator listener_iterator(_pending_listener_removals.begin());


			// Check the listener removal queue if the listener is already queued for removal.
			while ( listener_iterator != _pending_listener_removals.end() )
			{
				// If the listener is already queue stop the loop.
				if ( (*listener_iterator) == listener )
					break;

				++listener_iterator;
			}

			// If the listener is not queued for removal, add it to the queue.
			if  ( listener_iterator == _pending_listener_removals.end() )
				_pending_listener_removals.push_back(listener);
		};

		// A function responsible of queuing an event to be removed.
		void EventManager::_queue_event_removal( const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(_pending_event_removals.begin());


			// Check the event removal queue if the event is already queued for removal.
			while ( event_iterator != _pending_event_removals.end() )
			{
				// If the event is already queued for removal stop the loop.
				if ( (*event_iterator) == code )
					break;

				++event_iterator;
			}

			// If the event is not queued for removal, add it to the queue.
			if  ( event_iterator == _pending_event_removals.end() )
				_pending_event_removals.push_back(code);
		};

		// A function responsible of performing any queued removals.
		void EventManager::_perform_removals()
		{
			// For every listener in the listener removal queue.
			for (
					std::vector<Listener*>::iterator listener_iterator = _pending_listener_removals.begin();
					listener_iterator != _pending_listener_removals.end();
					++listener_iterator
				)
			{
				// Find the listener in the listener list.
				std::map<Listener*,std::vector<EventCode> >::iterator listener_list_iterator(_listener_list.find((*listener_iterator)));


				// If the listener exists in the list, remove it from the list.
				if ( listener_list_iterator != _listener_list.end() )
					_listener_list.erase(listener_list_iterator);
			}

			// For every event in the event removal queue.
			for (
					std::vector<EventCode>::iterator event_iterator = _pending_event_removals.begin();
					event_iterator != _pending_event_removals.end();
					++event_iterator
				)
			{
				// Find the event in the event list.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find((*event_iterator)));


				// If the event exists in the list, remove it from the list.
				if ( event_list_iterator != _event_list.end() )
					_event_list.erase(event_list_iterator);
			}

			// Clear the removal queues.
			_pending_listener_removals.clear();
			_pending_event_removals.clear();
		};

		// A function responsible of adding notification regarding a single event for a listener.
		void EventManager::_add_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code )
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
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find(code));


				// If the event has been added to the event list.
				if ( event_list_iterator != _event_list.end() )
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
					_event_list.insert(std::pair<EventCode,std::vector<Listener*> >(code,std::vector<Listener*>(1,listener_iterator->first)));

				// Add the event to the listener's event list.
				listener_iterator->second.push_back(code);
			}
		};

		// A function responsible of removing notification regarding a single event for a listener.
		void EventManager::_remove_event( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(listener_iterator->second.begin());


			// Check if the event exists in the listener's event list.
			while ( event_iterator != listener_iterator->second.end() )
			{
				// If the event exists in the list.
				if ( (*event_iterator) == code )
				{
					// Find the event in the event list.
					std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find(code));


					// If the event exists in the event list.
					if ( event_list_iterator != _event_list.end() )
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
									_queue_event_removal(code);

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
				_queue_listener_removal(listener_iterator->first);
		};

		// A function responsible of removing notification regarding all events for a listener.
		void EventManager::_remove_all_events( std::map<Listener*,std::vector<EventCode> >::iterator& listener_iterator , const bool removable )
		{
			// For all the events in the listener's event list.
			for (
					std::vector<EventCode>::iterator event_iterator(listener_iterator->second.begin());
					event_iterator != listener_iterator->second.end();
					++event_iterator
				)
			{
				// Find the event in the event list.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find((*event_iterator)));


				// If the event is found.
				if ( event_list_iterator != _event_list.end() )
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
								_queue_event_removal((*event_iterator));

							// Exit the loop.
							break;
						}

						++event_list_listener_iterator;
					}
				}
			}

			// If the listener should be removed from the listener list, queue the listener for removal.
			if ( removable )
				_queue_listener_removal(listener_iterator->first);
		};

		// The function that is used to perform any listener operations.
		void EventManager::_perform_listener_operation( ListenerOperation& operation )
		{
			// Locate the listener in the listener list.
			std::map<Listener*,std::vector<EventCode> >::iterator listener_iterator(_listener_list.find(operation.listener));


			// Depending on the requested operation perform the necessary actions.
			switch ( operation.operation )
			{
				// If we want to add an event notification.
				case AddEvent:

					// If the listener is not in the listener list.
					if ( listener_iterator == _listener_list.end() )
					{
						// Add the listener to the list
						_listener_list.insert(std::pair<Listener*,std::vector<EventCode> >(operation.listener,std::vector<EventCode>(0,0)));
						listener_iterator = _listener_list.find(operation.listener);
					}

					// If the listener is in the listener list, register the listener for notification of the specified event.
					if ( listener_iterator != _listener_list.end() )
						_add_event(listener_iterator,operation.code);
					
					break;

				// If we want to add notification for all events.
				case AddAllEvents:

					// If the listener is not in the listener list.
					if ( listener_iterator == _listener_list.end() )
					{
						// Add the listener to the list
						_listener_list.insert(std::pair<Listener*,std::vector<EventCode> >(operation.listener,std::vector<EventCode>(0,0)));
						listener_iterator = _listener_list.find(operation.listener);
					}
					else // If the listener is in the listener list.
						_remove_all_events(listener_iterator,false);	// Unregister all events associated with the listener since we will be notifying for all events anyway.

					// If the listener is in the listener list, register the listener for notification of the virtual ALL_EVENTS event.
					if ( listener_iterator != _listener_list.end() )
						_add_event(listener_iterator,operation.code);

					break;

				// If we want to remove notification for an event.
				case RemoveEvent:

					// Remove the event from the listener's list.
					if ( listener_iterator != _listener_list.end() )
						_remove_event(listener_iterator,operation.code);

					break;

				// If we want to remove notification for all events.
				case RemoveAllEvents:

					// Remove all events and the listeners from the lists.
					if ( listener_iterator != _listener_list.end() )
						_remove_all_events(listener_iterator,true);

					break;
			}
		};

		// The function that is used to perform the operation of the event manager.
		void EventManager::_operate()
		{
			/*
				A queue that is used to "buffer" the pending event list.
				By using this technique, mroe CPU power is utilized to copy the queue, however the
				size of the critical section that is needed is minimized.
			*/
			std::deque<Event> pending_events(0);
			// A variable that is used to get the current time value.
			utility::TimerValueType current_time = 0;
			// Whether to sleep or not at the end of the function in order to avoid busy loops.
			bool sleep = false;


			_lock.lock(false);
			// Get any pending events from the event queue and "buffer" them in the 
			pending_events.insert(pending_events.end(),_event_queue.begin(),_event_queue.end());
			// Clear the event queue.
			_event_queue.clear();
			// Get the current time.
			current_time = _timer.milliseconds();
			_lock.unlock();

			// For all events in the pending queue
			for (
					std::deque<Event>::iterator event_iterator = pending_events.begin();
					event_iterator != pending_events.end();
					++event_iterator
				)
			{
				_lock.lock(false);

				// For all listeners in the pending operation queue.
				for (
						std::deque<ListenerOperation>::iterator operation_iterator = _pending_operation_queue.begin();
						operation_iterator != _pending_operation_queue.end();
						++operation_iterator
					)
				{
					// Perform any pending listener operation.
					_perform_listener_operation((*operation_iterator));
				}

				// Clear the pending operation queue.
				_pending_operation_queue.clear();
				_lock.unlock();


				// Find the virtual event ALL_EVENTS in the event list that is used to notify the listeners that want to be notified for all events.
				std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find(ALL_EVENTS));
				
				
				// If there is an ALL_EVENT entry in the event list.
				if ( event_list_iterator != _event_list.end() )
				{
					// For all the listeners in the event's listener list.
					for (
							std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
							event_list_listener_iterator != event_list_iterator->second.end();
							++event_list_listener_iterator
						)
					{
						// Call the on_event function of the listener.
						(*event_list_listener_iterator)->on_event((*event_iterator));
					}
				}

				// Find the triggered event's entry in the entry list.
				event_list_iterator = _event_list.find(event_iterator->code());

				// If the entry exists in the event list.
				if ( event_list_iterator != _event_list.end() )
				{
					// For all the listeners in the event's listener list.
					for (
							std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
							event_list_listener_iterator != event_list_iterator->second.end();
							++event_list_listener_iterator
						)
					{
						// Call the on_event function of the listener.
						(*event_list_listener_iterator)->on_event((*event_iterator));
					}
				}

				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				event_iterator->cleanup();
			}

			_lock.lock(false);

			// For all the periodic events.
			for (
					std::vector<PeriodicEventInfo>::iterator event_iterator = _periodic_event_list.begin();
					event_iterator != _periodic_event_list.end();
					++event_iterator
				)
			{
				// Calculate the time difference between the current time and the last trigger of the vent.
				utility::TimerValueType difference = (current_time - event_iterator->_last_trigger);


				// If the time difference is greater than the period of the event, trigger the event.
				if ( difference >= event_iterator->_period )
				{
					// Find the virtual event ALL_EVENTS in the event list that is used to notify the listeners that want to be notified for all events.
					std::map<EventCode,std::vector<Listener*> >::iterator event_list_iterator(_event_list.find(ALL_EVENTS));
					// Get the first parameter of the event.
					Parameter parameter(event_iterator->_event.parameter(0));
				
					
					// Set the value of the first parameter of the event to the time that has passed since the last call.
					*(static_cast<utility::TimerValueType*>(parameter.data())) = difference;

					// For all listeners in the pending operation queue.
					for (
							std::deque<ListenerOperation>::iterator operation_iterator = _pending_operation_queue.begin();
							operation_iterator != _pending_operation_queue.end();
							++operation_iterator
						)
					{
						// Perform any pending listener operation.
						_perform_listener_operation((*operation_iterator));
					}
					
					// Clear the pending operation queue.
					_pending_operation_queue.clear();


					// If there is an ALL_EVENT entry in the event list.
					if ( event_list_iterator != _event_list.end() )
					{
						// For all the listeners in the event's listener list.
						for (
								std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
								event_list_listener_iterator != event_list_iterator->second.end();
								++event_list_listener_iterator
							)
						{
							// Call the on_event function of the listener.
							(*event_list_listener_iterator)->on_event(event_iterator->_event);
						}
					}

					// Find the triggered event's entry in the entry list.
					event_list_iterator = _event_list.find(event_iterator->_event.code());

					// If the entry exists in the event list.
					if ( event_list_iterator != _event_list.end() )
					{
						// For all the listeners in the event's listener list.
						for (
								std::vector<Listener*>::iterator event_list_listener_iterator = event_list_iterator->second.begin();
								event_list_listener_iterator != event_list_iterator->second.end();
								++event_list_listener_iterator
							)
						{
							// Call the on_event function of the listener.
							(*event_list_listener_iterator)->on_event(event_iterator->_event);
						}
					}

					// Update the time the periodic event was last triggered.
					event_iterator->_last_trigger = current_time;
				}
			}

			// If no events were triggered, sleep for the desired time in order to avoid busy loops.
			if ( pending_events.size() == 0  && _periodic_event_list.size() == 0 )
				sleep = true;

			// Perform any event and listener removals.
			_perform_removals();
			_lock.unlock();

			// If we should sleep
			if ( sleep )
			{
				// Sleep
				#ifdef _WIN32 
					Sleep(_sleep_time);
				#else
					usleep(_sleep_time);
				#endif /* WIN32 */
			}
		};

		// A function responsible of performing cleanup.
		void EventManager::_cleanup()
		{
			// For every periodic event.
			for (
					std::vector<PeriodicEventInfo>::iterator periodic_event_iterator = _periodic_event_list.begin();
					periodic_event_iterator != _periodic_event_list.end();
					++periodic_event_iterator
				)
			{
				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				periodic_event_iterator->_event.cleanup();
			}

			// For every event in the event queue.
			for ( 
					std::deque<Event>::iterator event_iterator = _event_queue.begin();
					event_iterator != _event_queue.end();
					++event_iterator 
				)
			{
				// Call the specified callback function that is responsible of performing cleanup for the parameters of the event.
				event_iterator->cleanup();
			}
		};


		// A function responsible of initialising the single instance of the class.
		bool EventManager::initialise()
		{
			bool return_value = true;


			_instance_lock.lock(false);

			if ( _instance == INVALID_POINTER )
			{
				_instance = new (std::nothrow) EventManager();
				return_value = ( _instance != INVALID_POINTER );
			}

			_instance_lock.unlock();


			return return_value;
		};

		// A function responsible of deinitialising the single instance of the class.
		void EventManager::deinitialise()
		{
			_instance_lock.lock(false);

			if ( _instance != INVALID_POINTER )
			{
				_instance->terminate();
				delete _instance;
				_instance = INVALID_POINTER;
			}

			_instance_lock.unlock();
		};

		// A function responsible of returning a single instance of the class.
		EventManager* EventManager::get()
		{
			EventManager* return_value = INVALID_POINTER;


			_instance_lock.lock(true);
			return_value = _instance;
			_instance_lock.unlock();


			return return_value;
		};

		
		// A function responsible of commencing the functionality of the event system.
		bool EventManager::startup()
		{
			bool return_value = true;


			_initialisation_lock.lock(false);

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
			#else
					
				// Specify the functionality of the thread that is spawned.
				_thread.functionality(&_thread_function);
				// Make the manager available to the thread.
				_thread.parameter(static_cast<void*>(this));
					
				// Startup the thread.
				if ( !_thread.create() )
					return_value = false;

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			// (Re)Start the timer.
			_timer.start();
			_initialisation_lock.unlock();


			return return_value;
		};

		// A function responsible of terminating the functionality of the event system.
		void EventManager::terminate()
		{
			_initialisation_lock.lock(false);

			#ifdef ATHENA_EVENTMANAGER_SINGLETHREADED
			#else
				// Destroy the thread.
				_thread.destroy();
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			// Perform cleanup.
			_cleanup();
			// Clear the lists and the queues.
			_event_list.clear();
			_listener_list.clear();
			_pending_operation_queue.clear();
			_event_queue.clear();
			_periodic_event_list.clear();
			_initialisation_lock.unlock();
		};

		// A function responsible of triggering an event with the given parameters and id code.
		void EventManager::trigger_event( const Event& event )
		{
			// If the event is not the virtual ALL_EVENTS event.
			if ( event.code() != ALL_EVENTS )
			{
				_lock.lock(false);
				// Queue the event for triggering.
				_event_queue.push_back(event);
				_lock.unlock();
			}
		};

		/*
			A function responsible of registering an event to be triggered periodically.
			The parameter at index 0 will be used to set the time that has passed since last trigger.
			If the parameter is not the proper type or if there are no parameter the event manager will 
			add the needed parameter.
		*/
		void EventManager::triger_event_periodically( Event& event , const double& period )
		{
			// If the event is not the virtual ALL_EVENTS event.
			if ( event.code() != ALL_EVENTS )
			{
				std::vector<PeriodicEventInfo>::iterator periodic_event_iterator;


				_lock.lock(false);
				periodic_event_iterator = _periodic_event_list.begin();
			
				// Check if the event is already being triggered periodically.
				while ( periodic_event_iterator != _periodic_event_list.end() )
				{
					// If the event is already being triggered.
					if ( periodic_event_iterator->_event.code() == event.code() )
					{
						// Update the period of the event to the newly given value.
						periodic_event_iterator->_period = period;
						// Exit the loop.
						break;
					}

					++periodic_event_iterator;
				}

				// If the event is not in the periodic event list.
				if ( periodic_event_iterator == _periodic_event_list.end() )
				{
					bool done = false;


					// Check if we have at least one(1) parameter for the time difference value.
					if ( event.parameter_count() > 0 )
					{
						// Ger the first parameter.
						Parameter parameter(event.parameter(0));


						// If the parameter is properly allocated and of valid type, we can add the event to the periodic event list.
						if ( parameter.type() == DoubleReal  &&  parameter.data() != INVALID_POINTER )
							done = true;
					}
					else // If the event does not have any parameters.
					{
						// Allocate a new variable to be used for the time difference value.
						double* value = new (std::nothrow) double;


						// If the allocation was successful.
						if ( value != INVALID_POINTER )
						{
							// Set the value to 0.
							(*value) = 0;
							// Add the parameter to the event.
							event.parameter(0,DoubleReal,value);
							done = true;
						}
					}

					// If we can add the event to the periodic event list.
					if ( done )
						_periodic_event_list.push_back(PeriodicEventInfo(event,period,0)); // Add the event to the periodic event list.
					else // If we cannot add the event.
						event.cleanup(); // Call the cleanup function of the event.
				}

				_lock.unlock();
			}
		};

		// A function responsible of unregistering an event from being triggered periodically.
		void EventManager::stop_triggerring_event_periodically( const EventCode& code )
		{
			// If the event is not the virtual ALL_EVENTS event.
			if ( code != ALL_EVENTS )
			{
				_lock.lock(false);
			
				// Check the periodic event list.
				for (
						std::vector<PeriodicEventInfo>::iterator periodic_event_iterator = _periodic_event_list.begin();
						periodic_event_iterator != _periodic_event_list.end();
						++periodic_event_iterator
					)
				{
					// If we find the event we are looking for.
					if ( periodic_event_iterator->_event.code() == code )
					{
						// Call the cleanup function of the event.
						periodic_event_iterator->_event.cleanup();
						// Remove the event from the periodic event list.
						_periodic_event_list.erase(periodic_event_iterator);
						// Exit the loop.
						break;
					}
				}

				_lock.unlock();
			}
		};

		// A function responsible of registering a listener for notification of the specified event code.
		void EventManager::register_event( Listener* listener , const EventCode& code )
		{
			// If the event is not the virtual ALL_EVENTS event.
			if ( code != ALL_EVENTS )
			{
				// Create the operation to be performed.
				ListenerOperation operation = {AddEvent,code,listener};


				_lock.lock(false);

				#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
					// If the manager is oeprational.
					if ( _thread.is_running() )
				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
					_pending_operation_queue.push_back(operation);
			
				_lock.unlock();
			}
		};

		/*
			A function responsible of registering all events for the listener.
			This function will unregister all previously registered events.
			Additionally, events cannot be unregistered on an individual basis, instead a call
			to unregister_all_events must be made.
		*/
		void EventManager::register_all_events( Listener* listener )
		{
			// Create the operation to be performed. Notice the virtual ALL_EVENTS event that is being used.
			ListenerOperation operation = {AddAllEvents,ALL_EVENTS,listener};


			_lock.lock(false);

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				// If the manager is oeprational.
				if ( _thread.is_running() )
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
				// Add the operation to the operation queue.
				_pending_operation_queue.push_back(operation);
			
			_lock.unlock();
		};
			
		// A function responsible of unregistering a listener from notification of the specified event code.
		void EventManager::unregister_event( Listener* listener , const EventCode& code )
		{
			// If the event is not the virtual ALL_EVENTS event.
			if ( code != ALL_EVENTS )
			{
				// Create the operation to be performed.
				ListenerOperation operation = {RemoveEvent,code,listener};


				_lock.lock(false);

				#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
					// If the manager is oeprational.
					if ( _thread.is_running() )
				#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
					// Add the operation to the operation queue.
					_pending_operation_queue.push_back(operation);

				_lock.unlock();
			}
		};

		// A function responsible of unregistering a listener from notification of all events.
		void EventManager::unregister_all_events( Listener* listener )
		{
			// Create the operation to be performed.
			ListenerOperation operation = {RemoveAllEvents,ALL_EVENTS,listener};


			_lock.lock(false);

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				// If the manager is oeprational.
				if ( _thread.is_running() )
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
				// Add the operation to the operation queue.
				_pending_operation_queue.push_back(operation);

			_lock.unlock();
		};

	} /* core */

} /* athena */