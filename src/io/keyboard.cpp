#include "keyboard.hpp"
#include <GL/freeglut.h>
#include <limits>
#include <cstring>
#include "../athena.hpp"

#ifdef _WIN32
	#pragma warning(disable:4505)
#endif /* _WIN32 */

/*
 * GLUT API macro definitions -- additional keyboard and joystick definitions
 */
//#define  GLUT_KEY_REPEAT_OFF                0x0000
//#define  GLUT_KEY_REPEAT_ON                 0x0001
//#define  GLUT_KEY_REPEAT_DEFAULT            0x0002
//
//#define  GLUT_JOYSTICK_BUTTON_A             0x0001
//#define  GLUT_JOYSTICK_BUTTON_B             0x0002
//#define  GLUT_JOYSTICK_BUTTON_C             0x0004
//#define  GLUT_JOYSTICK_BUTTON_D             0x0008

namespace athena
{

	namespace io
	{

		// An enumeration that is used to handle the key codes.
		enum KeyCodes
		{
			Shift = 0,
			Control ,
			Alt ,
			UpArrow ,
			RightArrow ,
			DownArrow ,
			LeftArrow ,
			Backspace ,
			Enter ,
			Return ,
			Home ,
			End ,
			PageUp ,
			PageDown ,
			Delete ,
			Insert ,
			Tab ,
			Spacebar ,
			PrintScreen ,
			Help ,
			Esc , 
			CapsLock ,
			ScrollLock ,
			NumLock ,
			Pause ,
			AppKey ,
			F1 ,
			F2 ,
			F3 ,
			F4 ,
			F5 ,
			F6 ,
			F7 ,
			F8 ,
			F9 ,
			F10 ,
			F11 ,
			F12 ,
			A ,
			B ,
			C ,
			D ,
			E ,
			F ,
			G ,
			H ,
			I ,
			J ,
			K ,
			L ,
			M ,
			N ,
			O ,
			P ,
			Q ,
			R ,
			S ,
			T ,
			U ,
			V ,
			X ,
			Y ,
			Z ,
			N0 ,
			N1 ,
			N2 ,
			N3 ,
			N4 ,
			N5 ,
			N6 ,
			N7 ,
			N8 ,
			N9 ,
			Period ,
			Comma ,
			Minus ,
			Equal ,
			Slash ,
			LeftBracket ,
			RightBracket ,
			ReverseSlash ,
			Semicolon ,
			Apostrophe ,
			Tilde ,
			Num0 ,
			Num1 ,
			Num2 ,
			Num3 ,
			Num4 ,
			Num5 ,
			Num6 ,
			Num7 ,
			Num8 ,
			Num9 ,
			NumAdd ,
			NumSubtract ,
			NumDivive ,
			NumMultiply ,
			NumDecimal ,
			NumEqual ,
			WinKey ,
			MacKey
		};

		// The status of the keys.
		bool Keyboard::m_status[s_keys];


		// Function responsible of finding the key code of the given key.
		unsigned int Keyboard::find_key_code( const unsigned char key )
		{
			unsigned int return_value = std::numeric_limits<unsigned int>::max();


			/*
				Missing:
				ScrollLock ,
				NumLock 
			*/
			if ( key >= 'a'  &&  key <= 'z' )
				return_value = KeyCodes::A + key - 'a';
			else if ( key >= 'A'  &&  key <= 'Z' )
				return_value = KeyCodes::A + key - 'A';
			else if ( key >= '0'  &&  key <= '9' )
				return_value = KeyCodes::N0 + key - '0';
			else if ( key == '!' )
				return_value = KeyCodes::N1;
			else if ( key == '@' )
				return_value = KeyCodes::N2;
			else if ( key == '#' )
				return_value = KeyCodes::N3;
			else if ( key == '$' )
				return_value = KeyCodes::N4;
			else if ( key == '%' )
				return_value = KeyCodes::N5;
			else if ( key == '^' )
				return_value = KeyCodes::N6;
			else if ( key == '&' )
				return_value = KeyCodes::N7;
			else if ( key == '*' )
				return_value = KeyCodes::N8;
			else if ( key == '(' )
				return_value = KeyCodes::N9;
			else if ( key == ')' )
				return_value = KeyCodes::N0;
			else if ( key == '-'  ||  key == '_' )
				return_value = KeyCodes::Minus;
			else if ( key == '='  ||  key == '+' )
				return_value = KeyCodes::Equal;
			else if ( key == ','  ||  key == '<' )
				return_value = KeyCodes::Comma;
			else if ( key == '.'  ||  key == '>' )
				return_value = KeyCodes::Period;
			else if ( key == '/'  ||  key == '?' )
				return_value = KeyCodes::Slash;
			else if ( key == '['  ||  key == '{' )
				return_value = KeyCodes::LeftBracket;
			else if ( key == ']'  ||  key == '}' )
				return_value = KeyCodes::RightBracket;
			else if ( key == '\\'  ||  key == '|' )
				return_value = KeyCodes::ReverseSlash;
			else if ( key == ';'  ||  key == ':' )
				return_value = KeyCodes::Semicolon;
			else if ( key == '\''  ||  key == '"' )
				return_value = KeyCodes::Apostrophe;
			else if ( key == '`'  ||  key == '~' )
				return_value = KeyCodes::Tilde;
			else if ( key == ' ' )
				return_value = KeyCodes::Spacebar;
			else if ( key == 27 )
				return_value = KeyCodes::Esc;
			else if ( key == 127 )
				return_value = KeyCodes::Delete;
			else if ( key == '\t' )
				return_value = KeyCodes::Tab;
			else if ( key == '\b' )
				return_value = KeyCodes::Backspace;
			else if ( key == '\n' || key == '\r' )
				return_value = KeyCodes::Enter;
			else if ( key == 20 )
				return_value = KeyCodes::CapsLock;
			else if ( key == 44 )
				return_value = KeyCodes::PrintScreen;
			else if ( key == 19 )
				return_value = KeyCodes::Pause;
			else if ( key == 91 )
				return_value = KeyCodes::WinKey;
			else if ( key == 93 )
				return_value = KeyCodes::AppKey;


			return return_value;
		}

