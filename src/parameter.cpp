#include "parameter.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Parameter::Parameter( const ParameterType& type , void* data ) : 
			m_type(type) , 
			m_data(data)
		{
		}

		// The destructor of the class.
		Parameter::~Parameter()
		{
		}

	} /* core */

} /* athena */