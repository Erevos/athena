#ifndef ATHENA_IO_INPUTMANAGER_HPP
#define ATHENA_IO_INPUTMANAGER_HPP

#include "../definitions.hpp"
#include <mutex>
#include <new>
#include "../athena.hpp"
#include "../core/listener.hpp"



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


				// The constructor of the class.
				InputManager();
				// The destructor of the class.
				~InputManager();


			protected:

				friend bool athena::init( const AthenaManagers& managers );
				friend bool athena::startup( const AthenaManagers& managers );
				friend void athena::deinit( const AthenaManagers& managers );


				// A function responsible of commencing the functionality of the event system.
				ATHENA_DLL bool startup();
				// A function responsible of terminating the functionality of the event system.
				ATHENA_DLL void terminate();


			public:

				// A function responsible of initialising the single instance of the class.
				ATHENA_DLL static bool init();
				// A function responsible of deinitialising the single instance of the class.
				ATHENA_DLL static void deinit();
				// A function responsible of returning a single instance of the class.
				ATHENA_DLL static InputManager* get();
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_INPUTMANAGER_HPP */