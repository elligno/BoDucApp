
// C++ includes 
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
// stl includes
#include <map>
#include <vector>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
#include  <boost/algorithm/string/predicate.hpp>
// app include
#include "VictoReader.h"

namespace boduc 
{
	VictoReader::VictoReader()
	{
		std::cout << "Nothing to do\n";
	}

	VictoReader::~VictoReader()
	{
		std::cout << "Nothing to do\n";
	}

	//
	// Services that are needed to fill the BoDuc field structure
	//
	void VictoReader::readNoCmd( const vecofstr& aCmdVec, BoDucFields& aBoducF)
	{
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

		short linenb(0);
		// need to check for "Date" first of (pair Date,No)
		for( const string& w_checkStr : aCmdVec)
		{
			// looking for "Date" then break
			if( contains(w_checkStr, "Date"))
			{
				linenb++; // line below which contains the command number
				break;
			}
			linenb++;
		}

		// string that should be pass to this method
		string w_fieldValue = aCmdVec[linenb];  					
		trim_right_if(w_fieldValue, is_any_of(","));
		// splitting string to retrieve COxxxxx cmd no
		vector<string> strs;
		split(strs, w_fieldValue, is_any_of(","));
		
		// not sure about this one, after this we should have a vector of 2 elements
		// because our search is based on a line that contains a pair (Date,No)
		strs.erase( remove_if(strs.begin(), strs.end(), // delete empty element
			[](const string& s)
		{ return s.empty(); }), strs.cend());

		// boost::starts_with algorithm
		if( starts_with(strs.back(),string("CO")))
		{
			aBoducF.m_noCmd = strs.back(); //assume that the last field is the right one
		}
	}
	
	void VictoReader::readShippedTo( const std::vector<std::string>& aFieldValue, BoDucFields& aBoducF)
	{
			using namespace std;
			using namespace boost;
			using namespace boost::algorithm;
	
// 			string w_shipto("Shipped To");
// 			auto w_string2Search = aCmdVec.cbegin();
// 			while (w_string2Search!= aCmdVec.cend())
// 			{
// 				string w_str2check = *w_string2Search;
// 				string::iterator w_found = search( w_str2check.begin(), w_str2check.end(),
// 					w_shipto.begin(), w_shipto.end());
// 
// 				if (w_found != w_str2check.cend())
// 				{
// 					// compute distance
// 					size_t w_dist = std::distance(aCmdVec.cbegin(), w_string2Search);
// 				//	vector<string>::iterator w_shipContent = w_string2Search;
// 					advance(w_string2Search,1);
// 					break;
// 				}
// 				++w_string2Search;
// 			}

// 			vector<string> w_addressPart;	
// 			do // address may take 2 cells, if more it's because of 
// 			{  // conversion mismatch and generally we use the last 2 lines
// 				trim_right_if(string(*w_string2Search), is_any_of(","));
// 			//	trim_left_if(w_addressValue, is_any_of(","));
// 				w_addressPart.push_back(*w_string2Search);
// 			} while( !contains(*w_string2Search++, "Contact"));

	    // rebuild address (assume that the last field is the right one)
//			aBoducF.m_deliverTo = rebuildShippedTo(w_addressPart); 

			std::string w_completeAddress;     // store part of the complete address
			if (aFieldValue.size() > 2)
			{
				// we need to take the last 2 in most cases
				using riter = std::reverse_iterator<std::vector<std::string>::const_iterator>;
				riter w_lastPart = aFieldValue.crbegin() + 1;
				w_completeAddress += *w_lastPart + std::string(" ");
				w_lastPart = aFieldValue.rbegin();
				w_completeAddress += *w_lastPart;
				aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
			}
			else if (aFieldValue.size() == 2)
			{
				// rebuild the whole address
				w_completeAddress += aFieldValue[0] + std::string(" ");
				w_completeAddress += aFieldValue[1];
				aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
			}
			else if (aFieldValue.size() == 1)
			{
				w_completeAddress += aFieldValue[0];
				aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one
			}
			else
			{
				std::cout << "There is a problem with Customer address\n";
			}
	}

