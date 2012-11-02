#ifndef ATHENA_CORE_EVENT_HPP
	#define ATHENA_CORE_EVENT_HPP

	#include "../athenaDefinitions.hpp"
	#include <vector>



	namespace athena
	{

		namespace core
		{

			enum ATHENA_DLL ParameterType
			{
				Null  = 0 , 
				Boolean , 
				Character ,
				ShortInteger , 
				Integer , 
				LongInteger , 
				Real , 
				DoubleReal , 
				Pointer
			};


			struct ATHENA_DLL Parameter
			{
				ParameterType type;
				void* data;
			};


			class Event
			{
				private:

					std::vector<Parameter> _parameters;
					unsigned long _id;


				public:

					ATHENA_DLL static Parameter NullParameter;


					ATHENA_DLL ATHENA_PRECALL Event( const unsigned long& id = 0 , const unsigned long& parameters = 0 ) ATHENA_POSTCALL;
					ATHENA_DLL ATHENA_PRECALL ~Event() ATHENA_POSTCALL;


					ATHENA_DLL void ATHENA_PRECALL parameter( const unsigned long& index , const Parameter& parameter ) ATHENA_POSTCALL;
					ATHENA_DLL void ATHENA_PRECALL id( const unsigned long& id ) ATHENA_POSTCALL;
					ATHENA_DLL void ATHENA_PRECALL clear() ATHENA_POSTCALL;

					ATHENA_DLL Parameter ATHENA_PRECALL parameter( const unsigned long& index ) const ATHENA_POSTCALL;
					ATHENA_DLL unsigned long ATHENA_PRECALL id() const ATHENA_POSTCALL;
					ATHENA_DLL unsigned long ATHENA_PRECALL parameter_count() const ATHENA_POSTCALL;
			};

		} /* core */

	} /* athena */


	#include "event.inl"



#endif /* ATHENA_CORE_EVENT_HPP */