#ifndef ATHENA_CORE_EVENT_HPP
#define ATHENA_CORE_EVENT_HPP

#include "../athenaDefinitions.hpp"
#include "parameter.hpp"
#include <vector>



namespace athena
{

	namespace core
	{

		// Forward Declaration in order to perform the type definition.
		class Event;


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
				std::vector<Parameter> _parameters;
				// The id code of the event
				EventCode _code;
				// The function to be called in order to perform cleanup.
				EventCleanupFunction _cleanup_function;


			public:

				// The constructor of the class.
				ATHENA_DLL Event( const EventCode& id = 0 , const unsigned long& parameters = 0 );
				// The destructor of the class.
				ATHENA_DLL ~Event();


				// Function responsible of setting the parameter at the given index or adding a new parameter to the list.
				ATHENA_DLL void parameter( const unsigned int& index , const ParameterType type , void* data );
				// Function responsible of setting the id code of the event.
				ATHENA_DLL void code( const EventCode& value );
				// Function responsible of setting the cleanup function.
				ATHENA_DLL void cleanup_function( EventCleanupFunction function );
				// Function responsible of clearing the parameter list.
				ATHENA_DLL void clear();
				// Function responsible of performing cleanup.
				ATHENA_DLL void cleanup();


				// Function returning the parameter at the given index.
				ATHENA_DLL Parameter parameter( const unsigned int& index ) const;
				// Function returning the id code of the event.
				ATHENA_DLL EventCode code() const;
				// Function returning the cleanup function.
				ATHENA_DLL EventCleanupFunction cleanup_function() const;
				// Function returning the size of the parameter list.
				ATHENA_DLL unsigned int parameter_count() const;
		};

	} /* core */

} /* athena */


#include "event.inl"



#endif /* ATHENA_CORE_EVENT_HPP */