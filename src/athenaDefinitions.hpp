#ifndef ATHENA_HPP
	#define ATHENA_HPP



	#define ATHENA_CALL  __stdcall


	#ifdef _WIN32
	
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN
		#endif /* WIN32_LEAN_AND_MEAN */

		#ifndef	NOMINMAX
			#define NOMINMAX
		#endif /* NOMINMAX */

		#ifdef ATHENA_EXPORTS
			#define ATHENA_DLL _declspec(dllexport)
		#else
			#define ATHENA_DLL _declspec(dllimport)
		#endif /* DLL_EXPORT */

	#else
		#define	ATHENA_DLL
	#endif /* _WIN32 */



#endif /* ATHENA_HPP */