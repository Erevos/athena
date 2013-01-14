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
		RENDER_MANAGER = 0x04 ,
		AUDIO_MANAGER = 0x08 ,
		INPUT_MANAGER = 0x10 ,
		LOG_MANAGER = 0x20 ,
		ALL = 0xFFFFFFFF
	};

	// Function responsible of initialising the specified managers.
	ATHENA_DLL bool init( const AthenaManagers& managers , int& argc , char**& argv );
	// Function responsible of starting the functionality of the specified managers.
	ATHENA_DLL bool startup( const AthenaManagers& managers = ALL );
	// Function responsible of deinitialising the specified managers.
	ATHENA_DLL void deinit( const AthenaManagers& managers = ALL );

	// A function responsible of performing any update operations.
	ATHENA_DLL void operate();
	// A function that triggers the terminating sequence of the engine and deinitialising any managers.
	ATHENA_DLL void terminate();

	// A function that triggers an event. Allias for the direct call from the manager.
	ATHENA_DLL void trigger_event( const core::Event& event );
	// A function that registers an event for periodic triggering. Allias for the direct call from the manager.
	ATHENA_DLL void trigger_event_periodically( core::Event& event , const utility::TimerValueType& period );
	// A function that unregisters an event from periodic triggering. Allias for the direct call from the manager.
	ATHENA_DLL void stop_triggering_event_periodically( const core::EventCode& code );

} /* athena */



#endif /* ATHENA_HPP */