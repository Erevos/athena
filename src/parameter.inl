#ifndef ATHENA_CORE_PARAMETER_INL
#define ATHENA_CORE_PARAMETER_INL
	
#ifndef ATHENA_CORE_PARAMETER_HPP
	#error "parameter.hpp must be included before parameter.inl"
#endif /* ATHENA_CORE_PARAMETER_HPP */
	

	
namespace athena
{

	namespace core
	{

		// A function responsible of setting the type of the parameter.
		inline void Parameter::type( const ParameterType& type )
		{
			m_type = type;
		}

		// A function responsible of setting the data of the parameter.
		inline void Parameter::data( void* data )
		{
			m_data = data;
		}

					
		// A function returning the type of the parameter.
		inline ParameterType Parameter::type() const
		{
			return m_type;
		}

		// A function returning the data of the parameter.
		inline void* Parameter::data() const
		{
			return m_data;
		}

	} /* core */

} /* athena */



#endif /* ATHENA_CORE_PARAMETER_INL */