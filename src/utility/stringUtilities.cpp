#include "stringUtilities.hpp"
#include <cstdlib>
#include <cstring>
#include <cmath>


namespace athena
{

	namespace utility
	{

		// A functions that converts a string to it's wide character equivalent.
		std::wstring string_to_wide_string( const std::string& value )
		{
			std::wstring return_value(L"");
			wchar_t* resulting_string = new (std::nothrow) wchar_t[value.size()+1];
			

			if ( resulting_string != NULL )
			{
				#ifdef _WIN32
					size_t characters_converted = 0;
					size_t buffer_size = value.size();
				#endif /* _WIN32 */


				memset(resulting_string,L'\0',sizeof(wchar_t)*(value.size()+1));

				#ifdef _WIN32
					mbstowcs_s(&characters_converted,resulting_string,buffer_size+1,value.c_str(),buffer_size);
				#else
					mbstowcs(resulting_string,value.c_str(),value.size());
				#endif /* _WIN32 */

				return_value = resulting_string;
				delete resulting_string;
			}


			return return_value;
			//return std::wstring(value.begin(),value.end());
		}

		// A function that converts a wide string to it's ascii character equivalent.
		std::string wide_string_to_string( const std::wstring& value )
		{
			static const unsigned int difference = static_cast<unsigned int>(ceil(static_cast<float>(sizeof(wchar_t)) / static_cast<float>(sizeof(char))));

			std::string return_value("");
			char* resulting_string = new (std::nothrow) char[difference*(value.size()+1)];


			if ( resulting_string != NULL )
			{
				#ifdef _WIN32
					size_t characters_converted = 0;
					size_t buffer_size = difference*value.size();
				#endif /* _WIN32 */


				memset(resulting_string,'\0',sizeof(char)*(difference*(value.size()+1)));

				#ifdef _WIN32
					wcstombs_s(&characters_converted,resulting_string,buffer_size+difference,value.c_str(),buffer_size);
				#else
					wcstombs(resulting_string,value.c_str(),difference*value.size());
				#endif /* _WIN32 */

				return_value = resulting_string;
				delete resulting_string;
			}


			return return_value;
		}

	} /* utility */

} /* athena */