#ifndef ATHENA_WINDOWS_DEFINITIONS_HPP
#define ATHENA_WINDOWS_DEFINITIONS_HPP



#ifdef _WIN32

	// Enabling a flag that disables some header files from being included.
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif /* WIN32_LEAN_AND_MEAN */

	// Disabling the min and max macros.
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif /* NOMINMAX */

	// A definition in order to detect memory leaks.
	#ifdef _DEBUG
		
		#ifndef _CRTDBG_MAP_ALLOC
			#define _CRTDBG_MAP_ALLOC
		#endif /* _CRTDBG_MAP_ALLOC */

		#include <stdlib.h>
		#include <crtdbg.h>

	#endif /* _DEBUG */

#endif /* _WIN32 */



#endif /* ATHENA_WINDOWS_DEFINITIONS_HPP */