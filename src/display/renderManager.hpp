#ifndef ATHENA_DISPLAY_RENDERMANAGER_HPP
	#define ATHENA_DISPLAY_RENDERMANAGER_HPP

	#include "../athenaDefinitions.hpp"
	#include "../utility/readersWriterLock.hpp"
	#include <new>



	namespace athena
	{

		namespace display
		{

			/*
				A class responsible of handling window creation and rendering contents into those windows.
			*/
			class RenderManager
			{
				private:

					// The single instance of the class.
					static RenderManager* _instance;
					// A lock used to handle concurrency issues regarding the instance of the class.
					static utility::ReadersWriterLock _instance_lock;


					// The constructor of the class.
					ATHENA_PRECALL RenderManager() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_PRECALL ~RenderManager() ATHENA_POSTCALL;

				public:

					// A function responsible of initialising the single instance of the class.
					ATHENA_DLL static bool ATHENA_PRECALL initialise() ATHENA_POSTCALL;
					// A function responsible of deinitialising the single instance of the class.
					ATHENA_DLL static void ATHENA_PRECALL deinitialise() ATHENA_POSTCALL;
					// A function responsible of returning a single instance of the class.
					ATHENA_DLL static RenderManager* ATHENA_PRECALL get() ATHENA_POSTCALL;
			};

		} /* display */

	} /* athena */



#endif /* ATHENA_DISPLAY_RENDERMANAGER_HPP */