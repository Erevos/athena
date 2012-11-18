#ifndef ATHENA_UTILITY_TIMER_HPP
	#define ATHENA_UTILITY_TIMER_HPP

	#include "../athenaDefinitions.hpp"
	#include "readersWriterLock.hpp"

	#ifdef _WIN32
		#include <Windows.h>
	#else
		#include <ctime>
	#endif /* _WIN32 */



	namespace athena
	{

		namespace utility
		{

			// A definition that defines the type that is used by the timer to return time values.
			typedef double TimerValueType;


			/*
				A class representing and managing a timer class.
			*/
			class Timer
			{
				private:

					// A lock that is used to handle concurrency issues.
					ReadersWriterLock _lock;
					// A variable holding the reverse frequency of the timer to speedup calculations.
					TimerValueType _reverse_frequency;
					// A variable holding the current time value.
					unsigned long long _start_time;
					// A variable holding the current time value.
					unsigned long long _current_time;
					// A variable holding the frequency of the timer.
					unsigned long long _frequency;
					// A variable holding the pause state of the timer.
					bool _paused;
					

					// A function responsible of getting the current time.
					static unsigned long long ATHENA_PRECALL _get_time() ATHENA_POSTCALL;
					// A function responsible of getting the frequency.
					static unsigned long long ATHENA_PRECALL _get_frequency() ATHENA_POSTCALL;


				public:

					// The constructor of the class.
					ATHENA_DLL ATHENA_PRECALL Timer() ATHENA_POSTCALL;
					// The destructor of the class.
					ATHENA_DLL ATHENA_PRECALL ~Timer() ATHENA_POSTCALL;


					// A function responsible of starting/resetting the timer.
					ATHENA_DLL void ATHENA_PRECALL start() ATHENA_POSTCALL;
					// A function responsible of pausing the timer.
					ATHENA_DLL void ATHENA_PRECALL pause() ATHENA_POSTCALL;
					// A function responsible of resuming the timer if it is paused.
					ATHENA_DLL void ATHENA_PRECALL resume() ATHENA_POSTCALL;


					// A function returning the current time in seconds.
					ATHENA_DLL TimerValueType ATHENA_PRECALL seconds() ATHENA_POSTCALL;
					// A function returning the current time in milliseconds.
					ATHENA_DLL TimerValueType ATHENA_PRECALL milliseconds() ATHENA_POSTCALL;
					// A function returning the difference since the last call in seconds.
					ATHENA_DLL TimerValueType ATHENA_PRECALL difference_in_seconds() ATHENA_POSTCALL;
					// A function returning the difference since the last call in milliseconds.
					ATHENA_DLL TimerValueType ATHENA_PRECALL difference_in_milliseconds() ATHENA_POSTCALL;
					// A function returning the current time of the timer.
					ATHENA_DLL unsigned long long ATHENA_PRECALL time() ATHENA_POSTCALL;
					// A function returning the frequency of the timer.
					ATHENA_DLL unsigned long long ATHENA_PRECALL frequency() ATHENA_POSTCALL;
					// A function returning whether the timer is paused or not.
					ATHENA_DLL bool ATHENA_PRECALL is_paused() ATHENA_POSTCALL;
			};

		} /* utility */

	} /* athena */


	#include "timer.inl"



#endif /* ATHENA_UTILITY_TIMER_HPP */