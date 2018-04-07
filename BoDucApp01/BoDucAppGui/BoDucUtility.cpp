#include <Windows.h>
#include <codecvt>
#include <vector>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>       // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
#include  <boost/algorithm/string/predicate.hpp>

#include "BoDucUtility.h"

using namespace bdGui;

std::vector<std::string> BoDucUtility::TrimRightSplitAndErase( const std::string & aStr2Trim)
{
	using namespace std;
	using namespace boost;

	std::string w_str2Trim(aStr2Trim.cbegin(), aStr2Trim.cend());
	trim_right_if(w_str2Trim, is_any_of(","));
	
	// splitting string to retrieve COxxxxx cmd no
	vector<string> strs;
	split(strs, w_str2Trim, is_any_of(","));

	// not sure about this one, after this we should have a vector of 2 elements
	// because our search is based on a line that contains a pair (Date,No)
	strs.erase( remove_if(strs.begin(), strs.end(), // delete empty element
		[](const string& s)
	{ return s.empty(); }), strs.cend());

	return strs;
}

bool BoDucUtility::isPostalCode(const std::string & aAddress)
{
		// split with token = ,
		// take last element of split vector and count 
		// if alpha and if digit by using the count count_if
		// stl algorithm. If both count == 3 we have a postal code

		std::locale loc;
		auto w_nbAlpha = std::count_if(aAddress.cbegin(), aAddress.cend(),
			[loc](unsigned char c) { return std::isalpha(c, loc); }
		);

		auto w_nbDigit = std::count_if(aAddress.cbegin(), aAddress.cend(),
			[loc](unsigned char c) { return std::isdigit(c, loc); }
		);
		// postal code has same number of digit and character
		if (w_nbAlpha == w_nbDigit)
		{
			return true;
		}
		else
		{
			size_t w_pos = aAddress.find_first_not_of(' ');
			// check last 3 characters (Francois Ethier corrupted postal code)
			auto begCode = aAddress.cbegin();
			if (w_pos == 1)
			{
				std::advance(begCode, 4);// because blank character at the beginning
			}
			else
			{
				std::advance(begCode, 3);// because blank character at the beginning
			}
			w_nbDigit = std::count_if(begCode, aAddress.cend(),
				[loc](unsigned char c) { return std::isdigit(c, loc); }
			);
			if (w_nbDigit == 2)
			{
				return true;
			}
		}
		return false;
}

std::wstring BoDucUtility::ConvertFromUtf8ToUtf16( const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if( requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign( buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
}

// same as above but using C++11/14 features
std::wstring BoDucUtility::FromUtf8ToUtf16(const std::string & str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cwuf8;
	std::wstring w_strConverted = cwuf8.from_bytes(str);
	return w_strConverted;
}