#ifndef ATHENA_HPP
	#define ATHENA_HPP



	// Windows definitions.
	#ifdef _WIN32
	
		// Enabling a flag that disables some header files from being included.
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN
		#endif /* WIN32_LEAN_AND_MEAN */

		// Disabling the min and max macros.
		#ifndef	NOMINMAX
			#define NOMINMAX
		#endif /* NOMINMAX */

		// The function calling convention that the library is using.
		#define ATHENA_PRECALL  __stdcall
		#define ATHENA_POSTCALL 

		// Macro that is used when the library is exported or imported.
		#ifdef ATHENA_EXPORTS
			#define ATHENA_DLL __declspec(dllexport)
		#else
			#define ATHENA_DLL __declspec(dllimport)
		#endif /* DLL_EXPORT */

	#else	// Unix definitions.
		
		// The function calling convention that the library is using.
		#define ATHENA_PRECALL  
		#define ATHENA_POSTCALL __attribute__(stdcall)
		
		#define	ATHENA_DLL

	#endif /* _WIN32 */



#endif /* ATHENA_HPP */