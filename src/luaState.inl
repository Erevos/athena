#ifndef ATHENA_IO_LUASTATE_INL
#define ATHENA_IO_LUASTATE_INL

#ifndef ATHENA_IO_LUASTATE_HPP
	#error "luaState.hpp must be included before luaState.inl"
#endif /* ATHENA_IO_LUASTATE_HPP */



namespace athena
{

	namespace io
	{

		// Function responsible of changing the allocation function of the state.
		inline void LuaState::allocation_function( const lua_Alloc function )
		{
			m_allocation_function = function;
		}

		// Function returning the allocation function of the state.
		inline lua_Alloc LuaState::allocation_function() const
		{
			return m_allocation_function;
		}

		// Function returning the panic function of the state.
		inline lua_CFunction LuaState::panic_function() const
		{
			return m_panic_function;
		}

		// Function returning _state != NULLthe current state of the stack.
		inline std::vector<std::string> LuaState::stack_dump()
		{
			return get_stack(m_state);
		}

		// Function returning whether the state has been initialised or not.
		inline bool LuaState::initialised()
		{
			return ( m_state != NULL );
		}


		// Function responsible of creating a global variable of the given name and assigning a nil value to it.
		inline void LuaState::load_nil( const std::string& name )
		{
			if ( initialised() )
			{
				lua_pushnil(m_state);
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning a boolean value to it.
		inline void LuaState::load_boolean( const std::string& name , const bool value )
		{
			if ( initialised() )
			{
				lua_pushboolean(m_state,( value  ?  1 : 0 ));
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning the given integer value to it.
		inline void LuaState::load_integer( const std::string& name , const lua_Integer value )
		{
			if ( initialised() )
			{
				lua_pushinteger(m_state,value);
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning the given unsigned integer value to it.
		inline void LuaState::load_unsigned_integer( const std::string& name , const lua_Unsigned value )
		{
			if ( initialised() )
			{
				lua_pushunsigned(m_state,value);
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning the given floating point number value to it.
		inline void LuaState::load_floating_point( const std::string& name , const lua_Number value )
		{
			if ( initialised() )
			{
				lua_pushnumber(m_state,value);
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning the given string to it.
		inline void LuaState::load_string( const std::string& name , const std::string& value )
		{
			if ( initialised() )
			{
				lua_pushstring(m_state,value.c_str());
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of creating a global variable of the given name and assigning the given function to it.
		inline void LuaState::load_function( const std::string& name , const lua_CFunction value )
		{
			if ( initialised() )
			{
				lua_pushcclosure(m_state,value,0);
				lua_setglobal(m_state,name.c_str());
			}
		}

		// Function responsible of loading a nil value to the stack.
		inline void LuaState::load_nil()
		{
			if ( initialised() )
				lua_pushnil(m_state);
		}

		// Function responsible of loading a boolean to the stack.
		inline void LuaState::load_boolean( const bool value )
		{
			if ( initialised() )
				lua_pushboolean(m_state,( value  ?  1 : 0 ));
		}

		// Function responsible of loading an integer to the stack.
		inline void LuaState::load_integer( const lua_Integer value )
		{
			if ( initialised() )
				lua_pushinteger(m_state,value);
		}

		// Function responsible of loading an unsigned integer to the stack.
		inline void LuaState::load_unsigned_integer( const lua_Unsigned value )
		{
			if ( initialised() )
				lua_pushunsigned(m_state,value);
		}

		// Function responsible of loading a floating point to the stack.
		inline void LuaState::load_floating_point( const lua_Number value )
		{
			if ( initialised() )
				lua_pushnumber(m_state,value);
		}

		// Function responsible of loading a string to the stack.
		inline void LuaState::load_string(  const std::string& value )
		{
			if ( initialised() )
				lua_pushstring(m_state,value.c_str());
		}

		// Function responsible of loading a function to the stack.
		inline void LuaState::load_function(  const lua_CFunction value )
		{
			if ( initialised() )
				lua_pushcclosure(m_state,value,0);
		}

		// Function responsible of loading the basic library.
		inline void LuaState::load_basic_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,"_G",luaopen_base,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the package library.
		inline void LuaState::load_package_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_LOADLIBNAME,luaopen_package,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the coroutine library.
		inline void LuaState::load_coroutine_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_COLIBNAME,luaopen_coroutine,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the string library.
		inline void LuaState::load_string_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_STRLIBNAME,luaopen_string,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the table library.
		inline void LuaState::load_table_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_TABLIBNAME,luaopen_table,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the mathematical library.
		inline void LuaState::load_math_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_MATHLIBNAME,luaopen_math,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the bitwise library.
		inline void LuaState::load_bitwise_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_BITLIBNAME,luaopen_bit32,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the IO library.
		inline void LuaState::load_io_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_IOLIBNAME,luaopen_io,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the OS library.
		inline void LuaState::load_os_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_OSLIBNAME,luaopen_os,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading the debug library.
		inline void LuaState::load_debug_library()
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,LUA_DBLIBNAME,luaopen_debug,1);
				lua_pop(m_state,1);
			}
		}

		// Function responsible of loading all the default libraries.
		inline void LuaState::load_all_libraries()
		{
			if ( initialised() )
				luaL_openlibs(m_state);
		}

		// Function responsible of a library by running the given function and registering the library with the given name. If the global variable is true, a global variable holding the library is created.
		inline void LuaState::load_library( const std::string& name , const lua_CFunction open_function , const bool global )
		{
			if ( initialised() )
			{
				luaL_requiref(m_state,name.c_str(),open_function,( global  ?  1 : 0 ));
				lua_pop(m_state,1);
			}
		}

		// Function responsible of popping values from the stack.
		inline void LuaState::pop_values( const int amount )
		{
			if ( initialised() )
				lua_pop(m_state,std::abs(amount));
		}

	} /* io */

} /* athena */



#endif /* ATHENA_IO_LUASTATE_INL */