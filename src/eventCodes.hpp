#ifndef ATHENA_EVENTCODES_HPP
#define ATHENA_EVENTCODES_HPP

#include "core/event.hpp"



namespace athena
{
	
	/*
		The IDs of the managers that are triggering the following events.
	*/
	enum ManagerIDs
	{
		ThreadPool = 0 ,
		EventManagerID ,
		RenderManagerID ,
		InputManagerID ,
		LogManagerID ,
		FirstAvailableID
	};


	const core::EventCode EVENT_ALL = 0x0000;
	const core::EventCode EVENT_EXIT = 0x0001;
	const core::EventCode EVENT_UPDATE = 0x0002;
	const core::EventCode EVENT_UPDATE_SLOW = 0x0003;
	const core::EventCode EVENT_UPDATE_MEDIUM = 0x0004;
	const core::EventCode EVENT_UPDATE_FAST = 0x0005;

} /* athena */

#endif /* ATHENA_EVENTCODES_HPP */
