#ifndef ATHENA_UTILITY_TIMER_INL
	#define ATHENA_UTILITY_TIMER_INL



	namespace athena
	{

		namespace utility
		{

			// A function returning the current time in milliseconds.
			inline TimerValueType Timer::milliseconds()
			{
				return seconds()*1000.0;
			};

			// A function returning the difference since the last call in milliseconds.
			inline TimerValueType Timer::difference_in_milliseconds()
			{
				return difference_in_seconds()*1000.0;
			};

		} /* utility */

	} /* athena */



#endif /* ATHENA_UTILITY_TIMER_INL */