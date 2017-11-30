
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>

namespace boduc
{
	// BoDuc file parser 
	int SimpleParseCSV( const std::string& aCsvFile)
	{
		using namespace std;
		using namespace boost;

		const std::string w_fileName = R"(jani 3156 19-06-17.csv)";
		//string data("data.csv");

		ifstream w_inFile(aCsvFile.c_str());
		if( !w_inFile.is_open()) return 1;

		// escape character used by csv file
		//  used to read multiple values separated by commas
		using Tokenizer = tokenizer< escaped_list_separator<char>>;

		vector< string > w_vec;
		string w_line;

		while( getline( w_inFile, w_line))
		{
			Tokenizer tok(w_line);
			w_vec.assign(tok.begin(), tok.end());

			if( w_vec.size() < 3) continue;

			copy( w_vec.begin(), w_vec.end(),
				ostream_iterator<string>(cout, "|"));

			cout << "\n----------------------" << endl;
		}

		return 1;
	}
	//}

	void testIngBoDucApp()
	{}
} // End of namespace