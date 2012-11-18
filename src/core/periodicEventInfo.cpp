#include "periodicEventInfo.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		PeriodicEventInfo::PeriodicEventInfo() : 
			_event() , 
			_period(0) , 
			_last_trigger(0)
		{
		};

		// A constructor for the class that performs initialisation.
		PeriodicEventInfo::PeriodicEventInfo( Event& event , const utility::TimerValueType& period , const utility::TimerValueType& last_trigger ) : 
			_event(event) , 
			_period(period) , 
			_last_trigger(last_trigger)
		{
		};
		
		// The destructor of the class.
		PeriodicEventInfo::~PeriodicEventInfo()
		{
		};

	} /* core */

} /* athena */