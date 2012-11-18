#ifndef ATHENA_CORE_EVENT_INL
	#define ATHENA_CORE_EVENT_INL



	namespace athena
	{

		namespace core
		{

			// Function responsible of setting the id code of the event.
			inline void Event::code( const EventCode& value )
			{
				_code = value;
			};

			// Function responsible of setting the cleanup function.
			inline void Event::cleanup_function( EventCleanupFunction function )
			{
				_cleanup_function = function;
			};

			// Function responsible of clearing the parameter list.
			inline void Event::clear()
			{
				_parameters.clear();
			};


			// Function returning the id code of the event.
			inline EventCode Event::code() const
			{
				return _code;
			};

			// Function returning the cleanup function.
			inline EventCleanupFunction Event::cleanup_function() const
			{
				return _cleanup_function;
			};

			// Function returning the size of the parameter list.
			inline unsigned int Event::parameter_count() const
			{
				return _parameters.size();
			};

		} /* core */
	
	} /* athena */



#endif /* ATHENA_CORE_EVENT_INL */