	void VictoReader::readDeliveryDate( const std::string& aFieldValue, BoDucFields& aBoducF)
	{
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

		// splitting string to retrieve Cxxx cmd no
		vector<string> strs;
		boost::split(strs, aFieldValue, is_any_of(","));
		strs.erase( remove_if(strs.begin(), strs.end(), // delete empty element
			[](const string& s)
		{ return s.empty(); }));
		
		strs.erase(unique(strs.begin(), strs.end()), strs.end());
		vector<string> w_datepromise;
		//boost::split(w_datepromise, strs[1], boost::is_any_of(" ")); // need to be split
		boost::split(w_datepromise, strs[strs.size() - 3], boost::is_any_of(" ")); // need to be split
		aBoducF.m_datePromise = w_datepromise[0];
	}

	std::string VictoReader::rebuildShippedTo( const vecofstr& aAddressPart)
	{
		std::string w_rebuildStr;

		if( aAddressPart.size() > 2)
		{
			// we need to take the last 2 in most cases
			using criter = std::reverse_iterator<std::vector<std::string>::const_iterator>;
			criter w_lastPart = aAddressPart.crbegin() + 1;
			w_rebuildStr += *w_lastPart + std::string(" ");
			w_lastPart = aAddressPart.crbegin();
			w_rebuildStr += *w_lastPart;
		}
		else if( aAddressPart.size() == 2)
		{
			// rebuild the whole address
			w_rebuildStr += aAddressPart[0] + std::string(" ");
			w_rebuildStr += aAddressPart[1];
		}
		else if( aAddressPart.size() == 1)
		{
			w_rebuildStr += aAddressPart[0];
		}

		return w_rebuildStr;
	}

	size_t VictoReader::nbOfCmd(const std::string& aCmdFile) const
	{
 		size_t w_nbCmd = 0;
		std::ifstream w_readCSV(aCmdFile.c_str());
		if (w_readCSV) // check if its open, then read line-by-line 
		{
			for (std::string line; std::getline(w_readCSV, line);)
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

	void VictoReader::readProdDescr( const std::string& aFieldValue, BoDucFields& aBoducF)
	{
		using namespace std;
		using namespace boost;

		// Algorithm Steps 
		// Based on search of "Silo" which is the last element of the string 
		// 1. first position to the string that is under "Silo"
		// 2. look for first element that is empty (maybe use boost find algorithm)
		// which use the iterator range as return value (range between start of the 
		// string and the first non empty string), then split about ",", code product
		// the last element of vector
		// 3. From that position (first empty to the end of the string), look for first
		// non empty character, probably the beginning of the description. NOTE sometimes
		// description and quantity/silo are on the same line because there is no notes.
		// Split about "," but if there is no other stuff than description, it shouldn't
		// have any (vector == 0)
		// descr should be the first element

		// last part is the product description
		size_t w_charPos = aFieldValue.find_first_of(" ");
		string w_lastPart(aFieldValue.cbegin() + w_charPos, aFieldValue.cend());
		boost::trim_left(w_lastPart);
		vector<string> w_descrStr;
		split(w_descrStr, w_lastPart, boost::is_any_of(","));
		// check size
	//	std::vector<std::string>::size_type w_lastSize = w_descrStr.size();
		aBoducF.m_produit = w_descrStr[0];
	}

	void VictoReader::readQtySilo(const std::string & aFieldValue, BoDucFields & aBoducF)
	{
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

#if 0
		// Qty and silo
		const std::string& w_qtySilo = "abc"; //*begVec;
		// find position of "TM" in the whole string
		//		auto pos = w_qtySilo.find_last_of("TM");

		std::string w_tm("TM");
		const auto pos = std::search(w_qtySilo.cbegin(), w_qtySilo.cend(),
			w_tm.cbegin(), w_tm.cend());//w_qtySilo.find_last_of("TM");

		if (pos != w_qtySilo.cend())
		{
			using dist = std::iterator_traits<std::string::iterator>::difference_type;
			dist w_startStr = std::distance(w_qtySilo.cbegin(), pos);
			//		auto w_begFound = w_qtySilo.cbegin();
			//	  	std::advance(w_qtySilo.cbegin(),pos);
			std::string w_str2Trim(w_qtySilo.cbegin() + w_startStr, w_qtySilo.cend());

			// 					if (pos != std::string::npos)
			// 					{
			//	std::string w_str2Trim(w_qtySilo.cbegin() + pos, w_qtySilo.cend());
			// want to retrieve Qty and Silo
			std::vector<std::string> w_splitStr;
			boost::split(w_splitStr, w_str2Trim, boost::is_any_of(","));
			// remove all empty string, then we have as first element the "TM" string
			// just after we have the qty and silo 
			w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
				[](const std::string& s)
			{ return s.empty(); }), w_splitStr.cend());
			// sanity check
			assert(3 == w_splitStr.size());

			float w_qty = std::stof(w_splitStr[1]);
			aBoducF.m_qty = w_qty;

			// not as a number but a string
		//	int w_silo = std::stoi(w_splitStr[2]);
			aBoducF.m_silo = w_splitStr[2];
		}
#endif

		if( contains(aFieldValue, std::string("TM")))
		{
			std::vector<std::string> w_splitStr;
			boost::split(w_splitStr, aFieldValue, boost::is_any_of(","));

			// remove all empty string, then we have as first element the "TM" string
			// just after we have the qty and silo 
			w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
				[](const std::string& s)
			{ return s.empty(); }), w_splitStr.cend());

