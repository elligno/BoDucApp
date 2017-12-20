#include <vector>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>       // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
#include  <boost/algorithm/string/predicate.hpp>

#include "BoDucUtility.h"

using namespace bdGui;

std::vector<std::string> BoDucUtility::TrimRightCommaAndErase( const std::string & aStr2Trim)
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
