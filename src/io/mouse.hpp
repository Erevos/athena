#ifndef ATHENA_IO_MOUSE_HPP
#define ATHENA_IO_MOUSE_HPP

#include "../definitions.hpp"
#include "inputDevice.hpp"
#include "../core/event.hpp"



namespace athena
{

	namespace io
	{

		/*
			A class representing and handling any mouse input.
		*/
		class Mouse : public InputDevice
		{
			private:

				// The number of mouse buttons;
				static const unsigned int s_buttons = 3;
				// The number of mouse coordinates.
				static const unsigned int s_coordinates = 2;
				// The last position of the mouse.
				static int s_position[s_coordinates];
				// The state of the buttons.
				static bool s_status[s_buttons];


				// Function responsible of handling any mouse button input.
				static void mouse_function( int button , int state , int x , int y );
				// Function responsible of handling the mouse position
				static void mouse_movement_function( int x , int y );
				// Function responsible of cleaning up any allocated memory for the events.
				static void cleanup( const core::Event& event );


			public:

				// The constructor of the class.
				Mouse();
				// The destructor of the class
				~Mouse();


				// Function responsible of performing any setup operations.
				void startup();
				// Function responsible of reforming any cleanup operations.
				void terminate();
				// Function responsible of performing update operations.
				void update();
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_MOUSE_HPP */