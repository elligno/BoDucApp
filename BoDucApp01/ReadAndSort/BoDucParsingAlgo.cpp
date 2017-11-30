// C++ include
#include <iostream>
#include <fstream>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// app include
#include "BoDucParsingAlgo.h"
#include "VictoBonLivraison.h"

namespace boduc
{
	void fillBoDucFieldsFromFiles(const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF);
	void fillBoDucFieldsFromFile(const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF);
}

using namespace boduc;

void BoDucParsingAlgo::parseCsv()
{
	BoDucParsingAlgo::mapIntVecstr w_mapofCmd = readedMap();
	if (w_mapofCmd.empty())
	{
		// need to do something, for now return (log something)
		return;
	}

	auto w_begMap = w_mapofCmd.cbegin();
	while( w_begMap != w_mapofCmd.cend()) // process each cmd parsed from csv file
	{
		vecofstr w_cmd2Proceed = w_begMap->second;
		if( !useTM(w_cmd2Proceed))
		{
			++w_begMap; // next in the list
			continue; //"TM" not used in this command
		}

		// filling the BoDuc struct	
		BoDucFields w_boducStruct;
		fillBoDucFieldsFromFile( w_cmd2Proceed, std::string("No Command"), std::ref(w_boducStruct));
		fillBoDucFieldsFromFile( w_cmd2Proceed, std::string("Shipped to"), std::ref(w_boducStruct));
		fillBoDucFieldsFromFile( w_cmd2Proceed, std::string("Date promise"), std::ref(w_boducStruct));
		fillBoDucFieldsFromFile( w_cmd2Proceed, std::string("produit et description"), std::ref(w_boducStruct));
//		fillBoDucFieldsFromFile( w_cmd2Proceed, std::string("TM"), std::ref(w_boducStruct));

		m_vecOfStruct.push_back(w_boducStruct);

		++w_begMap; // next in the list

#if _DEBUG
		std::cout << "\n";
		std::cout << "BoDuc command No is: " << w_boducStruct.m_noCmd << "\n";
		std::cout << "Command shipped to: "  << w_boducStruct.m_deliverTo << "\n";
		std::cout << "Deliver date is: "     << w_boducStruct.m_datePromise << "\n";
		std::cout << "Product ordered is: "  << w_boducStruct.m_produit << "\n";
		std::cout << "Quantity ordered is: " << w_boducStruct.m_qty << "\n";
		std::cout << "Silo number is: "      << w_boducStruct.m_silo << "\n";
#endif
	} //while
}

void BoDucParsingAlgo::parseCsv( boduc::BoDucBonLivraisonAlgorithm* aBonLivraisonAlgo)
{
	BoDucParsingAlgo::mapIntVecstr w_mapofCmd = readedMap();
	if (w_mapofCmd.empty())
	{
		// need to do something, for now return (log something)
		return;
	}

	auto w_begMap = w_mapofCmd.cbegin();
	while (w_begMap != w_mapofCmd.cend()) // process each cmd parsed from csv file
	{
		vecofstr w_cmd2Proceed = w_begMap->second;
		if (!useTM(w_cmd2Proceed))
		{
			++w_begMap; // next in the list
			continue; //"TM" not used in this command
		}

		// filling the BoDuc struct	
		BoDucFields w_boducStruct;
		if( aBonLivraisonAlgo != nullptr)
		{
			aBonLivraisonAlgo->fillBoDucFields(w_cmd2Proceed, w_boducStruct);
		}
		m_vecOfStruct.push_back(w_boducStruct);
		++w_begMap; // next in the list

#if _DEBUG
		std::cout << "\n";
		std::cout << "BoDuc command No is: " << w_boducStruct.m_noCmd << "\n";
		std::cout << "Command shipped to: " << w_boducStruct.m_deliverTo << "\n";
		std::cout << "Deliver date is: " << w_boducStruct.m_datePromise << "\n";
		std::cout << "Product ordered is: " << w_boducStruct.m_produit << "\n";
		std::cout << "Quantity ordered is: " << w_boducStruct.m_qty << "\n";
		std::cout << "Silo number is: " << w_boducStruct.m_silo << "\n";
#endif

	}//while-loop
}

size_t boduc::BoDucParsingAlgo::nbOfCmd( const std::string& aCmdFile) const
{
	size_t w_nbCmd=0;
	std::ifstream w_readCSV(aCmdFile.c_str());
	if( w_readCSV) // check if its open, then read line-by-line 
	{
		for( std::string line; std::getline(w_readCSV, line);)
		{
			// check if contains "Signature"
			if( boost::algorithm::contains(line, std::string("Signature")))
			{
			  ++w_nbCmd;
			}
		}//for-loop
	}

	return w_nbCmd;
}

