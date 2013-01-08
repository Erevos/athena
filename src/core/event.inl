#ifndef ATHENA_CORE_EVENT_INL
#define ATHENA_CORE_EVENT_INL

#ifndef ATHENA_CORE_EVENT_HPP
	#error "event.hpp must be included before event.inl"
#endif /* ATHENA_CORE_EVENT_HPP */




namespace athena
{

	namespace core
	{

		// Function responsible of setting the id code of the event.
		inline void Event::code( const EventCode& value )
		{
			m_code = value;
		}

		// Function responsible of setting the id code of the triggering entity.
		inline void Event::initiator_id( const ListenerIDType& value )
		{
			m_initiator_id = value;
		}

		// Function responsible of setting the cleanup function.
		inline void Event::cleanup_function( EventCleanupFunction function )
		{
			m_cleanup_function = function;
		}


		// Function returning the id code of the event.
		inline EventCode Event::code() const
		{
			return m_code;
		}

		// Function returning the id of the triggering entity.
		inline ListenerIDType Event::initiator_id() const
		{
			return m_initiator_id;
		}

		// Function returning the cleanup function.
		inline EventCleanupFunction Event::cleanup_function() const
		{
			return m_cleanup_function;
		}

		// Function returning the size of the parameter list.
		inline unsigned int Event::parameter_count() const
		{
			return m_parameters.size();
		}

	} /* core */
	
} /* athena */



#endif /* ATHENA_CORE_EVENT_INL */