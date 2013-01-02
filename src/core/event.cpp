#include "event.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Event::Event( const EventCode& code ) :
			m_parameters(0,NULL) ,
			m_code(code) ,
			m_cleanup_function(NULL)
		{
		}

		// The copy constructor of the class.
		Event::Event( const Event& event ) :
			m_parameters(0,NULL) ,
			m_code(event.code()) ,
			m_cleanup_function(event.cleanup_function())
		{
			for ( unsigned int i = 0;  i < event.parameter_count();  ++i )
			{
				const Parameter* new_parameter = event.parameter(i);


				if ( new_parameter != NULL )
					parameter(i,new_parameter->m_type,new_parameter->m_data);
			}
		}

		// The destructor of the class.
		Event::~Event()
		{
			clear();
		}

		// The assignment operator.
		Event& Event::operator=( const Event& event )
		{
			if ( this != &event )
			{
				clear();
				m_code = event.code();
				m_cleanup_function = event.cleanup_function();

				for ( unsigned int i = 0;  i < event.parameter_count();  ++i )
				{
					const Parameter* new_parameter = event.parameter(i);


					if ( new_parameter != NULL )
						parameter(i,new_parameter->m_type,new_parameter->m_data);
				}
			}

			return *this;
		}


		// Function responsible of setting the parameter at the given index or adding a new parameter to the list.
		void Event::parameter( const unsigned int& index , const ParameterType type , void* data )
		{
			if ( ( type != Null  &&  data != NULL )  ||  ( type == Null  &&  data == NULL ) )
			{
				if ( index < m_parameters.size() )
				{
					m_parameters[index]->m_type = type;
					m_parameters[index]->m_data = data;
				}
				else
				{
					Parameter* new_parameter = new (std::nothrow) Parameter(type,data);


					if ( new_parameter != NULL )
						m_parameters.push_back(new_parameter);
				}
			}
		}

		// Function responsible of clearing the parameter list.
		void Event::clear()
		{
			for (
					std::vector<Parameter*>::iterator parameter_iterator = m_parameters.begin();
					parameter_iterator != m_parameters.end();
					++parameter_iterator
				)
			{
				delete (*parameter_iterator);
			}

			m_parameters.clear();
		}

		// Function responsible of performing cleanup.
		void Event::cleanup()
		{
			if ( m_cleanup_function != NULL )
				(*m_cleanup_function)(*this);

			clear();
		}


		// Function returning the parameter at the given index.
		const Parameter* Event::parameter( const unsigned int& index ) const
		{
			if ( index < m_parameters.size() ) 
				return m_parameters[index];
			else
				return NULL;
		}

	} /* core */

} /* athena */