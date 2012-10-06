#include "eventManager.hpp"



namespace athena
{
	
	namespace core
	{

		EventManager* EventManager::_instance = NULL;
		utility::ReadersWriterLock EventManager::_instance_lock;


		EventManager::EventManager()	{};
		EventManager::~EventManager()	{};

	} /* core */

} /* athena */