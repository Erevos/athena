#include "audioManager.hpp"
#include "windowsDefinitions.hpp"
#include <AL/al.h>
#include <AL/alc.h>


namespace athena
{

	namespace display
	{

		// The single instance of the class.
		AudioManager* AudioManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex AudioManager::s_instance_lock;


		// The constructor of the class.
		AudioManager::AudioManager() :
			Listener(athena::AudioManagerID) ,
			m_lock() ,
			m_initialised(false)
		{
		}

		// The destructor of the class.
		AudioManager::~AudioManager()
		{
			terminate();
		}
		

		// A function responsible of initialising the single instance of the class.
		bool AudioManager::init()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) AudioManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void AudioManager::deinit()
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


		// A function responsible of commencing the functionality of the input system.
		bool AudioManager::startup()
		{
			bool return_value = true;


			m_lock.lock();

			if ( !m_initialised )
			{
				register_event(EVENT_EXIT);
				alGetError();
				m_initialised = true;
			}

			m_lock.unlock();


			return return_value;
		}

		// A function responsible of terminating the functionality of the input system.
		void AudioManager::terminate()
		{
			m_lock.lock();

			if ( m_initialised )
			{
				unregister_all_events();
				m_initialised = false;
			}

			m_lock.unlock();
		}


		// A function responsible of returning a single instance of the class.
		AudioManager* AudioManager::get()
		{
			AudioManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// Function responsible of responding to a triggered event.
		void AudioManager::on_event( const core::Event& event )
		{
			core::EventCode code = event.code();


			if ( code == EVENT_EXIT )
				terminate();
		}

	} /* display */

} /* athena */