#ifndef ATHENA_IO_INPUTMANAGER_HPP
#define ATHENA_IO_INPUTMANAGER_HPP

#include "definitions.hpp"
#include <mutex>
#include "athena.hpp"
#include "listener.hpp"
#include "inputDevice.hpp"



namespace athena
{

	namespace io
	{

		/*
			A class responsible of handling any user input.
		*/
		class InputManager : public core::Listener
		{
			private:

				// The single instance of the class.
				static InputManager* s_instance;
				// A lock used to handle concurrency issues regarding the instance of the class.
				static std::mutex s_instance_lock;


				// The list of devices that are currently active.
				std::vector<InputDevice*> m_devices;
				std::mutex m_lock;
				core::EventCode m_update_rate;
				bool m_initialised;


				// The constructor of the class.
				InputManager();
				// The destructor of the class.
				~InputManager();


				// Function responsible of changing the update rate of the input.
				void change_update_rate( const core::EventCode& code );
				// Function responsible of initialising a new device.
				bool initialise_device( const core::Event& event );
				// Function responsible of updating the active devices.
				void update();
				// Function responsible of performing cleanup.
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
				ATHENA_DLL static InputManager* get();


				// Function responsible of responding to a triggered event.
				ATHENA_DLL void on_event( const core::Event& event );
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_INPUTMANAGER_HPP */