		// Function responsible of finding the key code of the given special key.
		unsigned int Keyboard::find_special_key_code( const int key )
		{
			unsigned int return_value = std::numeric_limits<unsigned int>::max();


			if ( key == 112 )
				return_value = KeyCodes::Shift;
			else if ( key == 114 )
				return_value = KeyCodes::Control;
			else if ( key == 116 )
				return_value = KeyCodes::Alt;
			else if ( key >= GLUT_KEY_F1  &&  key <= GLUT_KEY_F12 )
				return_value = KeyCodes::F1 + key - GLUT_KEY_F1;
			else if ( key == GLUT_KEY_UP )
				return_value = KeyCodes::UpArrow;
			else if ( key == GLUT_KEY_RIGHT )
				return_value = KeyCodes::RightArrow;
			else if ( key == GLUT_KEY_DOWN )
				return_value = KeyCodes::DownArrow;
			else if ( key == GLUT_KEY_LEFT )
				return_value = KeyCodes::LeftArrow;
			else if ( key == GLUT_KEY_PAGE_UP )
				return_value = KeyCodes::PageUp;
			else if ( key == GLUT_KEY_PAGE_DOWN )
				return_value = KeyCodes::PageDown;
			else if ( key == GLUT_KEY_HOME )
				return_value = KeyCodes::Home;
			else if ( key == GLUT_KEY_END )
				return_value = KeyCodes::End;
			else if ( key == GLUT_KEY_INSERT )
				return_value = KeyCodes::Insert;


			return return_value;
		}

		// Function responsible of handling normal key down input.
		void Keyboard::keyboard_down_function( unsigned char key , int , int )
		{
			unsigned int code = find_key_code(key);


			if ( code <= s_keys )
			{
				if ( !m_status[code] )
				{
					m_status[code] = true;
					athena::trigger_event(core::Event(InputCodes::EVENT_INPUT_KEYBOARD_SHIFT_DOWN + code - KeyCodes::Shift));
				}
			}
		}
		
		// Function responsible of handling normal key up input.
		void Keyboard::keyboard_up_function( unsigned char key , int , int )
		{
			unsigned int code = find_key_code(key);


			if ( code <= s_keys )
			{
				if ( m_status[code] )
				{
					m_status[code] = false;
					athena::trigger_event(core::Event(InputCodes::EVENT_INPUT_KEYBOARD_SHIFT_UP + code - KeyCodes::Shift));
				}
			}
		}

		// Function responsible of handling special key down input.
		void Keyboard::special_key_down_function( int key , int , int )
		{
			unsigned int code = find_special_key_code(key);


			if ( code <= s_keys )
			{
				if ( !m_status[code] )
				{
					m_status[code] = true;
					athena::trigger_event(core::Event(InputCodes::EVENT_INPUT_KEYBOARD_SHIFT_DOWN + code - KeyCodes::Shift));
				}
			}
		}
		
		// Function responsible of handling special key up input.
		void Keyboard::special_key_up_function( int key , int , int )
		{
			unsigned int code = find_special_key_code(key);


			if ( code <= s_keys )
			{
				if ( m_status[code] )
				{
					m_status[code] = false;
					athena::trigger_event(core::Event(InputCodes::EVENT_INPUT_KEYBOARD_SHIFT_UP + code - KeyCodes::Shift));
				}
			}
		}

		// The constructor of the class.
		Keyboard::Keyboard()
		{
			memset(m_status,0,sizeof(bool)*s_keys);
		}

		// The destructor of the class
		Keyboard::~Keyboard()
		{
		}


		// Function responsible of performing any setup operations.
		void Keyboard::startup()
		{
			glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
			glutKeyboardFunc(keyboard_down_function);
			glutKeyboardUpFunc(keyboard_up_function);
			glutSpecialFunc(special_key_down_function);
			glutSpecialUpFunc(special_key_up_function);
		}

		// Function responsible of reforming any cleanup operations.
		void Keyboard::terminate()
		{
			glutKeyboardFunc(NULL);
			glutKeyboardUpFunc(NULL);
			glutSpecialFunc(NULL);
			glutSpecialUpFunc(NULL);
		}

		// Function responsible of performing update operations.
		void Keyboard::update()
		{
		}

	} /* io */

} /* athena */