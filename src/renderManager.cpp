#include "renderManager.hpp"
#include <GL/freeglut.h>
#include "eventCodes.hpp"

#ifdef _WIN32
	#pragma warning(disable:4505)
#endif /* _WIN32 */



namespace athena
{

	namespace display
	{

		// The single instance of the class.
		RenderManager* RenderManager::s_instance = NULL;
		// A lock used to handle concurrency issues regarding the instance of the class.
		std::mutex RenderManager::s_instance_lock;


		// The constructor of the class.
		RenderManager::RenderManager() :
			Listener(athena::RenderManagerID) ,
			m_lock() ,
			m_window_id(0) ,
			m_initialised(false)
		{
		}

		// The destructor of the class.
		RenderManager::~RenderManager()
		{
			terminate();
		}
		

		// A function responsible of initialising the single instance of the class.
		bool RenderManager::init()
		{
			bool return_value = true;


			s_instance_lock.lock();

			if ( s_instance == NULL )
			{
				s_instance = new (std::nothrow) RenderManager();
				return_value = ( s_instance != NULL );
			}

			s_instance_lock.unlock();


			return return_value;
		}

		// A function responsible of deinitialising the single instance of the class.
		void RenderManager::deinit()
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
		bool RenderManager::startup()
		{
			bool return_value = true;


			m_lock.lock();

			if ( !m_initialised )
			{
				register_event(EVENT_EXIT);
				glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_STENCIL|GLUT_MULTISAMPLE);
				m_window_id = glutCreateWindow("Athena");

				if ( m_window_id > 0 )
				{
					glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
					glutCloseFunc(athena::terminate);
					m_initialised = true;
				}
			}

			m_lock.unlock();


			return return_value;
		}

		// A function responsible of terminating the functionality of the input system.
		void RenderManager::terminate()
		{
			m_lock.lock();

			if ( m_initialised )
			{
				unregister_all_events();
				glutDestroyWindow(m_window_id);
				m_initialised = false;
			}

			m_lock.unlock();
		}


		// A function responsible of returning a single instance of the class.
		RenderManager* RenderManager::get()
		{
			RenderManager* return_value = NULL;


			s_instance_lock.lock();
			return_value = s_instance;
			s_instance_lock.unlock();


			return return_value;
		}


		// Function responsible of responding to a triggered event.
		void RenderManager::on_event( const core::Event& event )
		{
			core::EventCode code = event.code();


			if ( code == EVENT_EXIT )
				terminate();
		}

	} /* display */

} /* athena */