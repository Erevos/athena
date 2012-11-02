#ifndef ATHENA_UTILITY_THREAD_INL
	#define ATHENA_UTILITY_THREAD_INL



	namespace athena
	{

		namespace utility
		{

			// The static function responsible of calling the functionality function.
			#ifdef _WIN32
				inline DWORD Thread::_thread_function( void* parameter )
			#else
				inline int Thread::_thread_function( void* parameter )
			#endif /* _WIN32 */
			{
				Thread* thread = reinterpret_cast<Thread*>(parameter);


				if ( thread != NULL )
					thread->_exit_code = (*thread->functionality())(thread->parameter());
				

				#ifdef _WIN32
					return thread->_exit_code;
				#else
					pthread_exit(&(thread->_exit_code));
				#endif /* _WIN32 */
			};


			// Function setting the functionality of the thread. Must be called before the create() function.
			inline void Thread::functionality( threadFunction function )
			{
				_lock.lock(false);
				_function = function;
				_lock.unlock();
			};
			
			// Function setting the parameter of the thread. Must be called before the create() function.
			inline void Thread::parameter( void* value )
			{
				_lock.lock(false);
				_parameter = value;
				_lock.unlock();
			};


			// Function returning the running status of the thread.
			inline bool Thread::is_running() const
			{
				bool return_value = false;


				_lock.lock(true);
				return_value = _running;
				_lock.unlock();


				return return_value;
			};

			// Function returning the functionality function of the thread.
			inline threadFunction Thread::functionality()
			{
				threadFunction return_value = NULL;


				_lock.lock(true);
				return_value = _function;
				_lock.unlock();


				return return_value;
			};

			// Function returning the parameter of the thread.
			inline void* Thread::parameter()
			{
				void* return_value = NULL;


				_lock.lock(true);
				return_value = _parameter;
				_lock.unlock();


				return return_value;
			};

			// Function returning the exit code of the thread. Should be called after destroy().
			#ifdef _WIN32
				inline DWORD Thread::exit_code() const
			#else
				inline int Thread::exit_code() const
			#endif /* _WIN32 */
			{
				#ifdef _WIN32
					DWORD return_value = 0;
				#else
					int return_value = 0;
				#endif /* _WIN32 */


				_lock.lock(true);
				return_value = _exit_code;
				_lock.unlock();


				return return_value;
			};


			// Function creating the thread. Returns true on success, false on failure.
			inline bool Thread::create()
			{
				bool return_value = false;


				_lock.lock(false);

				#ifdef _WIN32

					// If the thread has not been created.
					if ( _id == NULL )
					{
						// Create the thread.
						_id = CreateThread(NULL,0,_thread_function,_parameter,0,NULL);

						// If the thread has been created successfully.
						if ( _id != NULL )
							return_value = true;
					}

				#else

					// If the thread has not been created.
					if ( _id == 0 )
					{
						// If the thread creation is successful.
						if ( pthread_create(_id,NULL,_thread_function,_parameter) == 0 )
							return_value = true;
					}

				#endif /* _WIN32 */

				_lock.unlock();


				return return_value;
			};

			// Function destroying the thread.
			inline void Thread::destroy()
			{
				_lock.lock(false);

				#ifdef _WIN32

					// If the thread has been created.
					if ( _id != NULL )
					{
						// Set the run flag to false.
						_running = false;
						// Wait for the thread to terminate.
						WaitForSingleObjectEx(_id,INFINITE,FALSE);
						// Reset the variable holding the id of the thread.
						_id = NULL;
					}

				#else

					// If the thread has been created.
					if ( _id != 0 )
					{
						// Set the run flag to false.
						_running = false;
						// Wait for the thread to terminate.
						pthread_join(_id,NULL);
						// Reset the variable holding the id of the thread.
						_id = 0;
					}

				#endif /* _WIN32 */

				_lock.unlock();
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_UTILITY_THREAD_INL */