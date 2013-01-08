#ifndef ATHENA_HPP
#define ATHENA_HPP

#include "definitions.hpp"
#include "eventCodes.hpp"
#include "core/event.hpp"
#include "utility/timer.hpp"



namespace athena
{

	enum AthenaManagers
	{
		THREAD_POOL = 0x01 ,
		EVENT_MANAGER = 0x02 ,
		INPUT_MANAGER = 0x04 ,
		RENDER_MANAGER = 0x06 ,
		LOG_MANAGER = 0x08 ,
		ALL = 0xFFFFFFFF
	};

	// Function responsible of initialising the specified managers.
	ATHENA_DLL bool init( const AthenaManagers& managers = ALL );
	// Function responsible of starting the functionality of the specified managers.
	ATHENA_DLL bool startup( const AthenaManagers& managers = ALL );
	// Function responsible of deinitialising the specified managers.
	ATHENA_DLL void deinit( const AthenaManagers& managers = ALL );

	// The main loop of the engine.
	ATHENA_DLL void mainLoop();
	// A function that triggers the terminating sequence of the engine.
	ATHENA_DLL void terminate();

	// A function that triggers an event. Allias for the direct call from the manager.
	ATHENA_DLL void triger_event( const core::Event& event );
	// A function that registers an event for periodic triggering. Allias for the direct call from the manager.
	ATHENA_DLL void trigger_event_periodically( core::Event& event , const utility::TimerValueType& period );
	// A function that unregisters an event from periodic triggering. Allias for the direct call from the manager.
	ATHENA_DLL void stop_triggering_event_periodically( const core::EventCode& code );

} /* athena */



#endif /* ATHENA_HPP */