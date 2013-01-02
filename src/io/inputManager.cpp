#include "inputManager.hpp"



namespace athena
{

	namespace io
	{

		// The single instance of the class.
		InputManager* InputManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex InputManager::s_instance_lock;


		// The constructor of the class.
		InputManager::InputManager()
		{
		}

		// The destructor of the class.
		InputManager::~InputManager()
		{
		}


		// A function responsible of initialising the single instance of the class.
		bool InputManager::initialise()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) InputManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void InputManager::deinitialise()
		{
			s_instance_lock.lock();

			if ( s_instance != NULL )
			{
				delete s_instance;
				s_instance = NULL;
			}

			s_instance_lock.unlock();
		}

		// A function responsible of returning a single instance of the class.
		InputManager* InputManager::get()
		{
			InputManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}

	} /* io */

} /* athena */