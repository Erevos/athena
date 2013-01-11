#include "inputManager.hpp"
#include "../eventCodes.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"



namespace athena
{

	namespace io
	{

		// The single instance of the class.
		InputManager* InputManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex InputManager::s_instance_lock;


		// The constructor of the class.
		InputManager::InputManager() :
			Listener(athena::InputManagerID) ,
			m_devices(0,NULL) ,
			m_lock() ,
			m_update_rate(EVENT_UPDATE) ,
			m_initialised(false)
		{
		}

		// The destructor of the class.
		InputManager::~InputManager()
		{
			terminate();
		}


		// Function responsible of changing the update rate of the input.
		void InputManager::change_update_rate( const core::EventCode& code )
		{
			if ( code >= EVENT_UPDATE  &&  code <= EVENT_UPDATE_SLOW )
			{
				if ( code != m_update_rate )
				{
					unregister_event(m_update_rate);
					m_update_rate = code;
					register_event(m_update_rate);
				}
			}
		}
		
		// Function responsible of initialising a new device.
		void InputManager::initialise_device( const core::Event& event )
		{
			core::EventCode code = event.code();
			InputDevice* new_device = NULL;


			if ( code == EVENT_INPUT_INIT_KEYBOARD )
				new_device = new (std::nothrow) Keyboard();
			else if ( code == EVENT_INPUT_INIT_MOUSE )
				new_device = new (std::nothrow) Mouse();


			if ( new_device != NULL )
			{
				new_device->startup();
				m_devices.push_back(new_device);
			}
		}
		
		// Function responsible of updating the active devices.
		void InputManager::update()
		{
			for (
					std::vector<InputDevice*>::iterator device_iterator = m_devices.begin();
					device_iterator != m_devices.end();
					++device_iterator
				)
			{
				(*device_iterator)->update();
			}
		}
		
		// Function responsible of performing cleanup.
		void InputManager::cleanup()
		{
			for (
					std::vector<InputDevice*>::iterator device_iterator = m_devices.begin();
					device_iterator != m_devices.end();
					++device_iterator
				)
			{
				(*device_iterator)->terminate();
				delete (*device_iterator);
			}

			m_devices.clear();
		}
		

		// A function responsible of commencing the functionality of the input system.
		bool InputManager::startup()
		{
			bool return_value = true;


			m_lock.lock();

			if ( !m_initialised )
			{
				register_event(EVENT_INPUT_UPDATE_RATE);
				register_event(EVENT_INPUT_INIT_KEYBOARD);
				register_event(EVENT_INPUT_INIT_MOUSE);
				register_event(m_update_rate);
				register_event(EVENT_EXIT);
				m_initialised = true;
			}

			m_lock.unlock();

			return return_value;
		}

		// A function responsible of terminating the functionality of the input system.
		void InputManager::terminate()
		{
			m_lock.lock();

			if ( m_initialised )
			{
				unregister_all_events();
				cleanup();
				m_initialised = false;
			}

			m_lock.unlock();
		}


		// A function responsible of initialising the single instance of the class.
		bool InputManager::init()
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
		void InputManager::deinit()
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
		InputManager* InputManager::get()
		{
			InputManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// Function responsible of responding to a triggered event.
		void InputManager::on_event( const core::Event& event )
		{
			core::EventCode code = event.code();


			if ( code == m_update_rate )
				update();
			else if ( code == EVENT_INPUT_UPDATE_RATE )
			{
				const core::Parameter* parameter(event.parameter(0));


				if ( parameter != NULL )
					change_update_rate(*(static_cast<core::EventCode*>(parameter->data())));
			}
			else if (
						code == EVENT_INPUT_INIT_KEYBOARD  ||
						code == EVENT_INPUT_INIT_MOUSE
					)
			{
				initialise_device(event);
				unregister_event(code);
			}
			else if ( code == EVENT_EXIT )
				terminate();
		}

	} /* io */

} /* athena */