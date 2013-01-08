#ifndef ATHENA_DISPLAY_RENDERMANAGER_HPP
#define ATHENA_DISPLAY_RENDERMANAGER_HPP

#include "../definitions.hpp"
#include <mutex>
#include <new>
#include "../athena.hpp"
#include "../core/listener.hpp"



namespace athena
{

	namespace display
	{

		/*
			A class responsible of handling window creation and rendering contents into those windows.
		*/
		class RenderManager : public core::Listener
		{
			private:

				// The single instance of the class.
				static RenderManager* s_instance;
				// A lock used to handle concurrency issues regarding the instance of the class.
				static std::mutex s_instance_lock;


				// The constructor of the class.
				RenderManager();
				// The destructor of the class.
				~RenderManager();


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
				ATHENA_DLL static RenderManager* get();
		};

	} /* display */

} /* athena */



#endif /* ATHENA_DISPLAY_RENDERMANAGER_HPP */