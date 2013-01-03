#include "timer.hpp"



namespace athena
{

	namespace utility
	{

		// A function responsible of getting the current time.
		unsigned long long Timer::get_time()
		{
			unsigned long long return_value = 0;


			#ifdef _WIN32

				LARGE_INTEGER temp = { 0 };


				QueryPerformanceCounter(&temp);
				return_value = temp.QuadPart;
				
			#else

				timespec temp = { 0 , 0 };


				clock_gettime(CLOCK_REALTIME,&temp);
				return_value = temp.tv_sec*1000000000 + temp.tv_nsec;

			#endif /* _WIN32 */


			return return_value;
		}

		#ifdef _WIN32

			// A function responsible of getting the frequency.
			unsigned long long Timer::get_frequency()
			{
				unsigned long long return_value = 1;
				LARGE_INTEGER temp = { 1 };

				
				QueryPerformanceFrequency(&temp);

				if ( temp.QuadPart != 0 )
					return_value = temp.QuadPart;


				return return_value;
			}

		#endif /* _WIN32 */


		// The constructor of the class.
		Timer::Timer() :
			m_lock() ,
			
			#ifdef _WIN32
				m_reverse_frequency(1) ,
			#endif /* _WIN32 */
			
			m_start_time(0) ,
			m_current_time(0) ,

			#ifdef _WIN32
				m_frequency(1) ,
			#endif /* _WIN32 */

			m_paused(false)
		{
		}

		// The destructor of the class.
		Timer::~Timer()
		{
		}


		// A function responsible of starting the timer.
		void Timer::start()
		{
			m_lock.lock();
			m_current_time = get_time();
			m_start_time = m_current_time;

			#ifdef _WIN32
				m_frequency = get_frequency();
				m_reverse_frequency = static_cast<TimerValueType>(1)/static_cast<TimerValueType>(m_frequency);
			#endif /* _WIN32 */

			m_paused = false;
			m_lock.unlock();
		}

		// A function responsible of pausing the timer.
		void Timer::pause()
		{
			m_lock.lock();
			m_paused = true;
			m_lock.unlock();
		}

		// A function responsible of resuming the timer if it is paused.
		void Timer::resume()
		{
			m_lock.lock();
			m_paused = false;
			m_lock.unlock();
		}


		#ifdef _WIN32

			// A function returning the current time in seconds.
			TimerValueType Timer::seconds()
			{
				TimerValueType return_value = 0;


				m_lock.lock();

				if ( !m_paused )
					m_current_time = get_time();

				return_value = static_cast<TimerValueType>(m_current_time - m_start_time)*m_reverse_frequency;
				m_lock.unlock();


				return return_value;
			}

			// A function returning the difference since the last call in seconds.
			TimerValueType Timer::difference_in_seconds()
			{
				TimerValueType return_value = 0;
				unsigned long long current_time = 0;


				m_lock.lock();

				if ( !m_paused )
					current_time = get_time();
				else
					current_time = m_current_time;

				return_value = static_cast<TimerValueType>(current_time - m_current_time)*m_reverse_frequency;
				m_current_time = current_time;
				m_lock.unlock();


				return return_value;
			}

		#else

			// A function returning the current time in seconds.
			TimerValueType Timer::nanoseconds()
			{
				TimerValueType return_value = 0;


				m_lock.lock();

				if ( !m_paused )
					m_current_time = get_time();

			
				return_value = static_cast<TimerValueType>(m_current_time - m_start_time);
				m_lock.unlock();


				return return_value;
			}

			// A function returning the difference since the last call in seconds.
			TimerValueType Timer::difference_in_nanoseconds()
			{
				TimerValueType return_value = 0;
				unsigned long long current_time = 0;


				m_lock.lock();

				if ( !m_paused )
					current_time = get_time();
				else
					current_time = m_current_time;

				return_value = static_cast<TimerValueType>(current_time - m_current_time);
				m_current_time = current_time;
				m_lock.unlock();


				return return_value;
			}

		#endif /* _WIN32 */

		// A function returning the current time of the timer.
		unsigned long long Timer::time()
		{
			unsigned long long return_value = 0;


			m_lock.lock();

			if ( !m_paused )
				m_current_time = get_time();

			return_value = m_current_time;
			m_lock.unlock();


			return return_value;
		}

		// A function returning the frequency of the timer.
		unsigned long long Timer::frequency()
		{
			#ifdef _WIN32

				unsigned long long return_value = 0;


				m_lock.lock();
				return_value = m_frequency;
				m_lock.unlock();


				return return_value;

			#else
				return 1;
			#endif /* _WIN32 */
		}

		// A function returning whether the timer is paused or not.
		bool Timer::is_paused()
		{
			bool return_value = false;


			m_lock.lock();
			return_value = m_paused;
			m_lock.unlock();


			return return_value;
		}

	} /* utility */

} /* athena */