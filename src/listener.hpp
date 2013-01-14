#ifndef ATHENA_CORE_LISTENER_HPP
#define ATHENA_CORE_LISTENER_HPP

#include "definitions.hpp"
#include "event.hpp"



namespace athena
{

	namespace core
	{
		/* 
			A class representing the base listener upon which
			other engine entities can be constructed in order to 
			utilize the event system.
		*/
		class Listener
		{
			private:

				// A variable holding the id of the listener.
				ListenerIDType m_id;


			public:

				// The constructor of the class.
				ATHENA_DLL explicit Listener( const ListenerIDType& id = 0 );
				// The destructor of the class.
				ATHENA_DLL virtual ~Listener();


				// A function responsible of setting the id of the listener.
				ATHENA_DLL void id( const ListenerIDType& value );
					

				// A function returning the id of the listener.
				ATHENA_DLL ListenerIDType id() const;


				// A function to be called when the listener is created.
				ATHENA_DLL virtual void on_create();
				// A function to be called when the listener is destroyed.
				ATHENA_DLL virtual void on_destroy();
				// A function to be called when an event occurs.
				ATHENA_DLL virtual void on_event( const Event& event );
				// A function responsible of registering an event for the listener.
				ATHENA_DLL void register_event( const EventCode& code );
				/*
					A function responsible of registering all events for the listener.
					This function will unregister all previously registered events.
					Additionally, events cannot be unregistered on an individual basis, instead a call
					to unregister_all_events must be made.
				*/
				ATHENA_DLL void register_all_events();
				// A function responsible of unregistering an event for the listener.
				ATHENA_DLL void unregister_event( const EventCode& code );
				// A function responsible of unregistering all events for the listener.
				ATHENA_DLL void unregister_all_events();
		};

	} /* core */

} /* athena */


#include "listener.inl"



#endif /* ATHENA_CORE_LISTENER_HPP */