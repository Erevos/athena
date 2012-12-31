#include "inputManager.hpp"



namespace athena
{

	namespace io
	{

		// The single instance of the class.
		InputManager* InputManager::_instance = INVALID_POINTER;
		// A lock used to handle concurrency issues regarding the instance of the class.
		utility::ReadersWriterLock InputManager::_instance_lock;


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


			_instance_lock.lock(false);

			if ( _instance == INVALID_POINTER )
			{
				_instance = new (std::nothrow) InputManager();
				return_value = ( _instance != INVALID_POINTER );
			}

			_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void InputManager::deinitialise()
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
		InputManager* InputManager::get()
		{
			InputManager* return_value = INVALID_POINTER;


			_instance_lock.lock(true);
			return_value = _instance;
			_instance_lock.unlock();


			return return_value;
		}

	} /* io */

} /* athena */