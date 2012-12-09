#include "entity.hpp"
#include "eventManager.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Entity::Entity( const unsigned int id ) : 
			_id(id)
		{
		};

		// The destructor of the class.
		Entity::~Entity()
		{
		};


		// A function to be called when the entity is created.
		void Entity::on_create()
		{
		};

		// A function to be called when the entity is destroyed.
		void Entity::on_destroy()
		{
		};

		// A function to be called when an event occurs.
		void Entity::on_event( const Event& )
		{
		};

		// A function responsible of registering an event for the entity.
		void Entity::register_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->register_event(this,code);
		};

		// A function responsible of unregistering an event for the entity.
		void Entity::unregister_event( const EventCode& code )
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_event(this,code);
		};

		// A function responsible of unregistering all events for the entity.
		void Entity::unregister_all_events()
		{
			EventManager* manager = EventManager::get();


			if ( manager != INVALID_POINTER )
				manager->unregister_all_events(this);
		};

	} /* core */

} /* athena */