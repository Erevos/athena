#include "event.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Event::Event( const EventCode& code , const unsigned long& parameters ) : 
			m_parameters(parameters,Parameter(Null,INVALID_POINTER)) , 
			m_code(code) , 
			m_cleanup_function(INVALID_POINTER)
		{
		}

		// The destructor of the class.
		Event::~Event()
		{
		}


		// Function responsible of setting the parameter at the given index or adding a new parameter to the list.
		void Event::parameter( const unsigned int& index , const ParameterType type , void* data )
		{
			if ( index < m_parameters.size() )
			{
				m_parameters[index].m_type = type;
				m_parameters[index].m_data = data;
			}
			else
				m_parameters.push_back(Parameter(type,data));
		}

		// Function responsible of performing cleanup.
		void Event::cleanup()
		{
			if ( m_cleanup_function != INVALID_POINTER )
				(*m_cleanup_function)(*this);

			clear();
		}


		// Function returning the parameter at the given index.
		Parameter Event::parameter( const unsigned int& index ) const
		{
			if ( index < m_parameters.size() ) 
				return m_parameters[index];
			else
				return Parameter(Null,INVALID_POINTER);
		}

	} /* core */

} /* athena */