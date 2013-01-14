#ifndef ATHENA_CORE_EVENT_HPP
#define ATHENA_CORE_EVENT_HPP

#include "definitions.hpp"
#include <vector>
#include "parameter.hpp"



namespace athena
{

	namespace core
	{

		// Forward Declaration in order to perform the type definition.
		class Event;


		// A type definition that defines the type of the ID of the listeners.
		typedef unsigned int ListenerIDType;
		// A type definition that defines the type of the event code;
		typedef unsigned int EventCode;
		// A definition that defines the function to be used for cleanup.
		typedef void (*EventCleanupFunction)( const Event& event );


		/*
			A class representing and handling an event of the event system.
		*/
		class Event
		{
			private:

				// The list of the parameters.
				std::vector<Parameter*> m_parameters;
				// The id code of the event. A valid code id is greater than 0. 0 is reserved for the EVENT_ALL virtual event.
				EventCode m_code;
				// The id of the triggering entity.
				ListenerIDType m_initiator_id;
				// The function to be called in order to perform cleanup.
				EventCleanupFunction m_cleanup_function;


			public:

				// The constructor of the class.
				ATHENA_DLL explicit Event( const EventCode& id = 0 , const ListenerIDType& initiator_id = 0 );
				// The copy constructor of the class.
				ATHENA_DLL Event( const Event& event );
				// The destructor of the class.
				ATHENA_DLL ~Event();

				// The assignment operator.
				ATHENA_DLL Event& operator=( const Event& event );


				// Function responsible of setting the parameter at the given index or adding a new parameter to the list.
				ATHENA_DLL void parameter( const unsigned int& index , const ParameterType type , void* data );
				// Function responsible of setting the id code of the event.
				ATHENA_DLL void code( const EventCode& value );
				// Function responsible of setting the id code of the triggering entity.
				ATHENA_DLL void initiator_id( const ListenerIDType& value );
				// Function responsible of setting the cleanup function.
				ATHENA_DLL void cleanup_function( EventCleanupFunction function );
				// Function responsible of clearing the parameter list.
				ATHENA_DLL void clear();
				// Function responsible of performing cleanup.
				ATHENA_DLL void cleanup();


				// Function returning the parameter at the given index.
				ATHENA_DLL const Parameter* parameter( const unsigned int& index ) const;
				// Function returning the id code of the event.
				ATHENA_DLL EventCode code() const;
				// Function returning the id of the triggering entity.
				ATHENA_DLL ListenerIDType initiator_id() const;
				// Function returning the cleanup function.
				ATHENA_DLL EventCleanupFunction cleanup_function() const;
				// Function returning the size of the parameter list.
				ATHENA_DLL unsigned int parameter_count() const;
		};

	} /* core */

} /* athena */


#include "event.inl"



#endif /* ATHENA_CORE_EVENT_HPP */