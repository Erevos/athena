#include "renderManager.hpp"



namespace athena
{

	namespace display
	{

		// The single instance of the class.
		RenderManager* RenderManager::s_instance = INVALID_POINTER;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex RenderManager::s_instance_lock;


		// The constructor of the class.
		RenderManager::RenderManager()
		{
		}

		// The destructor of the class.
		RenderManager::~RenderManager()
		{
		}


		// A function responsible of initialising the single instance of the class.
		bool RenderManager::initialise()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == INVALID_POINTER )
			{
				s_instance = new (std::nothrow) RenderManager();
				return_value = ( s_instance != INVALID_POINTER );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void RenderManager::deinitialise()
		{
			s_instance_lock.lock();

			if ( s_instance != INVALID_POINTER )
			{
				delete s_instance;
				s_instance = INVALID_POINTER;
			}

			s_instance_lock.unlock();
		}

		// A function responsible of returning a single instance of the class.
		RenderManager* RenderManager::get()
		{
			RenderManager* return_value = INVALID_POINTER;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}

	} /* display */

} /* athena */