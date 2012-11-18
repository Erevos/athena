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
				const unsigned int _sleep_time = 1000;;
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
			_entity_list() , 
			_event_queue(0) , 
			_pending_operation_queue(0) , 
			_periodic_event_list(0) , 
			_pending_event_removals(0) , 
			_pending_entity_removals(0) , 
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
			_cleanup();
		};


		// A function responsible of queuing an event to be removed.
		void EventManager::_queue_entity_removal( Entity* entity )
		{
			std::vector<Entity*>::iterator entity_iterator(_pending_entity_removals.begin());


			while ( entity_iterator != _pending_entity_removals.end() )
			{
				if ( (*entity_iterator) == entity )
					break;

				++entity_iterator;
			}

			if  ( entity_iterator == _pending_entity_removals.end() )
				_pending_entity_removals.push_back(entity);
		};

		// A function responsible of queuing an event to be removed.
		void EventManager::_queue_event_removal( const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(_pending_event_removals.begin());


			while ( event_iterator != _pending_event_removals.end() )
			{
				if ( (*event_iterator) == code )
					break;

				++event_iterator;
			}

			if  ( event_iterator == _pending_event_removals.end() )
				_pending_event_removals.push_back(code);
		};

		// A function responsible of performing any queued removals.
		void EventManager::_perform_removals()
		{
			for (
					std::vector<Entity*>::iterator entity_iterator = _pending_entity_removals.begin();
					entity_iterator != _pending_entity_removals.end();
					++entity_iterator
				)
			{
				std::map<Entity*,std::vector<EventCode> >::iterator entity_list_iterator(_entity_list.find((*entity_iterator)));


				if ( entity_list_iterator != _entity_list.end() )
					_entity_list.erase(entity_list_iterator);
			}

			for (
					std::vector<EventCode>::iterator event_iterator = _pending_event_removals.begin();
					event_iterator != _pending_event_removals.end();
					++event_iterator
				)
			{
				std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find((*event_iterator)));


				if ( event_list_iterator != _event_list.end() )
					_event_list.erase(event_list_iterator);
			}

			_pending_entity_removals.clear();
			_pending_event_removals.clear();
		};

		// A function responsible of adding notification regarding a single event for an entity.
		void EventManager::_add_event( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator , const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(entity_iterator->second.begin());


			while ( event_iterator != entity_iterator->second.end() )
			{
				if ( (*event_iterator) == code )
					break;

				++event_iterator;
			}

			if ( event_iterator == entity_iterator->second.end() )
			{
				std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find(code));


				if ( event_list_iterator != _event_list.end() )
				{
					std::vector<Entity*>::iterator event_list_entity_iterator(event_list_iterator->second.begin());
					

					while ( event_list_entity_iterator != event_list_iterator->second.end() )
					{
						if ( (*event_list_entity_iterator) == entity_iterator->first )
							break;

						++event_list_entity_iterator;
					}

					if ( event_list_entity_iterator == event_list_iterator->second.end() )
						event_list_iterator->second.push_back(entity_iterator->first);
				}
				else
					_event_list.insert(std::pair<EventCode,std::vector<Entity*> >(code,std::vector<Entity*>(1,entity_iterator->first)));

				entity_iterator->second.push_back(code);
			}
		};

		// A function responsible of removing notification regarding a single event for an entity.
		void EventManager::_remove_event( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator , const EventCode& code )
		{
			std::vector<EventCode>::iterator event_iterator(entity_iterator->second.begin());


			while ( event_iterator != entity_iterator->second.end() )
			{
				if ( (*event_iterator) == code )
				{
					std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find(code));


					if ( event_list_iterator != _event_list.end() )
					{
						std::vector<Entity*>::iterator event_list_entity_iterator(event_list_iterator->second.begin());
					

						while ( event_list_entity_iterator != event_list_iterator->second.end() )
						{
							if ( (*event_list_entity_iterator) == entity_iterator->first )
							{
								event_list_iterator->second.erase(event_list_entity_iterator);

								if ( event_list_iterator->second.size() == 0 )
									_queue_event_removal(code);

								break;
							}

							++event_list_entity_iterator;
						}
					}

					entity_iterator->second.erase(event_iterator);
					break;
				}

				++event_iterator;
			}

			if ( entity_iterator->second.size() == 0 )
				_queue_entity_removal(entity_iterator->first);
		};

		// A function responsible of removing notification regarding all events for an entity.
		void EventManager::_remove_all_events( std::map<Entity*,std::vector<EventCode> >::iterator& entity_iterator )
		{
			for (
					std::vector<EventCode>::iterator event_iterator(entity_iterator->second.begin());
					event_iterator != entity_iterator->second.end();
					++event_iterator
				)
			{
				std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find((*event_iterator)));


				if ( event_list_iterator != _event_list.end() )
				{
					std::vector<Entity*>::iterator event_list_entity_iterator(event_list_iterator->second.begin());
					

					while ( event_list_entity_iterator != event_list_iterator->second.end() )
					{
						if ( (*event_list_entity_iterator) == entity_iterator->first )
						{
							event_list_iterator->second.erase(event_list_entity_iterator);

							if ( event_list_iterator->second.size() == 0 )
								_queue_event_removal((*event_iterator));

							break;
						}

						++event_list_entity_iterator;
					}
				}
			}

			if ( entity_iterator->second.size() == 0 )
				_queue_entity_removal(entity_iterator->first);
		};

		// The function that is used to perform any entity operations.
		void EventManager::_perform_entity_operation( EntityOperation& operation )
		{
			std::map<Entity*,std::vector<EventCode> >::iterator entity_iterator(_entity_list.find(operation.entity));


			switch ( operation.operation )
			{
				case AddEvent:

					if ( entity_iterator == _entity_list.end() )
					{
						_entity_list.insert(std::pair<Entity*,std::vector<EventCode> >(operation.entity,std::vector<EventCode>(0,0)));
						entity_iterator = _entity_list.find(operation.entity);
					}

					if ( entity_iterator != _entity_list.end() )
						_add_event(entity_iterator,operation.code);
					

					break;

				case RemoveEvent:

					if ( entity_iterator != _entity_list.end() )
						_remove_event(entity_iterator,operation.code);

					break;

				case RemoveAllEvents:

					if ( entity_iterator != _entity_list.end() )
						_remove_all_events(entity_iterator);

					break;
			}
		};

		// The function that is used to perform the operation of the event manager.
		void EventManager::_operate()
		{
			std::deque<Event> pending_events(0);
			utility::TimerValueType current_time = 0;
			bool sleep = false;


			_lock.lock(false);
			pending_events.insert(pending_events.end(),_event_queue.begin(),_event_queue.end());
			_event_queue.clear();
			current_time = _timer.milliseconds();
			_lock.unlock();

			for (
					std::deque<Event>::iterator event_iterator = pending_events.begin();
					event_iterator != pending_events.end();
					++event_iterator
				)
			{
				_lock.lock(false);

				for (
						std::deque<EntityOperation>::iterator operation_iterator = _pending_operation_queue.begin();
						operation_iterator != _pending_operation_queue.end();
						++operation_iterator
					)
				{
					_perform_entity_operation((*operation_iterator));
				}

				_pending_operation_queue.clear();
				_lock.unlock();


				std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find(event_iterator->code()));


				if ( event_list_iterator != _event_list.end() )
				{
					for (
							std::vector<Entity*>::iterator event_list_entity_iterator = event_list_iterator->second.begin();
							event_list_entity_iterator != event_list_iterator->second.end();
							++event_list_entity_iterator
						)
					{
						(*event_list_entity_iterator)->on_event((*event_iterator));
					}
				}

				event_iterator->cleanup();
			}

			_lock.lock(false);

			for (
					std::vector<PeriodicEventInfo>::iterator event_iterator = _periodic_event_list.begin();
					event_iterator != _periodic_event_list.end();
					++event_iterator
				)
			{
				utility::TimerValueType difference = (current_time - event_iterator->_last_trigger);


				if ( difference >= event_iterator->_period )
				{
					std::map<EventCode,std::vector<Entity*> >::iterator event_list_iterator(_event_list.find(event_iterator->_event.code()));
					Parameter parameter(event_iterator->_event.parameter(0));
				
					
					*(static_cast<utility::TimerValueType*>(parameter.data())) = difference;

					for (
							std::deque<EntityOperation>::iterator operation_iterator = _pending_operation_queue.begin();
							operation_iterator != _pending_operation_queue.end();
							++operation_iterator
						)
					{
						_perform_entity_operation((*operation_iterator));
					}
					
					_pending_operation_queue.clear();

					if ( event_list_iterator != _event_list.end() )
					{
						for (
								std::vector<Entity*>::iterator event_list_entity_iterator = event_list_iterator->second.begin();
								event_list_entity_iterator != event_list_iterator->second.end();
								++event_list_entity_iterator
							)
						{
							(*event_list_entity_iterator)->on_event(event_iterator->_event);
						}
					}

					event_iterator->_last_trigger = current_time;
				}
			}

			if ( pending_events.size() == 0  && _periodic_event_list.size() == 0 )
				sleep = true;

			_perform_removals();
			_lock.unlock();

			if ( sleep )
			{
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
			for (
					std::vector<PeriodicEventInfo>::iterator periodic_event_iterator = _periodic_event_list.begin();
					periodic_event_iterator != _periodic_event_list.end();
					++periodic_event_iterator
				)
			{
				periodic_event_iterator->_event.cleanup();
			}

			for ( 
					std::deque<Event>::iterator event_iterator = _event_queue.begin();
					event_iterator != _event_queue.end();
					++event_iterator 
				)
			{
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
					
				_thread.functionality(&_thread_function);
				_thread.parameter(static_cast<void*>(this));
					
				if ( !_thread.create() )
					return_value = false;

			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

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
				_thread.destroy();
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */

			_cleanup();
			_event_list.clear();
			_entity_list.clear();
			_pending_operation_queue.clear();
			_event_queue.clear();
			_periodic_event_list.clear();
			_initialisation_lock.unlock();
		};

		// A function responsible of triggering an event with the given parameters and id code.
		void EventManager::trigger_event( const Event& event )
		{
			_lock.lock(false);
			_event_queue.push_back(event);
			_lock.unlock();
		};

		/*
			A function responsible of registering an event to be triggered periodically.
			The parameter at index 0 will be used to set the time that has passed since last trigger.
			If the parameter is not the proper type or if there are no parameter the event manager will 
			add the needed parameter.
		*/
		void EventManager::triger_event_periodically( Event& event , const double& period )
		{
			std::vector<PeriodicEventInfo>::iterator periodic_event_iterator;


			_lock.lock(false);
			periodic_event_iterator = _periodic_event_list.begin();
			
			while ( periodic_event_iterator != _periodic_event_list.end() )
			{
				if ( periodic_event_iterator->_event.code() == event.code() )
				{
					periodic_event_iterator->_period = period;
					break;
				}

				++periodic_event_iterator;
			}

			if ( periodic_event_iterator == _periodic_event_list.end() )
			{
				bool done = false;


				if ( event.parameter_count() > 0 )
				{
					Parameter parameter(event.parameter(0));


					if ( parameter.type() == DoubleReal  &&  parameter.data() != INVALID_POINTER )
						done = true;
				}
				else
				{
					double* value = new (std::nothrow) double;


					if ( value != INVALID_POINTER )
					{
						(*value) = 0;
						event.parameter(0,DoubleReal,value);
						done = true;
					}
				}

				if ( done )
					_periodic_event_list.push_back(PeriodicEventInfo(event,period,0));
				else
					event.cleanup();
			}

			_lock.unlock();
		};

		// A function responsible of unregistering an event from being triggered periodically.
		void EventManager::stop_triggerring_event_periodically( const EventCode& code )
		{
			_lock.lock(false);
			
			for (
					std::vector<PeriodicEventInfo>::iterator periodic_event_iterator = _periodic_event_list.begin();
					periodic_event_iterator != _periodic_event_list.end();
					++periodic_event_iterator
				)
			{
				if ( periodic_event_iterator->_event.code() == code )
				{
					periodic_event_iterator->_event.cleanup();
					_periodic_event_list.erase(periodic_event_iterator);
					break;
				}
			}

			_lock.unlock();
		};

		// A function responsible of registering an entity for notification of the specified event code.
		void EventManager::register_event( Entity* entity , const EventCode& code )
		{
			EntityOperation operation = {AddEvent,code,entity};


			_lock.lock(false);

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				if ( _thread.is_running() )
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
				_pending_operation_queue.push_back(operation);
			
			_lock.unlock();
		};
			
		// A function responsible of unregistering an entity from notification of the specified event code.
		void EventManager::unregister_event( Entity* entity , const EventCode& code )
		{
			EntityOperation operation = {RemoveEvent,code,entity};


			_lock.lock(false);

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				if ( _thread.is_running() )
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
				_pending_operation_queue.push_back(operation);

			_lock.unlock();
		};

		// A function responsible of unregistering an entity from notification of all events.
		void EventManager::unregister_all_events( Entity* entity )
		{
			EntityOperation operation = {RemoveAllEvents,0,entity};


			_lock.lock(false);

			#ifndef ATHENA_EVENTMANAGER_SINGLETHREADED
				if ( _thread.is_running() )
			#endif /* ATHENA_EVENTMANAGER_SINGLETHREADED */
			
				_pending_operation_queue.push_back(operation);

			_lock.unlock();
		};

	} /* core */

} /* athena */