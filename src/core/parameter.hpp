#ifndef ATHENA_CORE_PARAMETER_HPP
	#define ATHENA_CORE_PARAMETER_HPP

	#include "../athenaDefinitions.hpp"



	namespace athena
	{

		namespace core
		{

			// An enumeration containing all the possible data types.
			enum ATHENA_DLL ParameterType
			{
				Null  = 0 , 
				Boolean , 
				Character ,
				UnsignedCharacter , 
				WideCharacter , 
				ShortInteger , 
				UnsignedShortInteger , 
				Integer , 
				UnsignedInteger , 
				LongInteger , 
				UnsignedLongInteger , 
				Real , 
				DoubleReal , 
				Pointer
			};


			/*
				A class representing and handling a parameter of an event.
			*/
			class Parameter
			{
				public:

					/*
						Due to the fact that this class should have fast access to the 
						type and data of the parameter, public access to the type and data
						is recommended since the class will be used extensively by the event system
						and using the set/get functions can add up to a significant overhead.
						However set/get functions are provided if the "typical" OO model is preferred.
					*/
					// A variable holding the type of the parameter.
					ParameterType _type;
					// A variable pointing to the data of the parameter.
					void* _data;


				public:

					// The constructor of the class.
					explicit ATHENA_DLL ATHENA_PRECALL Parameter( const ParameterType& type = Null , void* data = INVALID_POINTER ) ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~Parameter() ATHENA_POSTCALL;


					// A function responsible of setting the type of the parameter.
					ATHENA_DLL void ATHENA_PRECALL type( const ParameterType& type ) ATHENA_POSTCALL;
					// A function responsible of setting the data of the parameter.
					ATHENA_DLL void ATHENA_PRECALL data( void* data ) ATHENA_POSTCALL;

					
					// A function returning the type of the parameter.
					ATHENA_DLL ParameterType ATHENA_PRECALL type() const ATHENA_POSTCALL;
					// A function returning the data of the parameter.
					ATHENA_DLL void* ATHENA_PRECALL data() const ATHENA_POSTCALL;
			};

		} /* core */

	} /* athena */


	#include "parameter.inl"



#endif /* ATHENA_CORE_PARAMETER_HPP */