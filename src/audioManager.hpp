#ifndef ATHENA_DISPLAY_AUDIOMANAGER_HPP
#define ATHENA_DISPLAY_AUDIOMANAGER_HPP

#include "definitions.hpp"
#include <mutex>
#include "athena.hpp"
#include "listener.hpp"



namespace athena
{

	namespace display
	{

		/*
			A class handling any necessary audio operations.
		*/
		class AudioManager : public core::Listener
		{
			private:

				// The single instance of the class.
				static AudioManager* s_instance;
				// A lock used to handle concurrency issues regarding the instance of the class.
				static std::mutex s_instance_lock;


				std::mutex m_lock;
				bool m_initialised;


				// The constructor of the class.
				AudioManager();
				// The destructor of the class.
				~AudioManager();


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
				ATHENA_DLL static AudioManager* get();


				// Function responsible of responding to a triggered event.
				ATHENA_DLL void on_event( const core::Event& event );
		};

	} /* display */

} /* athena */



#endif /* ATHENA_DISPLAY_AUDIOMANAGER_HPP */