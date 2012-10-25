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
				null  = 0 , 
				boolean , 
				character ,
				shortInteger , 
				integer , 
				longInteger , 
				real , 
				doubleReal , 
				pointer
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


					ATHENA_DLL ATHENA_CALL Event( const unsigned long& id = 0 , const unsigned long& parameters = 0 );
					ATHENA_DLL ATHENA_CALL ~Event();


					ATHENA_DLL void ATHENA_CALL parameter( const unsigned long& index , const Parameter& parameter );
					ATHENA_DLL void ATHENA_CALL id( const unsigned long& id );
					ATHENA_DLL void ATHENA_CALL clear();

					ATHENA_DLL Parameter ATHENA_CALL parameter( const unsigned long& index ) const;
					ATHENA_DLL unsigned long ATHENA_CALL id() const;
					ATHENA_DLL unsigned long ATHENA_CALL parameter_count() const;
			};

		} /* core */

	} /* athena */


	#include "event.inl"



#endif /* ATHENA_CORE_EVENT_HPP */