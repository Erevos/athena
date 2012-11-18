#ifndef ATHENA_CORE_PERIODICEVENTINFO_HPP
	#define ATHENA_CORE_PERIODICEVENTINFO_HPP

	#include "event.hpp"
	#include "../utility/timer.hpp"



	namespace athena
	{

		namespace core
		{
			
			/*
				An utility class that is representing and handling the information of an periodic event.
			*/
			class PeriodicEventInfo
			{
				public:

					// The event that is being triggered periodically.
					Event _event;
					// The period that is the event is being triggered.
					utility::TimerValueType _period;
					// The time the event was last triggered.
					utility::TimerValueType _last_trigger;


				public:

					// The constructor of the class.
					ATHENA_DLL ATHENA_PRECALL PeriodicEventInfo() ATHENA_POSTCALL;
					// A constructor for the class that performs initialisation.
					explicit ATHENA_DLL ATHENA_PRECALL PeriodicEventInfo( Event& event , const utility::TimerValueType& period = 0 , const utility::TimerValueType& last_trigger = 0 ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_POSTCALL ~PeriodicEventInfo() ATHENA_POSTCALL;
			};

		} /* core */

	} /* athena */



#endif /* ATHENA_CORE_PERIODICEVENTINFO_HPP */