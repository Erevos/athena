#ifndef ATHENA_UTILITY_TIMER_INL
#define ATHENA_UTILITY_TIMER_INL

#ifndef ATHENA_UTILITY_TIMER_HPP
	#error "timer.hpp must be included before timer.inl"
#endif /* ATHENA_UTILITY_TIMER_HPP */


namespace athena
{

	namespace utility
	{

		// A function responsible of pausing the timer.
		inline void Timer::pause()
		{
			m_paused = true;
		}

		// A function responsible of resuming the timer if it is paused.
		inline void Timer::resume()
		{
			m_paused = false;
		}


		#ifndef _WIN32

			// A function returning the current time in milliseconds.
			inline TimerValueType Timer::seconds()
			{
				return nanoseconds()*0.000000001;
			}

		#endif /* _WIN32 */

		// A function returning the current time in milliseconds.
		inline TimerValueType Timer::milliseconds()
		{
			#ifdef _WIN32
				return seconds()*1000.0;
			#else
				return nanoseconds()*0.000001;
			#endif /* _WIN32 */
		}

		// A function returning the current time in milliseconds.
		inline TimerValueType Timer::microseconds()
		{
			#ifdef _WIN32
				return seconds()*1000000.0;
			#else
				return nanoseconds()*0.001;
			#endif /* _WIN32 */
		}

		#ifdef _WIN32

			// A function returning the current time in milliseconds.
			inline TimerValueType Timer::nanoseconds()
			{
				return seconds()*1000000000.0;
			}

		#endif /* _WIN32 */

		#ifndef _WIN32

			// A function returning the difference since the last call in milliseconds.
			inline TimerValueType Timer::difference_in_seconds()
			{
				return difference_in_nanoseconds()*0.000000001;
			}

		#endif /* _WIN32 */

		// A function returning the difference since the last call in milliseconds.
		inline TimerValueType Timer::difference_in_milliseconds()
		{
			#ifdef _WIN32
				return difference_in_seconds()*1000.0;
			#else
				return difference_in_nanoseconds()*0.000001;
			#endif /* _WIN32 */
		}

		// A function returning the difference since the last call in milliseconds.
		inline TimerValueType Timer::difference_in_microseconds()
		{
			#ifdef _WIN32
				return difference_in_seconds()*1000000.0;
			#else
				return difference_in_nanoseconds()*0.001;
			#endif /* _WIN32 */
		}

		#ifdef _WIN32

			// A function returning the difference since the last call in milliseconds.
			inline TimerValueType Timer::difference_in_nanoseconds()
			{
				return difference_in_seconds()*1000000000.0;
			}

		#endif /* _WIN32 */

		// A function returning whether the timer is paused or not.
		inline bool Timer::is_paused()
		{
			return m_paused;
		}

	} /* utility */

} /* athena */



#endif /* ATHENA_UTILITY_TIMER_INL */