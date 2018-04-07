#pragma once

#include <string>
#include <vector>

namespace bdGui 
{
	class BoDucUtility 
	{
	public:
		static std::vector<std::string> TrimRightSplitAndErase( const std::string& aStr2Trim);
		// Used when we have a line in the address that
		// we want to check if it ends with postal code
		static bool isPostalCode(const std::string& aAddress);
		static std::wstring ConvertFromUtf8ToUtf16(const std::string& str);
		static std::wstring FromUtf8ToUtf16( const std::string& str);
	};
} // End of namespace
