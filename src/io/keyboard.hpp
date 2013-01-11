#ifndef ATHENA_IO_KEYBOARD_HPP
#define ATHENA_IO_KEYBOARD_HPP

#include "../definitions.hpp"
#include "inputDevice.hpp"



namespace athena
{

	namespace io
	{

		/*
			A class representing and handling any keyboard input.
		*/
		class Keyboard : public InputDevice
		{
			private:

				// The amount of keys.
				static const unsigned int s_keys = 256;
				// The status of the keys.
				static bool m_status[s_keys];


				// Function responsible of finding the key code of the given key.
				static unsigned int find_key_code( const unsigned char key );
				// Function responsible of finding the key code of the given special key.
				static unsigned int find_special_key_code( const int key );
				// Function responsible of handling normal key down input.
				static void keyboard_down_function( unsigned char key , int x, int y );
				// Function responsible of handling normal key up input.
				static void keyboard_up_function( unsigned char key , int x, int y );
				// Function responsible of handling special key down input.
				static void special_key_down_function( int key , int x , int y );
				// Function responsible of handling special key up input.
				static void special_key_up_function( int key , int x , int y );


			public:

				// The constructor of the class.
				Keyboard();
				// The destructor of the class
				~Keyboard();


				// Function responsible of performing any setup operations.
				void startup();
				// Function responsible of reforming any cleanup operations.
				void terminate();
				// Function responsible of performing update operations.
				void update();
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_KEYBOARD_HPP */