// Use a do-while loop to find the string "produit et description"
// once you find it go to the next line to look for the string "TM"
// Basically this function iterate through the cmd and look for a given string
// check if TM is present
//
bool boduc::BoDucParsingAlgo::useTM( const std::vector<std::string>& aVecOfCmdLines)
{
	bool retTM = false;
	auto lineIter = aVecOfCmdLines.cbegin();
	do 
	{
		lineIter++;
	} while( !boost::algorithm::contains(*lineIter, std::string("produit et description")));

	using dist = std::iterator_traits<std::string::iterator>::difference_type;
	dist w_startStr = std::distance(aVecOfCmdLines.cbegin(), lineIter);
	auto begVec = aVecOfCmdLines.cbegin() + w_startStr;
	while( begVec != aVecOfCmdLines.cend())
	{
		if( boost::algorithm::contains(*begVec, std::string("TM")))
		{
			retTM = true;
			break;
		}
		++begVec;
	}

	// next line contains the string "TM" that we are looking for
//	advance(lineIter, 2);
//	std::string w_line2Look = *lineIter;
	// find position of "TM" in the whole string usually at the end of the string
	// in the following format TM,1.0,,2
//	std::string w_tm("TM");
//	auto pos = std::search( w_line2Look.cbegin(), w_line2Look.cend(),
//		                      w_tm.cbegin(), w_tm.cend());

// 	if( pos != w_line2Look.cend()) // found something
// 	{
// 		retTM = true;
// 	}

#if 0
	while( begVec != aVecOfCmdLines.cend())
	{
		if( boost::algorithm::contains(*begVec, std::string("TM")))
		{
			std::string w_qtySilo = *begVec;
			// find position of "TM" in the whole string usually at the end of the string
			// in the following format TM,1.0,,2
			std::string w_tm("TM");
			auto pos = std::search(w_qtySilo.cbegin(), w_qtySilo.cend(),
				w_tm.cbegin(),w_tm.cend());//w_qtySilo.find_last_of("TM");
		
			if( pos!=w_qtySilo.cend()/*!= std::string::npos*/)
			{
				using dist = std::iterator_traits<std::string::iterator>::difference_type;
				dist w_startStr = std::distance(w_qtySilo.cbegin(),pos);
		//		auto w_begFound = w_qtySilo.cbegin();
	//	  	std::advance(w_qtySilo.cbegin(),pos);
				std::string w_str2Trim(w_qtySilo.begin()+ w_startStr, w_qtySilo.end());

				// want to retrieve Qty and Silo
				std::vector<std::string> w_splitStr;
				boost::split(w_splitStr, w_str2Trim, boost::is_any_of(","));
				// remove all empty string, then we have as first element the "TM" string
				// just after we have the qty and silo 
				w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
					[](const std::string& s)
				{ return s.empty(); }), w_splitStr.cend());
				assert(3 == w_splitStr.size()); // {"TM","2.5","5"} as vector element
				// check if the next string is a number (quantity), do that because 
				// we could have found a TM but necessarily the one we are looking for 
				try
				{
					double w_qty = boost::lexical_cast<double>(w_splitStr[1]); // double could be anything with >> operator.
					retTM = true; // use TM
				}
				catch (...) //catch almost anything
				{
					std::cout << "oops, not a number\n";
					retTM = false;
				}
			}// if
		} //if-contains
		++begVec;
	}//while-loop
#endif

	return retTM;
}

void BoDucParsingAlgo::readFile( const std::string& aFileAnPath, const std::string& aSplitBill)
{
	using namespace std;
	using namespace boost;
	using namespace boost::algorithm;

	// create alias
	using vecofstr = std::vector<std::string>;

	// declare vector to store our string from cvs document 
	vecofstr w_vecStr;
	w_vecStr.reserve(50); // reserve enough space for storing each line ??

  // Command in one file, reading the command by splitting with the "Ordered on"
	short i(0);
	ifstream w_readVSV( aFileAnPath.c_str());
	if( w_readVSV) // check if its open, then read line-by-line 
	{
		for( string line; std::getline(w_readVSV, line);)
		{
			if( i == 0) // first line is obsolete
			{
				// check if contains "Ordered on" as first line
				// NOTE "request-2.csv" we split the bill with this token, 
				// and format was that first line contains this string
				if( contains(line, "Ordered on"))
				{
					++i;  // next line
					continue; // i don't see why we should increment it
				}
			}//if(i==0)
			w_vecStr.push_back(line);

			// NOTE we assume that we are at the last line of the command
			// then we check if the carrier string name is part of the whole 
			// command (if so, add it to the map otherwise skip it)
			// IMPORTANT this algorithm assume that we are at the end or the 
			// last line (split into separate command is based on this assumption)
			// if not the case then it won't work!!
			if( contains( line, aSplitBill))
			{
				// check for carrier name some are different and don't need to be treated
				// Francois mentioned that field can be blank, need to be careful
				// Also, we must check also for the "TM" tonne metrique, if not present
				// the cmd is discarded
				if( any_of( w_vecStr.cbegin(), w_vecStr.cend(),
					[]( const std::string& aStr2Look) -> bool     // lambda (anonymous) function
				{
					return (contains( aStr2Look, "NIR R-117971-3 TRSP CPB INC")
						   || contains( aStr2Look, "NIR R-117971-3 TRANSPORT CPB")); // sometime we have empty (blank field) string
				}
				)//any_of
					)//if
				{
					m_mapintVec.insert( make_pair(i++, w_vecStr));
					w_vecStr.clear();
				}
				else
				{
					/*w_mapofCmd.insert(std::make_pair(i++,w_vecStr));*/
					w_vecStr.clear();
				}
			}
		}//for-loop
	}//if
}

void BoDucParsingAlgo::createReport( const std::string& aBonDeLivraison/*="BonDeLivraison.txt"*/)
{
	std::ofstream w_bonLivraison(aBonDeLivraison/*aOutputFile*/, std::ios::out);
	assert(w_bonLivraison.is_open());

	// write title or header
	w_bonLivraison << "No Command " << "\t" << "Shipped To " << "\t" << "Deliver Date " << "\t" <<
		"Product " << "\t" << "Quantity " << "\t" << "Silo" << "\n";
	
	for( const BoDucFields& w_bfield : m_vecOfStruct)
	{
	  w_bonLivraison << w_bfield.m_noCmd << "\t" << w_bfield.m_deliverTo << "\t" << w_bfield.m_datePromise
		<< "\t" << w_bfield.m_produit << "\t" << w_bfield.m_qty << "\t" << w_bfield.m_silo << "\n";
	}
}