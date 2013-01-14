#ifndef ATHENA_CORE_PERIODICEVENTINFO_HPP
#define ATHENA_CORE_PERIODICEVENTINFO_HPP

#include "event.hpp"
#include "timer.hpp"



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
				Event m_event;
				// The period that is the event is being triggered.
				utility::TimerValueType m_period;
				// The time the event was last triggered.
				utility::TimerValueType m_last_trigger;


			public:

				// The constructor of the class.
				ATHENA_DLL PeriodicEventInfo();
				// A constructor for the class that performs initialisation.
				explicit ATHENA_DLL PeriodicEventInfo( Event& event , const utility::TimerValueType& period = 0 , const utility::TimerValueType& last_trigger = 0 );
				// The destructor of the class.
				ATHENA_DLL ~PeriodicEventInfo();
		};

	} /* core */

} /* athena */



#endif /* ATHENA_CORE_PERIODICEVENTINFO_HPP */