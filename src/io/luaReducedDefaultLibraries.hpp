#ifndef ATHENA_IO_LUAREDUCEDDEFAULTLIBRARIES_HPP
#define ATHENA_IO_LUAREDUCEDDEFAULTLIBRARIES_HPP

#include "luaState.hpp"
#include "../definitions.hpp"
#include "../utility/timer.hpp"



namespace athena
{

	namespace io
	{

		/*
			A class holding a reduced set of functions from the basic and os libraries of lua.
		*/
		class LuaReducedDefaultLibraries
		{
			private:

				// The timer that is used to calculate high precision times.
				static utility::Timer s_timer;
				// An array holding the name of each function and a pointer to the function.
				static luaL_Reg s_functions[26];

				/*
					Functions mirroring the functionality of the functions with the same name from Lua's base and os libraries.
					They represent a cut down version of those libraries, in order to disable operations that a script should not have.
					The code used by these functions has been copied by the source files of those two libraries.
				*/
				static int assert( lua_State* state );
				static int error( lua_State* state );
				static int getmetatable( lua_State* state );
				static int ipairs( lua_State* state );
				static int next( lua_State* state );
				static int pairs( lua_State* state );
				static int print_error( lua_State* state );
				static int print_warning( lua_State* state );
				static int print( lua_State* state );
				static int rawequal( lua_State* state );
				static int rawlen( lua_State* state );
				static int rawget( lua_State* state );
				static int rawset( lua_State* state );
				static int select( lua_State* state );
				static int setmetatable( lua_State* state );
				static int tonumber( lua_State* state );
				static int tostring( lua_State* state );
				static int toboolean( lua_State* state );
				static int type( lua_State* state );
				static int clock( lua_State* state );
				static int date( lua_State* state );
				static int difftime( lua_State* state );
				// Function returning the current time. The _timer variable is being used in order to provide with high precision timing.
				static int time( lua_State* state );


				// The default constructor. Declared as private to disable instance of the class.
				LuaReducedDefaultLibraries();
				// The destructor. Declared as private to disable instances of the class.
				~LuaReducedDefaultLibraries();


			public:

				// Function used to load the library to the given state. Mirrors the functionality of lua_openbase. Code has been copied from lua_openbase.
				static int open_reducedlibraries( lua_State* state );
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_LUAREDUCEDDEFAULTLIBRARIES_HPP */
