#ifndef ATHENA_CORE_EVENT_INL
	#define ATHENA_CORE_EVENT_INL



	namespace athena
	{

		namespace core
		{

			inline void Event::parameter( const unsigned long& index , const Parameter& parameter )
			{
				if ( index < _parameters.size() )
					_parameters[index] = parameter;
				else
					_parameters.push_back(parameter);
			};

			inline void Event::id( const unsigned long& id )
			{
				_id = id;
			};

			inline void Event::clear()
			{
				_parameters.clear();
			};


			inline Parameter Event::parameter( const unsigned long& index ) const
			{
				if ( index < _parameters.size() ) 
					return _parameters[index];
				else
					return NullParameter;
			};

			inline unsigned long Event::id() const
			{
				return _id;
			};

			inline unsigned long Event::parameter_count() const
			{
				return _parameters.size();
			};

		} /* core */
	
	} /* athena */



#endif /* ATHENA_CORE_EVENT_INL */