#include "thread.hpp"



namespace athena
{

	namespace utility
	{

		// The static function responsible of calling the functionality function.
		ThreadExitType Thread::_thread_function( void* parameter )
		{
			Thread* thread = reinterpret_cast<Thread*>(parameter);


			if ( thread != INVALID_POINTER )
			{
				thread->_lock.lock(false);
				thread->_running = true;
				thread->_lock.unlock();
				thread->_exit_code = (*thread->functionality())(thread->parameter());
			}
				

			#ifdef _WIN32
				return thread->_exit_code;
			#else
				pthread_exit(&(thread->_exit_code));
			#endif /* _WIN32 */
		}


		// The constructor of the class.
		Thread::Thread( ThreadFunction function , void* parameter ) : 
			_lock() , 
			_id(0) , 
			_exit_code(0) , 
			_function(function) , 
			_parameter(parameter) , 
			_running(false)
		{
		}

		// The destructor of the class.
		Thread::~Thread()
		{
			// Destroy the thread if it is not already destroyed.
			destroy();
		}


		// Function setting the functionality of the thread. Must be called before the create() function.
		void Thread::functionality( ThreadFunction function )
		{
			_lock.lock(false);
			_function = function;
			_lock.unlock();
		}
			
		// Function setting the parameter of the thread. Must be called before the create() function.
		void Thread::parameter( void* value )
		{
			_lock.lock(false);
			_parameter = value;
			_lock.unlock();
		}


		// Function returning the running status of the thread.
		bool Thread::is_running() const
		{
			bool return_value = false;


			_lock.lock(true);
			return_value = _running;
			_lock.unlock();


			return return_value;
		}

		// Function returning the functionality function of the thread.
		ThreadFunction Thread::functionality()
		{
			ThreadFunction return_value = INVALID_POINTER;


			_lock.lock(true);
			return_value = _function;
			_lock.unlock();


			return return_value;
		}

		// Function returning the parameter of the thread.
		void* Thread::parameter()
		{
			void* return_value = INVALID_POINTER;


			_lock.lock(true);
			return_value = _parameter;
			_lock.unlock();


			return return_value;
		}

		// Function returning the exit code of the thread. Should be called after destroy().
		ThreadExitType Thread::exit_code() const
		{
			ThreadExitType return_value = 0;


			_lock.lock(true);
			return_value = _exit_code;
			_lock.unlock();


			return return_value;
		}


		// Function creating the thread. Returns true on success, false on failure.
		bool Thread::create()
		{
			bool return_value = false;


			_lock.lock(false);

			#ifdef _WIN32

				// If the thread has not been created.
				if ( _id == INVALID_POINTER )
				{
					// Create the thread.
					_id = CreateThread(INVALID_POINTER,0,_thread_function,static_cast<void*>(this),0,INVALID_POINTER);

					// If the thread has been created successfully.
					if ( _id != INVALID_POINTER )
						return_value = true;
				}

			#else

				// If the thread has not been created.
				if ( _id == 0 )
				{
					// If the thread creation is successful.
					if ( pthread_create(_id,INVALID_POINTER,_thread_function,static_cast<void*>(this)) == 0 )
						return_value = true;
				}

			#endif /* _WIN32 */

			_lock.unlock();


			return return_value;
		}

		// Function destroying the thread.
		void Thread::destroy()
		{
			_lock.lock(false);

			#ifdef _WIN32

				// If the thread has been created.
				if ( _id != INVALID_POINTER )
				{
					// Set the run flag to false.
					_running = false;
					_lock.unlock();
					// Wait for the thread to terminate.
					WaitForSingleObjectEx(_id,INFINITE,FALSE);
					_lock.lock(false);
					// Reset the variable holding the id of the thread.
					_id = INVALID_POINTER;
				}

			#else

				// If the thread has been created.
				if ( _id != 0 )
				{
					// Set the run flag to false.
					_running = false;
					_lock.unlock();
					// Wait for the thread to terminate.
					pthread_join(_id,INVALID_POINTER);
					_lock.lock(false);
					// Reset the variable holding the id of the thread.
					_id = 0;
				}

			#endif /* _WIN32 */

			_lock.unlock();
		}

	} /* utility */

} /* athena */