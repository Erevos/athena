#include "mouse.hpp"
#include <GL/freeglut.h>
#include <cstring>
#include <cmath>
#include <limits>
#include "athena.hpp"

#ifdef _WIN32
	#pragma warning(disable:4505)
#endif /* _WIN32 */



namespace athena
{

	namespace io
	{

		// An enumeration that is used to keep the mouse button codes.
		enum MouseCodes
		{
			Left = 0, 
			Middle ,
			Right ,
			MouseWheel ,
		};


		// The deadzone that is used to determine whether an update is parsed.
		unsigned int Mouse::s_deadzone = 0;
		// The last position of the mouse.
		int Mouse::s_position[s_coordinates];
		// The state of the buttons.
		bool Mouse::s_status[s_buttons];


		// Function responsible of setting the deadzone.
		void Mouse::deadzone( const unsigned int value )
		{
			s_deadzone = value;
		}


		// Function responsible of handling any mouse button input.
		void Mouse::mouse_function( int button , int state , int , int )
		{
			unsigned int code = std::numeric_limits<unsigned int>::max();


			if ( button == GLUT_LEFT_BUTTON )
				code = MouseCodes::Left;
			else if ( button == GLUT_MIDDLE_BUTTON )
				code = MouseCodes::Middle;
			else if ( button == GLUT_RIGHT_BUTTON )
				code = MouseCodes::Right;
			else if ( state != GLUT_UP )
			{
				code = MouseCodes::MouseWheel;

				if ( button == 3 )
					athena::trigger_event(core::Event(EVENT_INPUT_MOUSE_LEFT_UP + code - MouseCodes::Left));
				else if ( button == 4 )
					athena::trigger_event(core::Event(EVENT_INPUT_MOUSE_LEFT_DOWN + code - MouseCodes::Left));
			}

			if ( code < s_buttons )
			{
				if ( state == GLUT_DOWN  &&  !s_status[code] )
				{
					s_status[code] = true;
					athena::trigger_event(core::Event(EVENT_INPUT_MOUSE_LEFT_DOWN + code - MouseCodes::Left));
				}
				else if ( state == GLUT_UP  &&  s_status[code] )
				{
					s_status[code] = false;
					athena::trigger_event(core::Event(EVENT_INPUT_MOUSE_LEFT_UP + code - MouseCodes::Left));
				}
			}
		}

		// Function responsible of handling the mouse position
		void Mouse::mouse_movement_function( int x , int y )
		{
			if ( static_cast<unsigned int>(abs(x) + abs(y)) >= s_deadzone )
			{
				int* new_x = new (std::nothrow) int(x);


				if ( new_x != NULL )
				{
					int* new_y = new (std::nothrow) int(y);

					if ( new_y != NULL )
					{
						int* diff_x = new (std::nothrow) int(x - s_position[0]);
					

						if ( diff_x != NULL )
						{
							int* diff_y = new (std::nothrow) int(y - s_position[1]);


							if ( diff_y != NULL )
							{
								core::Event event(EVENT_INPUT_MOUSE_POSITION);


								s_position[0] = x;
								s_position[1] = y;
								event.cleanup_function(cleanup);
								event.parameter(0,core::ParameterType::Integer,new_x);
								event.parameter(1,core::ParameterType::Integer,new_y);
								athena::trigger_event(event);

								event.code(EVENT_INPUT_MOUSE_POSITION_DIFFERENCE);
								event.parameter(0,core::ParameterType::Integer,diff_x);
								event.parameter(1,core::ParameterType::Integer,diff_y);
								athena::trigger_event(event);
							}
							else
							{
								delete diff_x;
								delete new_y;
								delete new_x;
							}
						}
						else
						{
							delete new_y;
							delete new_x;
						}
					}
					else
						delete new_x;
				}
			}
		}

		// Function responsible of cleaning up any allocated memory for the events.
		void Mouse::cleanup( const core::Event& event )
		{
			for ( unsigned int i = 0;  i < event.parameter_count();  ++i )
			{
				const core::Parameter* parameter(event.parameter(i));


				if ( parameter != NULL )
				{
					delete static_cast<int*>(parameter->data());
				}
			}
		}


		// The constructor of the class.
		Mouse::Mouse()
		{
			memset(s_position,0,sizeof(int)*s_coordinates);
			memset(s_status,0,sizeof(bool)*s_buttons);
		}

		// The destructor of the class
		Mouse::~Mouse()
		{
		}


		// Function responsible of performing any setup operations.
		void Mouse::startup()
		{
			glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
			glutMouseFunc(mouse_function);
			//glutMouseWheelFunc(mousewheel_function);
			glutMotionFunc(mouse_movement_function);
			glutPassiveMotionFunc(mouse_movement_function);
		}

		// Function responsible of reforming any cleanup operations.
		void Mouse::terminate()
		{
			glutMouseFunc(NULL);
			//glutMouseWheelFunc(NULL);
			glutMotionFunc(NULL);
			glutPassiveMotionFunc(NULL);
		}

		// Function responsible of performing update operations.
		void Mouse::update()
		{
		}


		// Function returning the type of the device.
		DeviceType Mouse::type() const
		{
			return DeviceType::IsMouse;
		}

	} /* io */

} /* athena */