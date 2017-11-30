// C++ include
#include <iostream>
// STL includes
#include <functional>
#include <algorithm>
// boost include
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// app includes
//#include "BoDucApp.h"
#include "../BoDucParsingAlgo.h"

namespace boduc 
{
	void testTrim()
	{
		using namespace std;
		using namespace std::placeholders;
		using namespace boost::algorithm;

		const std::string w_workPath = R"(C:\JeanWorks\BoDuc\App\BoDucApp\ReadAndSort\DataToTest\)";
		const std::string w_workFile = "victocm1.csv";
		const std::string w_fileandpath = w_workPath + w_workFile;
		std::cout << "Test working file is: " << "\n";

		// read file 
		BoDucParsingAlgo w_testAlgo;
		size_t w_checknbCmd = w_testAlgo.nbOfCmd(w_fileandpath);
		// , std::string("Signature")
		w_testAlgo.readFile(w_fileandpath, std::string("Signature")); //can set the splitting token default "Ordered on"
		BoDucParsingAlgo::mapIntVecstr w_mapofCmd = w_testAlgo.readedMap();
		// sanity check
		if( w_mapofCmd.empty())
		{
			std::cout << "We have an empty cmd line map\n";
		}

		// 
		BoDucParsingAlgo::mapIntVecstr::iterator begMap = w_mapofCmd.begin();
		vector<string>& w_checkTrim = begMap->second;
// 		for_each( w_checkTrim.begin(), w_checkTrim.end(),
// 			std::bind(&trim_right_if,_1,is_any_of(",")));
		
		for_each(w_checkTrim.begin(), w_checkTrim.end(), 
			[](string& aStr2Trm)
		{
			trim_right_if(aStr2Trm, is_any_of(","));
		});

		// actually we don't need to trim from left, it is obsolete
		for_each(w_checkTrim.begin(), w_checkTrim.end(),
			[](string& aStr2Trm)
		{
			trim_left_if(aStr2Trm, is_any_of(","));
		});

		// go through each cmd vector and trim line
		// use the bind mechanism
		// std::for_each(begin90,end(),std::bind(&trim_right,_1, ","))
	}
} // End of namespace