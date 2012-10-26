#include "thread.hpp"



namespace athena
{

	namespace utility
	{

		// The constructor of the class.
		Thread::Thread( threadFunction function , void* parameter ) : 
			_lock() , 
			_id(0) , 
			_exit_code(0) , 
			_function(function) , 
			_parameter(parameter) , 
			_running(false)
		{
		};

		// The destructor of the class.
		Thread::~Thread()
		{
			// Destroy the thread if it is not already destroyed.
			destroy();
		};

	} /* utility */

} /* athena */