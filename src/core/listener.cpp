#include "listener.hpp"
#include "eventManager.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Listener::Listener( const unsigned int id ) : 
			_id(id)
		{
		}

		// The destructor of the class.
		Listener::~Listener()
		{
		}


		// A function to be called when the listener is created.
		void Listener::on_create()
		{
		}

		// A function to be called when the listener is destroyed.
		void Listener::on_destroy()
		{
		}

		// A function to be called when an event occurs.
		void Listener::on_event( const Event& )
		{
		}

		// A function responsible of registering an event for the listener.
		void Listener::register_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->register_event(this,code);
		}

		/*
			A function responsible of registering all events for the listener.
			This function will unregister all previously registered events.
			Additionally, events cannot be unregistered on an individual basis, instead a call
			to unregister_all_events must be made.
		*/
		void Listener::register_all_events()
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->register_all_events(this);
		}

		// A function responsible of unregistering an event for the listener.
		void Listener::unregister_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_event(this,code);
		}

		// A function responsible of unregistering all events for the listener.
		void Listener::unregister_all_events()
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_all_events(this);
		}

	} /* core */

} /* athena */