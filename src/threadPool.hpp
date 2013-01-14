#ifndef ATHENA_CORE_THREADPOOL_HPP
#define ATHENA_CORE_THREADPOOL_HPP

#include "definitions.hpp"
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "athena.hpp"



namespace athena
{

	namespace core
	{

		/*
			A type definition that is used to define the function that is 
			considered a "task" for the thread pool threads.
		*/
		typedef int (*TaskFunction)( void * parameter );
		/*
			A type definition that is used to define the function that is called
			when a task is completed.
		*/
		typedef void (*TaskCallbackFunction)( const int exit_code , void* parameter );

		
		/*
			The struct that is used to store the task of the thread and it's
			single parameter.
		*/
		struct ThreadTask
		{
			TaskFunction m_function;
			TaskCallbackFunction m_callback;
			void* m_parameter;
			void* m_callback_parameter;


			ThreadTask(
						TaskFunction function ,
						void* parameter ,
						TaskCallbackFunction callback ,
						void* callback_parameter
					) :
				m_function(function) ,
				m_callback(callback) , 
				m_parameter(parameter) ,
				m_callback_parameter(callback_parameter)
			{
			};
		};


		/*
			A class representing and handling the thread pool that the 
			engine is using to perform various tasks. In the event that the 
			hardware supports only one (1) thread the thread pool will have a 
			size of one (1), resulting in two (2) total threads including the 
			main thread of the application.
		*/
		class ThreadPool
		{
			private:

				// The single instance of the class.
				static ThreadPool* s_instance;
				// A lock that is used to resolve concurrency issues for the instance of the class.
				static std::mutex s_instance_lock;


				// The pool of threads.
				std::vector<std::thread*> m_pool;
				// The pending task list.
				std::deque<ThreadTask*> m_tasks;
				// A lock that is used to handle concurrency issues.
				std::mutex m_lock;
				// A lock that is used to signal waiting threads.
				std::mutex m_wait_lock;
				// A condition variable that is used to manage the waiting threads.
				std::condition_variable_any m_condition_variable;
				// A variable holding whether the class has been initialised.
				bool m_initialised;
				// A variable holding whether the class is operational.
				bool m_run;


				// The function that is used to perform the functionality of each thread.
				static void thread_functionality( void* parameter );


				// The constructor of the class.
				ThreadPool();
				// The destructor of the class.
				~ThreadPool();


				// A function responsible of performing any needed cleanup.
				void cleanup();


			protected:

				friend bool athena::init( const AthenaManagers& managers , int& argc , char**& argv );
				friend bool athena::startup( const AthenaManagers& managers );
				friend void athena::deinit( const AthenaManagers& managers );


				// A function responsible of initialising the single instance of the class.
				ATHENA_DLL static bool init();
				// A function responsible of deinitialising the single instance of the class.
				ATHENA_DLL static void deinit();


				// A function responsible of commencing the functionality of the event system.
				ATHENA_DLL bool startup();
				// A function responsible of terminating the functionality of the event system.
				ATHENA_DLL void terminate();


			public:

				// A function responsible of returning a single instance of the class.
				ATHENA_DLL static ThreadPool* get();


				// A function responsible of queuing a task.
				ATHENA_DLL bool add_task( TaskFunction task , void* parameter , TaskCallbackFunction callback , void* callback_parameter );
		};

	} /* core */

} /* athena */



#endif /* ATHENA_CORE_THREADPOOL_HPP */