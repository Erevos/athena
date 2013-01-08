#include "renderManager.hpp"
#include "../eventCodes.hpp"



namespace athena
{

	namespace display
	{

		// The single instance of the class.
		RenderManager* RenderManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex RenderManager::s_instance_lock;


		// The constructor of the class.
		RenderManager::RenderManager() :
			Listener(athena::RenderManagerID)
		{
		}

		// The destructor of the class.
		RenderManager::~RenderManager()
		{
		}
		

		// A function responsible of commencing the functionality of the input system.
		bool RenderManager::startup()
		{
			bool return_value = true;


			return return_value;
		}

		// A function responsible of terminating the functionality of the input system.
		void RenderManager::terminate()
		{
		}


		// A function responsible of initialising the single instance of the class.
		bool RenderManager::init()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) RenderManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void RenderManager::deinit()
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

		// A function responsible of returning a single instance of the class.
		RenderManager* RenderManager::get()
		{
			RenderManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}

	} /* display */

} /* athena */