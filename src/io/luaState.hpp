#ifndef ATHENA_IO_LUASTATE_HPP
#define ATHENA_IO_LUASTATE_HPP

#include "../definitions.hpp"
#include <string>
#include <cstdarg>
#include <vector>
#include <cmath>
#include "../windowsDefinitions.hpp"

#ifdef _WIN32

	#include <lua/src/luaconf.h>
	#include <lua/src/lua.hpp>

#else

	#include <luaconf.h>
	#include <lua.hpp>

#endif /* _WIN32 */



namespace athena
{

	namespace io
	{

		/*
		Type definitions.
		*/
		// A type definitions of a function that can called with access to the lua state.
		typedef int (*lua_function) ( lua_State* state , const unsigned int parameter_count , va_list parameters );


		/*
		A class representing and handling a Lua state.
		*/
		class LuaState
		{
			private:

				// A variable containing a pointer to the lua_State struct which handles the state.
				lua_State* m_state;
				// A variable containing a pointer needed by the new_state function, and is passed to the memory allocation function.
				unsigned int m_ud;
				// A variable containing a pointer to the allocation function used by the state.
				lua_Alloc m_allocation_function;
				// A variable containing a pointer to the user-defined panic function.
				lua_CFunction m_panic_function;
				// A variable containing a pointer to the default panic function.
				lua_CFunction m_default_panic_function;


				// Static function used to allocate memory by the state.
				static void* allocation( void* ud , void* ptr , size_t old_size , size_t new_size );


			public:

				// Function returning the current state of the stack for the given state and emptying the stack.
				static std::vector<std::string> get_stack( lua_State* _state );
				// Function returning whether the contents of the stack at the given position is a boolean and what the value is.
				static bool get_boolean( lua_State* _state , const int location , bool& value );
				// Function returning whether the contents of the stack at the given position is an integer and what the value is.
				static bool get_integer( lua_State* _state , const int location , lua_Integer& value );
				// Function returning whether the contents of the stack at the given position is an unsigned integer and what the value is.
				static bool get_unsigned_integer( lua_State* _state , const int location , lua_Unsigned& value );
				// Function returning whether the contents of the stack at the given position is a floating point number and what the value is.
				static bool get_floating_point( lua_State* _state , const int location , lua_Number& value );
				// Function returning whether the contents of the stack at the given position is a string and what the value is.
				static bool get_string( lua_State* _state , const int location , std::string& value );
				// Function returning whether the contents of the stack at the given position is a function and what the value is.
				static bool get_function( lua_State* _state , const int location , lua_CFunction& value );


				// The default constructor.
				LuaState();
				// The destructor.
				~LuaState();


				// Function responsible of changing the allocation function of the state.
				void allocation_function( const lua_Alloc function );
				// Function responsible of changing the panic function of the state.
				void panic_function( const lua_CFunction function );


				// Function returning the allocation function of the state.
				lua_Alloc allocation_function() const;
				// Function returning the panic function of the state.
				lua_CFunction panic_function() const;
				// Function returning the current state of the stack.
				std::vector<std::string> stack_dump();
				// Function returning whether the state has been initialised or not.
				bool initialised();


