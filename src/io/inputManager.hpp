#ifndef ATHENA_IO_INPUTMANAGER_HPP
#define ATHENA_IO_INPUTMANAGER_HPP

#include "../athenaDefinitions.hpp"
#include "../utility/readersWriterLock.hpp"
#include <new>



namespace athena
{

	namespace io
	{

		/*
			A class responsible of handling any user input.
		*/
		class InputManager
		{
			private:

				// The single instance of the class.
				static InputManager* _instance;
				// A lock used to handle concurrency issues regarding the instance of the class.
				static utility::ReadersWriterLock _instance_lock;


				// The constructor of the class.
				InputManager();
				// The destructor of the class.
				~InputManager();

			public:

				// A function responsible of initialising the single instance of the class.
				ATHENA_DLL static bool initialise();
				// A function responsible of deinitialising the single instance of the class.
				ATHENA_DLL static void deinitialise();
				// A function responsible of returning a single instance of the class.
				ATHENA_DLL static InputManager* get();
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_INPUTMANAGER_HPP */