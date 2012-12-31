#ifndef ATHENA_UTILITY_READERSWRITERLOCK_INL
#define ATHENA_UTILITY_READERSWRITERLOCK_INL

#ifndef ATHENA_UTILITY_READERSWRITERLOCK_HPP
	#error "readersWriterLock.hpp must be included before readersWriterLock.inl"
#endif /* ATHENA_UTILITY_READERSWRITERLOCK_HPP */



namespace athena
{

	namespace utility
	{

		/*
			A function returning a reference to the critical section variable.
			For use with functions that need access to the variable.
		*/
		#ifdef _WIN32
			inline ATHENA_DLL SRWLOCK& ReadersWriterLock::lock_ref()
		#else
			inline ATHENA_DLL pthread_rwlock_t& ReadersWriterLock::lock_ref()
		#endif /* _WIN32 */
		{
			return _lock;
		}


		// A function returning whether the critical section has been initialised.
		inline bool ReadersWriterLock::initialised() const
		{
			return _initialised;
		}

	} /* utility */

} /* athena */



#endif /* ATHENA_UTILITY_READERSWRITERLOCK_INL */