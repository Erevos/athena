#include "athenaDefinitions.hpp"
#include <Windows.h>



BOOLEAN WINAPI DllMain( HINSTANCE hDllHandle, DWORD nReason, LPVOID )
{
	BOOLEAN bSuccess = TRUE;


	//  Perform global initialization.
	switch ( nReason )
	{
		case DLL_PROCESS_ATTACH:

			//  For optimization.
			DisableThreadLibraryCalls( hDllHandle );
			break;
	}


	return bSuccess;
 }