#pragma once

#include <string>
#include <vector>

namespace bdGui 
{
	class BoDucUtility 
	{
	public:
		static std::vector<std::string> TrimRightCommaAndErase( const std::string& aStr2Trim);
		// Used when we have a line in the address that
		// we want to check if it ends with postal code
		static bool isPostalCode(const std::string& aAddress);
	};
} // End of namespace
