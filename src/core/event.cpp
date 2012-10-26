#include "event.hpp"



namespace athena
{

	namespace core
	{
		Parameter Event::NullParameter = {null,NULL};


		Event::Event( const unsigned long& id , const unsigned long& parameters ) : 
			_parameters(parameters,NullParameter) , 
			_id(id)
		{
		};

		Event::~Event()
		{
		};

	} /* core */

} /* athena */