#ifndef ATHENA_CORE_ENTITY_INL
	#define ATHENA_CORE_ENTITY_INL



	namespace athena
	{

		namespace core
		{

			// A function responsible of setting the id of the entity.
			inline void Entity::id( const unsigned int value )
			{
				_id = value;
			};


			// A function returning the id of the entity.
			inline unsigned int Entity::id() const
			{
				return _id;
			};

		} /* core */

	} /* athena */



#endif /* ATHENA_CORE_ENTITY_INL */