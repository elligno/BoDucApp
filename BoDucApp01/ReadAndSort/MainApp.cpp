
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
// stl include
#include <vector>
// boost include
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
#include "StructUtils.h"
#include "ParseCSV.hpp"
#include "BoDucApp.h"

//struct BoDucFields;
// namespace boduc { void findBoDucFields(const std::vector<std::string>& aCmdVec, const std::string& aField2Look, 
// 	BoDucFields& aBoducF);}

bool isColon(char c)
{
	return c == ':';
}

char* removeAccented(char* str) {
	char *p = str;
	while ((*p) != 0) {
		const char*
			//   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
			tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
		unsigned char ch = (*p);
		if (ch >= 192) {
			(*p) = tr[ch - 192];
		}
		++p; // http://stackoverflow.com/questions/14094621/
	}
	return str;
}

// remove all special character in a string
bool my_predicate(char c)
{
	return !(isalpha(c) || c == '_');
}

//s.erase(std::remove_if(s.begin(), s.end(), my_predicate), s.end());
// files to test
// std::vector<std::string> sfileNameAndPath{ R"(jani 3156 19-06-17.csv)", R"(jani 3157 19-06-17.csv)",
// R"(jani 3158 19-06-17.csv)", R"(jani 3159 19-06-17.csv)", R"(jani 3160 19-06-17.csv)",
// R"(jani 3161 19-06-17.csv)", R"(jani 3162 19-06-17.csv)",R"(jani 3161 19-06-17.csv)" };

// =================================================================
//
//                Windows Main Entry Point
//
// =================================================================

int main( int argc, char* argv[])
{
// 	std::vector<std::string> w_testRem = { "","toutou","jean","tia","","","pap",""};
// 	std::vector<int>::size_type w_siz = w_testRem.size();
// 	w_testRem.erase(std::remove(w_testRem.begin(), w_testRem.end(), ""), w_testRem.end());
// 	w_siz = w_testRem.size();
  
	//
	// We are reading the CSV file format (BoDuc billing order)
	// 
	std::string w_dataFilePath = R"(C:\JeanWorks\BoDuc\App\BoDucApp\ReadAndSort\DataToTest\)";  // string literal
	std::string w_fileName;
	std::cout << "Enter file name: \n";
	std::cin >> w_fileName;
	std::string w_fileAndPath = w_dataFilePath + w_fileName;
	std::cout << "We are processing the following file: " << w_fileAndPath.c_str() << "\n";

	// BoDuc application (default file)
	boduc::BoDucApp w_refactoringParser;
	//w_refactoringParser.parseCsvFiles(w_fileNameAndPath, w_bonLivraison);
	w_refactoringParser.parseCsvFiles(w_fileAndPath);

	std::cout << "we are just about to leave the main function\n";

 	return EXIT_SUCCESS;
}