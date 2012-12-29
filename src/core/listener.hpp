#ifndef ATHENA_CORE_LISTENER_HPP
#define ATHENA_CORE_LISTENER_HPP

#include "../athenaDefinitions.hpp"
#include "event.hpp"



namespace athena
{

	namespace core
	{

		/* 
			A class representing the base entity upon which
			other engine entities can be constructed in order to 
			utilize the event system.
		*/
		class Listener
		{
			private:

				// A variable holding the id of the entity.
				unsigned int _id;


			public:

				// The constructor of the class.
				ATHENA_DLL explicit Listener( const unsigned int id = 0 );
				// The destructor of the class.
				ATHENA_DLL virtual ~Listener();


				// A function responsible of setting the id of the entity.
				ATHENA_DLL void id( const unsigned int value );
					

				// A function returning the id of the entity.
				ATHENA_DLL unsigned int id() const;


				// A function to be called when the entity is created.
				ATHENA_DLL virtual void on_create();
				// A function to be called when the entity is destroyed.
				ATHENA_DLL virtual void on_destroy();
				// A function to be called when an event occurs.
				ATHENA_DLL virtual void on_event( const Event& event );
				// A function responsible of registering an event for the entity.
				ATHENA_DLL void register_event( const EventCode& code );
				// A function responsible of unregistering an event for the entity.
				ATHENA_DLL void unregister_event( const EventCode& code );
				// A function responsible of unregistering all events for the entity.
				ATHENA_DLL void unregister_all_events();
		};

	} /* core */

} /* athena */


#include "listener.inl"



#endif /* ATHENA_CORE_LISTENER_HPP */