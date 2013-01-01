#include "timer.hpp"
#include <ctime>


namespace athena
{

	namespace utility
	{

		// A function responsible of getting the current time.
		unsigned long long Timer::_get_time()
		{
			unsigned long long return_value = 0;


			#ifdef _WIN32

				LARGE_INTEGER temp = { 0 };


				QueryPerformanceCounter(&temp);
				return_value = temp.QuadPart;
				
			#else

				timespec temp = { 0 , 0 };


				clock_gettime(CLOCK_REALTIME,&temp);
				return_value = temp.tv_sec;

			#endif /* _WIN32 */


			return return_value;
		}

		// A function responsible of getting the frequency.
		unsigned long long Timer::_get_frequency()
		{
			unsigned long long return_value = 1;


			#ifdef _WIN32

				LARGE_INTEGER temp = { 1 };

				
				QueryPerformanceFrequency(&temp);

				if ( temp.QuadPart != 0 )
					return_value = temp.QuadPart;
				
			#else

				timespec temp = { 0 , 0 };


				clock_getres(CLOCK_REALTIME,&temp);
				return_value = temp.tv_sec;

			#endif /* _WIN32 */


			return return_value;
		}


		// The constructor of the class.
		Timer::Timer() : 
			_lock() , 
			_reverse_frequency(1) , 
			_start_time(0) , 
			_current_time(0) , 
			_frequency(1) , 
			_paused(false)
		{
		}

		// The destructor of the class.
		Timer::~Timer()
		{
		}


		// A function responsible of starting the timer.
		void Timer::start()
		{
			_lock.lock();
			_current_time = _get_time();
			_start_time = _current_time;
			_frequency = _get_frequency();
			_reverse_frequency = static_cast<TimerValueType>(1)/static_cast<TimerValueType>(_frequency);
			_paused = false;
			_lock.unlock();
		}

		// A function responsible of pausing the timer.
		void Timer::pause()
		{
			_lock.lock();
			_paused = true;
			_lock.unlock();
		}

		// A function responsible of resuming the timer if it is paused.
		void Timer::resume()
		{
			_lock.lock();
			_paused = false;
			_lock.unlock();
		}


		// A function returning the current time in seconds.
		TimerValueType Timer::seconds()
		{
			TimerValueType return_value = 0;


			_lock.lock();

			if ( !_paused )
				_current_time = _get_time();

			return_value = static_cast<TimerValueType>(_current_time - _start_time)*_reverse_frequency;
			_lock.unlock();


			return return_value;
		}

		// A function returning the difference since the last call in seconds.
		TimerValueType Timer::difference_in_seconds()
		{
			TimerValueType return_value = 0;
			unsigned long long current_time = 0;


			_lock.lock();

			if ( !_paused )
				current_time = _get_time();
			else
				current_time = _current_time;

			return_value = static_cast<TimerValueType>(current_time - _current_time)*_reverse_frequency;
			_current_time = current_time;
			_lock.unlock();


			return return_value;
		}

		// A function returning the current time of the timer.
		unsigned long long Timer::time()
		{
			unsigned long long return_value = 0;


			_lock.lock();

			if ( !_paused )
				_current_time = _get_time();

			return_value = _current_time;
			_lock.unlock();


			return return_value;
		}

		// A function returning the frequency of the timer.
		unsigned long long Timer::frequency()
		{
			unsigned long long return_value = 0;


			_lock.lock();
			return_value = _frequency;
			_lock.unlock();


			return return_value;
		}

		// A function returning whether the timer is paused or not.
		bool Timer::is_paused()
		{
			bool return_value = false;


			_lock.lock();
			return_value = _paused;
			_lock.unlock();


			return return_value;
		}

	} /* utility */

} /* athena */