			auto w_look4TM = std::find(w_splitStr.cbegin(), w_splitStr.cend(), std::string("TM"));
			++w_look4TM;
			aBoducF.m_qty = std::stof(*w_look4TM++);

			// NOTE: some of the silo number has the following format "03"
			// want to remove the first character since it is not relevant
			if ((*w_look4TM).find("0") != std::string::npos)
			{
				aBoducF.m_silo = (*w_look4TM)[1];
			}
			else
			{
				aBoducF.m_silo = *w_look4TM;
			}
			//break; no point to go further
		}
	}

	void VictoReader::readProdCode(const std::string & aFieldValue, BoDucFields& aBoducF)
	{
		using namespace std;
		using namespace boost;

		size_t w_charPos = aFieldValue.find_first_of(" ");
		string w_subStr(aFieldValue.cbegin(), aFieldValue.cbegin() + w_charPos);
		vector<std::string> w_codeSplit;
		split(w_codeSplit, w_subStr, is_any_of(","));

		// search for first digit
		std::string w_prodCode;
		if( !std::all_of(w_codeSplit[1].begin(), w_codeSplit[1].end(), ::isdigit)) //!is_digits(w_codeSplit[1])
		{
			// we have to remove non-digit character from string 
			string::iterator w_foundDigit = find_if(w_codeSplit[1].begin(), w_codeSplit[1].end(),
				[](char c)
			{
				locale loc;
				return isdigit(static_cast<unsigned char>(c), loc);
			});

			size_t w_next = distance(w_codeSplit[1].begin(), w_foundDigit);
			// product code
			string::iterator w_chck = w_codeSplit[1].begin();
			advance(w_chck, w_next);
			string w_codeProd(w_chck, w_codeSplit[1].end());
			aBoducF.m_prodCode = atoi(w_codeProd.c_str());
		}
		else
		{
			w_prodCode = w_codeSplit[1];
			aBoducF.m_prodCode = atoi(w_prodCode.c_str());
		}
	}
} // End of namespace

