#include "renderManager.hpp"



namespace athena
{

	namespace display
	{

		// The single instance of the class.
		RenderManager* RenderManager::_instance = INVALID_POINTER;
		// A lock used to handle concurrency issues regarding the instance of the class.
		utility::ReadersWriterLock RenderManager::_instance_lock;


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


			_instance_lock.lock(false);

			if ( _instance == INVALID_POINTER )
			{
				_instance = new (std::nothrow) RenderManager();
				return_value = ( _instance != INVALID_POINTER );
			}

			_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void RenderManager::deinitialise()
		{
			_instance_lock.lock(false);

			if ( _instance != INVALID_POINTER )
			{
				delete _instance;
				_instance = INVALID_POINTER;
			}

			_instance_lock.unlock();
		}

		// A function responsible of returning a single instance of the class.
		RenderManager* RenderManager::get()
		{
			RenderManager* return_value = INVALID_POINTER;


			_instance_lock.lock(true);
			return_value = _instance;
			_instance_lock.unlock();


			return return_value;
		}

	} /* display */

} /* athena */