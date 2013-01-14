#include "threadPool.hpp"
#include <iostream>

#ifdef _WIN32
	
	#include "../windowsDefinitions.hpp"
	#include <Windows.h>

#else
	#include <unistd.h>
#endif /* _WIN32 */



namespace athena
{

	namespace core
	{

		// The single instance of the class.
		ThreadPool* ThreadPool::s_instance = NULL;
		// A lock that is used to resolve concurrency issues for the instance of the class.
		std::mutex ThreadPool::s_instance_lock;


		// The function that is used to perform the functionality of each thread.
		void ThreadPool::thread_functionality( void* parameter )
		{
			ThreadPool* pool = static_cast<ThreadPool*>(parameter);


			if ( pool != NULL )
			{
				ThreadTask* task = NULL;
				bool run = true;


				while ( run )
				{
					// Get the task lock.
					pool->m_lock.lock();
					// Check if the thread pool is running.
					run = pool->m_run;
					
					// If the thread pool is operational.
					if ( run )
					{
						// If the task queue is not empty.
						if ( pool->m_tasks.size() > 0 )
						{
							// Get the first task in the queue.
							task = pool->m_tasks[0];
							// Pop the task from the queue.
							pool->m_tasks.pop_front();
						}
					}
					
					// Release the task lock.
					pool->m_lock.unlock();

					if ( run )
					{
						// If a task was retrieved.
						if ( task != NULL )
						{
							// A variable that is used to hold the exit code of the task.
							int exit_code = 0;
						
						
							// Perform the task.
							exit_code = (*task->m_function)(task->m_parameter);

							// If a callback function was provided.
							if ( task->m_callback != NULL )
								(*task->m_callback)(exit_code,task->m_callback_parameter); // Call the callback function.
						
							// Deallocate the task.
							delete task;
							// Reset the task pointer.
							task = NULL;
						}
						else
						{
							// Get the condition variable lock.
							pool->m_wait_lock.lock();
							// Wait for a signal from the condition variable.
							pool->m_condition_variable.wait(pool->m_wait_lock);
							// Release the condition variable lock.
							pool->m_wait_lock.unlock();
						}
					}
				}
			}
		}


		// The constructor of the class.
		ThreadPool::ThreadPool() :
			m_pool(0,NULL) ,
			m_tasks(0,NULL) ,
			m_lock() ,
			m_wait_lock() ,
			m_condition_variable() ,
			m_initialised(false) ,
			m_run(false)
		{
		}

		// The destructor of the class.
		ThreadPool::~ThreadPool()
		{
			// Perform cleanup if it has not been done already.
			cleanup();
		}


		// A function responsible of performing any needed cleanup.
		void ThreadPool::cleanup()
		{
			// For every thread in the pool.
			for (
					std::vector<std::thread*>::iterator thread_iterator = m_pool.begin();
					thread_iterator != m_pool.end();
					++thread_iterator
				)
			{
				// Deallocate the thread.
				delete (*thread_iterator);
			}

			// Clear the thread pool.
			m_pool.clear();

			// For every pending task.
			for ( 
					std::deque<ThreadTask*>::iterator task_iterator = m_tasks.begin();
					task_iterator != m_tasks.end();
					++task_iterator
				)
			{
				// Deallocate the task.
				delete (*task_iterator);
			}

			// Clear the task queue.
			m_tasks.clear();
		}


		// A function responsible of initialising the single instance of the class.
		bool ThreadPool::init()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) ThreadPool();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void ThreadPool::deinit()
		{
			s_instance_lock.lock();

			if ( s_instance != NULL )
			{
				s_instance->terminate();
				delete s_instance;
				s_instance = NULL;
			}

			s_instance_lock.unlock();
		}


		// A function responsible of commencing the functionality of the thread pool.
		bool ThreadPool::startup()
		{
			// Get the maximum number of threads.
			unsigned int max_threads = std::thread::hardware_concurrency();
			bool return_value = true;


			// If the maximum number of threads is 0, we will create at least 1 thread.
			if ( max_threads < 1 )
				max_threads = 1;

			m_lock.lock();

			// If the thread pool is not operational.
			if ( !m_initialised )
			{
				// Set the run variable.
				m_run = true;

				// Spawn the threads.
				for ( unsigned int i = 0;  i < max_threads;  ++i )
				{
					std::thread* new_thread = new (std::nothrow) std::thread(thread_functionality,static_cast<void*>(this));


					if ( new_thread != NULL )
						m_pool.push_back(new_thread);
				}

				if ( m_pool.size() > 0 )
					m_initialised = true;
				else
					return_value = false;
			}

			m_lock.unlock();


			return return_value;
		}

		// A function responsible of terminating the functionality of the thread pool.
		void ThreadPool::terminate()
		{
			m_lock.lock();

			if ( m_initialised )
			{
				// Set the run variable to false.
				m_run = false;
				m_lock.unlock();
				
				// Wait for every thread to exit.
				for (
						std::vector<std::thread*>::iterator thread_iterator = m_pool.begin();
						thread_iterator != m_pool.end();
						++thread_iterator
					)
				{
					/*
						Wake all the threads that are waiting on the condition variable.
						This is a workaround for missed wake up calls.
					*/
					m_condition_variable.notify_all();
					// Join the thread.
					(*thread_iterator)->join();
				};

				m_lock.lock();
				// Perform cleanup.
				cleanup();
				// Set the initialised flag to false.
				m_initialised = false;
			}

			m_lock.unlock();
		}


		// A function responsible of returning a single instance of the class.
		ThreadPool* ThreadPool::get()
		{
			ThreadPool* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// A function responsible of queuing a task.
		bool ThreadPool::add_task( TaskFunction task , void* parameter , TaskCallbackFunction callback , void* callback_parameter )
		{
			bool return_value = false;


			if ( task != NULL )
			{
				m_lock.lock();

				// If the thread pool is operational.
				if ( m_run )
				{
					// Create the new task to be performed.
					ThreadTask* new_task = new (std::nothrow) ThreadTask(task,parameter,callback,callback_parameter);


					if ( new_task != NULL )
					{
						// Insert the new task to the task queue.
						m_tasks.push_back(new_task);
						// Wake up a suspended thread (If any thread is suspended).
						m_condition_variable.notify_one();
						return_value = true;
					}
				}

				m_lock.unlock();
			}


			return return_value;
		}

	} /* core */

} /* athena */