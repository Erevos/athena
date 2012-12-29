#ifndef ATHENA_CORE_LISTENER_INL
#define ATHENA_CORE_LISTENER_INL

#ifndef ATHENA_CORE_LISTENER_HPP
	#error "listener.hpp must be included before listener.inl"
#endif /* ATHENA_CORE_LISTENER_HPP */



namespace athena
{

	namespace core
	{

		// A function responsible of setting the id of the listener.
		inline void Listener::id( const unsigned int value )
		{
			_id = value;
		};


		// A function returning the id of the listener.
		inline unsigned int Listener::id() const
		{
			return _id;
		};

	} /* core */

} /* athena */



#endif /* ATHENA_CORE_LISTENER_INL */