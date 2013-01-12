#include <sstream>
#include "luaState.hpp"



namespace athena
{

	namespace io
	{

		// Static function used to allocate memory by the state.
		void* LuaState::allocation( void* , void* ptr , size_t , size_t new_size )
		{
			void* return_value = NULL;


			if ( new_size == 0 )
				free(ptr);
			else
				return_value = realloc(ptr,new_size);


			return return_value;
		}


		// The default constructor.
		LuaState::LuaState() :
			m_state(NULL) ,
			m_ud(0) ,
			m_allocation_function(NULL) ,
			m_panic_function(NULL) ,
			m_default_panic_function(NULL)
		{
		}

		// The destructor.
		LuaState::~LuaState()
		{
			if ( initialised() )
				destroy();
		}


		// Function returning the current state of the stack for the given state and emptying the stack.
		std::vector<std::string> LuaState::get_stack( lua_State* _state )
		{
			std::vector<std::string> return_value(0);


			if ( _state != NULL )
			{
				std::stringstream buffer;
				int top = lua_gettop(_state);
				int counter = top;


				while( counter > 0 )
				{
					buffer.flush();
					buffer.str("");

					switch ( lua_type(_state,counter) )
					{
						case LUA_TNIL:

							buffer << "NIL";
							break;
					
						case LUA_TBOOLEAN:
						{
							bool	value = false;


							if ( get_boolean(_state,counter,value ) )
								buffer << (std::boolalpha) << value << (std::noboolalpha);

							break;
						}

						case LUA_TNUMBER:
						{
							lua_Number	value = 0;


							if ( get_floating_point(_state,counter,value) )
								buffer << value;

							break;
						}

						case LUA_TSTRING:
						{
							std::string	value("");


							if ( get_string(_state,counter,value) )
								buffer << value;

							break;
						}

						case LUA_TFUNCTION:
						{
							lua_CFunction	value = NULL;


							if ( get_function(_state,counter,value) )
								buffer << "Function: " << value;

							break;
						}

						case LUA_TTABLE:

							buffer << "Table: " << lua_topointer(_state,counter);
							break;
					
						case LUA_TUSERDATA:

							buffer << "Userdata: " << lua_touserdata(_state,counter);
							break;
					
						case LUA_TLIGHTUSERDATA:

							buffer << "Light Userdata: " << lua_touserdata(_state,counter);
							break;
					
						case LUA_TTHREAD:

							buffer << "Thread: " << lua_tothread(_state,counter); 
							break;
					
						default:

							buffer << "Unknown.";
							break;
					}

					return_value.push_back(buffer.str());
					--counter;
				}

				lua_pop(_state,top);
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is a boolean and what the value is.
		bool LuaState::get_boolean( lua_State* _state , const int location , bool& value )
		{
			bool return_value = false;


			if ( lua_isboolean(_state,location) )
			{
				value = ( lua_toboolean(_state,location) > 0  ?  true : false );
				return_value = true;
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is an integer and what the value is.
		bool LuaState::get_integer( lua_State* _state , const int location , lua_Integer& value )
		{
			lua_Integer result = 0;
			int success = 0;
			bool return_value = false;


			result = lua_tointegerx(_state,location,&success);

			if ( success > 0 )
			{
				value = result;
				return_value = true;
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is an unsigned integer and what the value is.
		bool LuaState::get_unsigned_integer( lua_State* _state , const int location , lua_Unsigned& value )
		{
			lua_Unsigned result = 0;
			int success = 0;
			bool return_value = false;


			result = lua_tounsignedx(_state,location,&success);

			if ( success > 0 )
			{
				value = result;
				return_value = true;
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is a floating point number and what the value is.
		bool LuaState::get_floating_point( lua_State* _state , const int location , lua_Number& value )
		{
			lua_Number result = 0;
			int success = 0;
			bool return_value = false;


			result = lua_tonumberx(_state,location,&success);

			if ( success > 0 )
			{
				value = result;
				return_value = true;
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is a string and what the value is.
		bool LuaState::get_string( lua_State* _state , const int location , std::string& value )
		{
			bool return_value = false;


			if ( lua_isstring(_state,location) > 0 )
			{
				const char* result = lua_tolstring(_state,location,NULL);



				if ( result != NULL )
				{
					value = result;
					return_value = true;
				}
			}


			return return_value;
		}

		// Function returning whether the contents of the stack at the given position is a function and what the value is.
		bool LuaState::get_function( lua_State* _state , const int location , lua_CFunction& value )
		{
			bool return_value = false;


			if ( lua_iscfunction(_state,location) > 0 )
			{
				lua_CFunction result = lua_tocfunction(_state,location);



				if ( result != NULL )
				{
					value = result;
					return_value = true;
				}
			}


			return return_value;
		}


		// Function responsible of changing the panic function of the state.
		void LuaState::panic_function( const lua_CFunction function )
		{
			m_panic_function = function;

			if ( initialised() )
			{
				if ( function == NULL )
				{
					if ( m_default_panic_function != NULL )
					{
						lua_atpanic(m_state,m_default_panic_function);
						m_panic_function = m_default_panic_function;
					}
				}
				else
					lua_atpanic(m_state,function);
			}
		}


		// Function returning a global variable with the given name as a boolean. Returns false if the variable is not a boolean or is nil.
		bool LuaState::get_boolean( const std::string& name )
		{
			bool return_value = false;


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_boolean(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning a global variable with the given name as an integer. Returns 0 if the variable is not an integer or is nil.
		lua_Integer LuaState::get_integer( const std::string& name )
		{
			lua_Integer return_value = 0;


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_integer(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning a global variable with the given name as an unsigned integer. Returns 0 if the variable is not an unsigned integer or is nil.
		lua_Unsigned LuaState::get_unsigned_integer( const std::string& name )
		{
			lua_Unsigned return_value = 0;


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_unsigned_integer(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning a global variable with the given name as a floating point number. Returns 0 if the variable is not a floating point number or is nil.
		lua_Number LuaState::get_floating_point( const std::string& name )
		{
			lua_Number return_value = 0;


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_floating_point(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning a global variable with the given name as a string. Returns an empty string if the variable is not a string or is nil.
		std::string LuaState::get_string( const std::string& name )
		{
			std::string return_value("");


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_string(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning a global variable with the given name as a C function. Returns NULL if the variable is not a C function or is nil.
		lua_CFunction LuaState::get_function( const std::string& name )
		{
			lua_CFunction return_value = NULL;


			if ( initialised() )
			{
				lua_getglobal(m_state,name.c_str());
				get_function(m_state,-1,return_value);
				lua_pop(m_state,1);
			}


			return return_value;
		}

		// Function returning the variable at the given position as a boolean. Returns false if the variable is not a boolean or is nil.
		bool LuaState::get_boolean( const int location )
		{
			bool return_value = false;


			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_boolean(m_state,location,return_value);
			}


			return return_value;
		}

		// Function returning the variable at the given position  as an integer. Returns 0 if the variable is not an integer or is nil.
		lua_Integer LuaState::get_integer( const int location )
		{
			lua_Integer return_value = 0;



			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_integer(m_state,location,return_value);
			}


			return return_value;
		}

		// Function returning the variable at the given position  as an unsigned integer. Returns 0 if the variable is not an unsigned integer or is nil.
		lua_Unsigned LuaState::get_unsigned_integer( const int location )
		{
			lua_Unsigned return_value = 0;


			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_unsigned_integer(m_state,location,return_value);
			}


			return return_value;
		}

		// Function returning the variable at the given position as a floating point number. Returns 0 if the variable is not a floating point number or is nil.
		lua_Number LuaState::get_floating_point( const int location )
		{
			lua_Number return_value = 0;


			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_floating_point(m_state,location,return_value);
			}


			return return_value;
		}

		// Function returning the variable at the given position as a string. Returns an empty string if the variable is not a string or is nil.
		std::string LuaState::get_string( const int location )
		{
			std::string return_value("");


			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_string(m_state,location,return_value);
			}


			return return_value;
		}

		// Function returning the variable at the given position as a C function. Returns NULL if the variable is not a C function or is nil.
		lua_CFunction LuaState::get_function( const int location )
		{
			lua_CFunction return_value = NULL;


			if ( initialised() )
			{
				int top = lua_gettop(m_state);


				if ( std::abs(top) >= std::abs(location) )
					get_function(m_state,location,return_value);
			}


			return return_value;
		}


		// Function responsible of loading the string to the state and executing it.
		int LuaState::run_string( const std::string& input )
		{
			int return_value = LUA_ERRRUN;


			if ( initialised() )
			{
				return_value = luaL_loadstring(m_state,input.c_str());

				if ( return_value == LUA_OK )
					return_value = lua_pcallk(m_state,0,0,0,0,NULL);
			}


			return return_value;
		}

		// Function responsible of loading a file to the state and executing it.
		int LuaState::run_file( const std::string& file )
		{
			int return_value = LUA_ERRFILE;


			if ( initialised() )
			{
				return_value = luaL_loadfilex(m_state,file.c_str(),"t");

				if ( return_value == LUA_OK )
					return_value = lua_pcallk(m_state,0,0,0,0,NULL);
			}


			return return_value;
		}

		// Function responsible of pushing a c function to the stack of the state and executing it.
		int LuaState::run_function( lua_CFunction function , const int arguments , const int results )
		{
			int return_value = 0;


			if ( initialised() )
			{
				int position = lua_gettop(m_state);


				lua_pushcclosure(m_state,function,0);

				if ( arguments > 0 )
					lua_insert(m_state,std::max(position-arguments,1));

				return_value = lua_pcallk(m_state,std::abs(arguments),std::abs(results),0,0,NULL);
			}


			return return_value;
		}

		// Function responsible of calling the given function with the given parameters and parameter count.
		int LuaState::run_function( lua_function function , const unsigned int parameter_count , ... )
		{
			int return_value = 0;


			if ( initialised() )
			{
				va_list args;


				va_start(args,parameter_count);
				return_value = function(m_state,parameter_count,args);
				va_end(args);
			}


			return return_value;
		}


		// Function responsible of creating the state.
		bool LuaState::create()
		{
			bool return_value = true;


			if ( !initialised() )
			{
				if ( m_allocation_function == NULL )
					m_allocation_function = allocation;

				m_state = lua_newstate(m_allocation_function,static_cast<void*>(&m_ud));

				if ( m_state == NULL )
					return_value = false;
				else
				{
					if ( m_panic_function != NULL )
						m_default_panic_function = lua_atpanic(m_state,m_panic_function);
				}
			}


			return return_value;
		}

		// Function responsible of destroying the state.
		bool LuaState::destroy()
		{
			bool return_value = true;



			if ( initialised() )
			{
				lua_close(m_state);
				m_state = NULL;
			}


			return return_value;
		}

	} /* io */

} /* athena */