#ifndef ATHENA_CORE_PARAMETER_HPP
#define ATHENA_CORE_PARAMETER_HPP

#include "definitions.hpp"
#include <cstddef>



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
				ParameterType m_type;
				// A variable pointing to the data of the parameter.
				void* m_data;


			public:

				// The constructor of the class.
				explicit ATHENA_DLL Parameter( const ParameterType& type = Null , void* data = NULL );
				// The destructor of the class.
				ATHENA_DLL ~Parameter();


				// A function responsible of setting the type of the parameter.
				ATHENA_DLL void type( const ParameterType& type );
				// A function responsible of setting the data of the parameter.
				ATHENA_DLL void data( void* data );

					
				// A function returning the type of the parameter.
				ATHENA_DLL ParameterType type() const;
				// A function returning the data of the parameter.
				ATHENA_DLL void* data() const;
		};

	} /* core */

} /* athena */


#include "parameter.inl"



#endif /* ATHENA_CORE_PARAMETER_HPP */