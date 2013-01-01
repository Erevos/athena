#include "athenaDefinitions.hpp"

#ifdef _WIN32

#include <Windows.h>

/*
	The function responsible of handling initialisation and
	deinitialisation of the DLL library.
*/

BOOLEAN WINAPI DllMain( HINSTANCE handle , DWORD reason , LPVOID )
{
	BOOLEAN return_value = TRUE;


	// Perform global initialization.
	switch ( reason )
	{
		// When the library is being attached to the program.
		case DLL_PROCESS_ATTACH:

			// For optimization.
			DisableThreadLibraryCalls(handle);
			break;
	}


	return return_value;
 }

#endif /* _WIN32 */
