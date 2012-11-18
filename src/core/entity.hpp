#ifndef ATHENA_CORE_ENTITY_HPP
	#define ATHENA_CORE_ENTITY_HPP

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
			class Entity
			{
				private:

					// A variable holding the id of the entity.
					unsigned int _id;


				public:

					// The constructor of the class.
					ATHENA_DLL explicit ATHENA_PRECALL Entity( const unsigned int id = 0 ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL virtual ATHENA_PRECALL ~Entity() ATHENA_POSTCALL;


					// A function responsible of setting the id of the entity.
					ATHENA_DLL void ATHENA_PRECALL id( const unsigned int value ) ATHENA_POSTCALL;
					

					// A function returning the id of the entity.
					ATHENA_DLL unsigned int ATHENA_PRECALL id() const ATHENA_POSTCALL;


					// A function to be called when the entity is created.
					ATHENA_DLL virtual void ATHENA_PRECALL on_create() ATHENA_POSTCALL;
					// A function to be called when the entity is destroyed.
					ATHENA_DLL virtual void ATHENA_PRECALL on_destroy() ATHENA_POSTCALL;
					// A function to be called when an event occurs.
					ATHENA_DLL virtual void ATHENA_PRECALL on_event( const Event& event ) ATHENA_POSTCALL;
					// A function responsible of registering an event for the entity.
					ATHENA_DLL void ATHENA_PRECALL register_event( const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of unregistering an event for the entity.
					ATHENA_DLL void ATHENA_PRECALL unregister_event( const EventCode& code ) ATHENA_POSTCALL;
					// A function responsible of unregistering all events for the entity.
					ATHENA_DLL void ATHENA_PRECALL unregister_all_events() ATHENA_POSTCALL;
			};

		} /* core */

	} /* athena */


	#include "entity.inl"



#endif /* ATHENA_CORE_ENTITY_HPP */