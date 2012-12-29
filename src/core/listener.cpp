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
		};

		// The destructor of the class.
		Listener::~Listener()
		{
		};


		// A function to be called when the entity is created.
		void Listener::on_create()
		{
		};

		// A function to be called when the entity is destroyed.
		void Listener::on_destroy()
		{
		};

		// A function to be called when an event occurs.
		void Listener::on_event( const Event& )
		{
		};

		// A function responsible of registering an event for the entity.
		void Listener::register_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->register_event(this,code);
		};

		// A function responsible of unregistering an event for the entity.
		void Listener::unregister_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_event(this,code);
		};

		// A function responsible of unregistering all events for the entity.
		void Listener::unregister_all_events()
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_all_events(this);
		};

	} /* core */

} /* athena */