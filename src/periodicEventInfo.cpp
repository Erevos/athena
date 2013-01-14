#include "periodicEventInfo.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		PeriodicEventInfo::PeriodicEventInfo() : 
			m_event() , 
			m_period(0) , 
			m_last_trigger(0)
		{
		}

		// A constructor for the class that performs initialisation.
		PeriodicEventInfo::PeriodicEventInfo( Event& event , const utility::TimerValueType& period , const utility::TimerValueType& last_trigger ) : 
			m_event(event) , 
			m_period(period) , 
			m_last_trigger(last_trigger)
		{
		}
		
		// The destructor of the class.
		PeriodicEventInfo::~PeriodicEventInfo()
		{
		}

	} /* core */

} /* athena */