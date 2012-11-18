#include "event.hpp"



namespace athena
{

	namespace core
	{

		// The constructor of the class.
		Event::Event( const EventCode& code , const unsigned long& parameters ) : 
			_parameters(parameters,Parameter(Null,INVALID_POINTER)) , 
			_code(code) , 
			_cleanup_function(INVALID_POINTER)
		{
		};

		// The destructor of the class.
		Event::~Event()
		{
		};


		// Function responsible of setting the parameter at the given index or adding a new parameter to the list.
		void Event::parameter( const unsigned int& index , const ParameterType type , void* data )
		{
			if ( index < _parameters.size() )
			{
				_parameters[index]._type =type;
				_parameters[index]._data = data;
			}
			else
				_parameters.push_back(Parameter(type,data));
		};

		// Function responsible of performing cleanup.
		void Event::cleanup()
		{
			if ( _cleanup_function != INVALID_POINTER )
				(*_cleanup_function)(*this);

			clear();
		};


		// Function returning the parameter at the given index.
		Parameter Event::parameter( const unsigned int& index ) const
		{
			if ( index < _parameters.size() ) 
				return _parameters[index];
			else
				return Parameter(Null,INVALID_POINTER);
		};

	} /* core */

} /* athena */