				// Function responsible of creating a global variable of the given name and assigning a nil value to it.
				void load_nil( const std::string& name );
				// Function responsible of creating a global variable of the given name and assigning a boolean value to it.
				void load_boolean( const std::string& name , const bool value );
				// Function responsible of creating a global variable of the given name and assigning the given integer value to it.
				void load_integer( const std::string& name , const lua_Integer value );
				// Function responsible of creating a global variable of the given name and assigning the given unsigned integer value to it.
				void load_unsigned_integer( const std::string& name , const lua_Unsigned value );
				// Function responsible of creating a global variable of the given name and assigning the given floating point number value to it.
				void load_floating_point( const std::string& name , const lua_Number value );
				// Function responsible of creating a global variable of the given name and assigning the given string to it.
				void load_string( const std::string& name , const std::string& value );
				// Function responsible of creating a global variable of the given name and assigning the given function to it.
				void load_function( const std::string& name , const lua_CFunction value );
				// Function responsible of loading a nil value to the stack.
				void load_nil();
				// Function responsible of loading a boolean to the stack.
				void load_boolean( const bool value );
				// Function responsible of loading an integer to the stack.
				void load_integer( const lua_Integer value );
				// Function responsible of loading an unsigned integer to the stack.
				void load_unsigned_integer( const lua_Unsigned value );
				// Function responsible of loading a floating point to the stack.
				void load_floating_point( const lua_Number value );
				// Function responsible of loading a string to the stack.
				void load_string(  const std::string& value );
				// Function responsible of loading a function to the stack.
				void load_function(  const lua_CFunction value );
				// Function responsible of loading the basic library.
				void load_basic_library();
				// Function responsible of loading the package library.
				void load_package_library();
				// Function responsible of loading the coroutine library.
				void load_coroutine_library();
				// Function responsible of loading the string library.
				void load_string_library();
				// Function responsible of loading the table library.
				void load_table_library();
				// Function responsible of loading the mathematical library.
				void load_math_library();
				// Function responsible of loading the bitwise library.
				void load_bitwise_library();
				// Function responsible of loading the IO library.
				void load_io_library();
				// Function responsible of loading the OS library.
				void load_os_library();
				// Function responsible of loading the debug library.
				void load_debug_library();
				// Function responsible of loading all the default libraries.
				void load_all_libraries();
				// Function responsible of a library by running the given function and registering the library with the given name. If the global variable is true, a global variable holding the library is created.
				void load_library( const std::string& name , const lua_CFunction open_function , const bool global = true );
				// Function responsible of popping values from the stack.
				void pop_values( const int amount );


				// Function returning a global variable with the given name as a boolean. Returns false if the variable is not a boolean or is nil.
				bool get_boolean( const std::string& name );
				// Function returning a global variable with the given name as an integer. Returns 0 if the variable is not an integer or is nil.
				lua_Integer get_integer( const std::string& name );
				// Function returning a global variable with the given name as an unsigned integer. Returns 0 if the variable is not an unsigned integer or is nil.
				lua_Unsigned get_unsigned_integer( const std::string& name );
				// Function returning a global variable with the given name as a floating point number. Returns 0 if the variable is not a floating point number or is nil.
				lua_Number get_floating_point( const std::string& name );
				// Function returning a global variable with the given name as a string. Returns an empty string if the variable is not a string or is nil.
				std::string get_string( const std::string& name );
				// Function returning a global variable with the given name as a C function. Returns NULL if the variable is not a C function or is nil.
				lua_CFunction get_function( const std::string& name );
				// Function returning the variable at the given position as a boolean. Returns false if the variable is not a boolean or is nil.
				bool get_boolean( const int location = -1 );
				// Function returning the variable at the given position  as an integer. Returns 0 if the variable is not an integer or is nil.
				lua_Integer get_integer( const int location = -1 );
				// Function returning the variable at the given position  as an unsigned integer. Returns 0 if the variable is not an unsigned integer or is nil.
				lua_Unsigned get_unsigned_integer( const int location = -1 );
				// Function returning the variable at the given position as a floating point number. Returns 0 if the variable is not a floating point number or is nil.
				lua_Number get_floating_point( const int location = -1 );
				// Function returning the variable at the given position as a string. Returns an empty string if the variable is not a string or is nil.
				std::string get_string( const int location = -1 );
				// Function returning the variable at the given position as a C function. Returns NULL if the variable is not a C function or is nil.
				lua_CFunction get_function( const int location = -1 );


				// Function responsible of loading the string to the state and executing it.
				int run_string( const std::string& input );
				// Function responsible of loading a file to the state and executing it.
				int run_file( const std::string& file );
				// Function responsible of pushing a c function to the stack of the state and executing it.
				int run_function( lua_CFunction function , const int arguments = 0 , const int results = LUA_MULTRET );
				// Function responsible of calling the given function with the given parameters and parameter count.
				int run_function( lua_function function , const unsigned int parameter_count , ... );


				// Function responsible of creating the state.
				bool create();
				// Function responsible of destroying the state.
				bool destroy();
		};

	} /* io */

} /* athena */

#include "luaState.inl"



#endif /* ATHENA_IO_LUASTATE_HPP */