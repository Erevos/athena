#ifndef ATHENA_CORE_PARAMETER_INL
	#define ATHENA_CORE_PARAMETER_INL
	
	
	
	namespace athena
	{

		namespace core
		{

			// A function responsible of setting the type of the parameter.
			inline void Parameter::type( const ParameterType& type )
			{
				_type = type;
			};

			// A function responsible of setting the data of the parameter.
			inline void Parameter::data( void* data )
			{
				_data = data;
			};

					
			// A function returning the type of the parameter.
			inline ParameterType Parameter::type() const
			{
				return _type;
			};

			// A function returning the data of the parameter.
			inline void* Parameter::data() const
			{
				return _data;
			};

		} /* core */

	} /* athena */



#endif /* ATHENA_CORE_PARAMETER_INL */