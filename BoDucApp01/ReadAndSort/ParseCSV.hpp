
#pragma once

// C++ includes
#include <iostream>
#include <string>
#include <fstream>
// boost includes
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

// Remark: need to test this algorithm with BoDuc file (cmd)
// and see what it output
namespace boduc 
{
	// Usage is simple :
	//
 	//  vector<vector<int>> result = ParseCsv<int>(ifstream(“test.csv”));

	// i am not sure what we do here, reading a csv file an return what?
	//
	template<class T>
	std::vector<std::vector<T>> ParseCsv(std::istream& is)
	{
		using namespace std;
		using namespace boost;

     // define some alias
		using iterator = istream_iterator<char>;
		using separator = char_separator<char>;
		using Tokenizer = tokenizer<separator, iterator, string>;

		is.unsetf(ios_base::skipws); // probably skip white space

		vector<vector<T>> result;
		Tokenizer tokens(iterator(is), iterator(), separator(", ", "\n"));
		bool newLine = true;
		for( auto token = tokens.begin(); token != tokens.end(); ++token)
		{
			// what we do here?
			if( newLine)
			{
				result.push_back(vector<T>());
				newLine = false;
			}
			// also here, not following!!
			if( *token == "\n")
				newLine = true;
			else
				result.back().push_back(lexical_cast<T>(trim_copy(*token)));
		};

		return result;
	}
} // End of namespace