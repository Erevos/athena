#ifndef ATHENA_DEFINITIONS_HPP
#define ATHENA_DEFINITIONS_HPP



// Windows definitions.
#ifdef _WIN32

	// Macro that is used when the library is exported or imported.
	#ifdef ATHENA_EXPORTS
		#define ATHENA_DLL __declspec(dllexport)
	#else
		#define ATHENA_DLL __declspec(dllimport)
	#endif /* DLL_EXPORT */

#else	// Unix definitions.

	#define ATHENA_DLL

#endif /* _WIN32 */



#endif /* ATHENA_DEFINITIONS_HPP */