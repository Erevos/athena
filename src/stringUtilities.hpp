#ifndef ATHENA_UTILITY_STRINGUTILITIES_HPP
#define ATHENA_UTILITY_STRINGUTILITIES_HPP

#include <string>


namespace athena
{

	namespace utility
	{

		// A functions that converts a string to it's wide character equivalent.
		std::wstring string_to_wide_string( const std::string& value );

		// A function that converts a wide string to it's ascii character equivalent.
		std::string wide_string_to_string( const std::wstring& value );

	} /* utility */

} /* athena */

#endif /* ATHENA_UTILITY_STRINGUTILITIES_HPP */