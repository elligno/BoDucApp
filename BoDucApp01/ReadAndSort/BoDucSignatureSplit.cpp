// C++ include
#include <iostream>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// App include
#include "BoDucSignatureSplit.h"

namespace boduc 
{
	BoDucSignatureSplit::BoDucSignatureSplit( std::initializer_list<std::string>& aListOfKeywords)
	: m_vecOfKeywords(aListOfKeywords)
	{
		std::cout << "Algorithm keyword are the following: " << "\n";
	}

	void BoDucSignatureSplit::fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField)
	{
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

		// temporary
		std::vector<std::string> w_vecOfKeywords = { string("Shipped to"), string("Date promise"),
			string("produit et description"), string("TM") };

		// could not compile when insert method is call, we have a const vector and 
		// a member variable which is not const, not sure if it is a problem
		// Using the move semantic, not sure i really understand what's going on here
		m_vecoflines = std::move(aCmdVec); // .insert(aCmdVec.begin(), aCmdVec.end());
		
		// check for "TM" is present, otherwise we don't consider it

		// retrieve the command number
		readNoCmd(aBoDucField);

//		short vecPos(0);
		// line iterator
		auto begVec = m_vecoflines.cbegin();
		// parse according to the list of keyword
		for( const string& w_field2Look : w_vecOfKeywords)
		{
			while( begVec != m_vecoflines.cend()) // go through all element cmd line
			{
				const std::string& str2look = *begVec;
				if( contains(str2look, w_field2Look.c_str()))
				{
					// retrieve the cmd no. which is next line
					std::string w_fieldValue = m_vecoflines[++m_vecPos];  //next line (below current which contains field to look)
					
				  // calling the method to read field
					if( ::strcmp("Shipped to", w_field2Look.c_str()) == 0)
					{
						readShippedTo(aBoDucField);
						break;
					}
					if( ::strcmp("Date promise", w_field2Look.c_str()) == 0)
					{
						readDatePromise(aBoDucField);
						break;
					}
					if (::strcmp("produit et description", w_field2Look.c_str()) == 0)
					{
						readProductDescr(aBoDucField);
						break;
					}
					if( ::strcmp("TM", w_field2Look.c_str()) == 0)
					{
						readQtyAndSiloNo(aBoDucField);
						break;
					}
				}
				++begVec;// next line in the cmd
				++m_vecPos; // holding position
			} //while-loop
		}//for-loop
	}

	void BoDucSignatureSplit::readNoCmd( BoDucFields& aBoDucField)
	{
		// reading the command number which has the following format C12324345 (first character not a digit)
		int linenb(0);
		using namespace std;
		using namespace boost::algorithm;

		// workaround to take account the CSV formatting when cmd are tied together  
		std::string w_fieldValue = m_vecoflines[0];        // based on "Ordered on"
		trim_right_if( w_fieldValue, is_any_of(","));
		trim_left_if( w_fieldValue, is_any_of(","));

		// splitting string to retrieve Cxxx cmd no
		vector<string> strs;
		boost::split( strs, w_fieldValue, boost::is_any_of(","));
		// last element is the cmd no (assume this for now)

		//aBoducF.m_noCmd = strs.back(); assume that the last field is the right one 
		// check for the first character 'C' for command number
		if( strs.back()[0] == 'C')
		{
			aBoDucField.m_noCmd = strs.back(); // assume that the last field is the right one 
		}
		else
		{
			// need to check for "Date" first of
			for (const std::string& w_checkStr : m_vecoflines)
			{
				// looking for "Date" then break
				if (contains(w_checkStr, "Date"))
				{
					linenb++; // line below which contains the command number
					break;
				}
				linenb++;
			}
			w_fieldValue = m_vecoflines[linenb];        // based on "Ordered on"
			trim_right_if(w_fieldValue, is_any_of(","));
			trim_left_if(w_fieldValue, is_any_of(","));
			// splitting string to retrieve Cxxx cmd no
			vector<string> strs;
			boost::split(strs, w_fieldValue, boost::is_any_of(","));
			aBoDucField.m_noCmd = strs.back();
		}//else

		std::cout << "Cmd is : " << aBoDucField.m_noCmd.c_str() << "\n";
		return; // shouldn't do that, but no choice
	}

	void BoDucSignatureSplit::readShippedTo( BoDucFields& aBoDucField)
	{
		using namespace std;
		using namespace boost;

		// retrieve field value which is just below the string keyword found
		std::string w_fieldValue = m_vecoflines[++m_vecPos];

		string w_addressValue = w_fieldValue; // initialize
		string w_completeAddress;     // store part of the complete address
		vector<string> w_addressPart;

		do // address may take 2 cells, if more it's because of 
		{  // conversion mismatch and generally we use the last 2 lines
			 // put in comment for now, may be in use in the future
			 //			w_addressValue.erase(0, 1); // erase first three char
			 //			w_addressValue.erase(w_addressValue.size() - 1);

			trim_right_if(w_addressValue, is_any_of(","));
			trim_left_if(w_addressValue, is_any_of(","));
			w_addressPart.push_back(w_addressValue);
			//w_completeAddress += w_addressValue + std::string(" ");
			w_addressValue = m_vecoflines[++m_vecPos];
		} while (!contains(w_addressValue, "Contact"));

		if (w_addressPart.size() > 2)
		{
			// we need to take the last 2 in most cases
			using riter = std::reverse_iterator<std::vector<std::string>::iterator>;
			riter w_lastPart = w_addressPart.rbegin() + 1;
			w_completeAddress += *w_lastPart + std::string(" ");
			w_lastPart = w_addressPart.rbegin();
			w_completeAddress += *w_lastPart;
			aBoDucField.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
		}
		else if (w_addressPart.size() == 2)
		{
			// rebuild the whole address
			w_completeAddress += w_addressPart[0] + std::string(" ");
			w_completeAddress += w_addressPart[1];
			aBoDucField.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
		}
		else if (w_addressPart.size() == 1)
		{
			w_completeAddress += w_addressPart[0];
			aBoDucField.m_deliverTo = w_completeAddress; // assume that the last field is the right one
		}
		else
		{
			std::cout << "There is a problem with Customer address\n";
		}
	//	break;
	}

	void BoDucSignatureSplit::readDatePromise(BoDucFields& aBoDucField)
	{
		using namespace std;
		using namespace boost;

		// retrieve field value which is just below the string keyword found
		std::string w_fieldValue = m_vecoflines[++m_vecPos];

		vector<string> strs;
		boost::split(strs, w_fieldValue, boost::is_any_of(","));
		strs.erase(std::remove_if(strs.begin(), strs.end(), // delete empty element
			[](const std::string& s)
		{ return s.empty(); }));
		
		//			std::unique(strs.begin(), strs.end());
		strs.erase(std::unique(strs.begin(), strs.end()), strs.end());
		std::vector<std::string> w_datepromise;
		//boost::split(w_datepromise, strs[1], boost::is_any_of(" ")); // need to be split
		boost::split(w_datepromise, strs[strs.size() - 3], boost::is_any_of(" ")); // need to be split
		
		aBoDucField.m_datePromise = w_datepromise[0];
	}

	void BoDucSignatureSplit::readProductDescr( BoDucFields & aBoDucField)
	{
		short w_tmp = m_vecPos;

		// for some command we don't get
		std::string w_prodDescr1 = m_vecoflines[m_vecPos++]; // product description
																									// check if the first number is a digit
		std::locale loc;
		if (!std::isdigit(static_cast<unsigned char>(w_prodDescr1[0]), loc))
		{
			//need to do something, check for next line
			w_prodDescr1 = m_vecoflines[++w_tmp]; // next line

			// check again for a digit as the first character
			std::locale loc;
			if (std::isdigit(static_cast<unsigned char>(w_prodDescr1[0]), loc))
			{
				// check if it is equal to 1
				int w_descrNo = boost::lexical_cast<int>(w_prodDescr1[0]);
				if (w_descrNo == 1)
				{
					// we are at right place in the command
					std::cout << "Product number is: " << w_descrNo << "\n";
				}
			}
		}//if

		std::vector<std::string> w_prodDescrp1;
		//	w_prodDescrp1.reserve(10);
		boost::split(w_prodDescrp1, w_prodDescr1, boost::is_any_of(" "));
		if (w_prodDescrp1.size() != 1)
		{
			// remove all empty string
			w_prodDescrp1.erase(std::remove(w_prodDescrp1.begin(), w_prodDescrp1.end(), std::string("")));
			auto w_findIt = std::find_if(w_prodDescrp1.cbegin(), w_prodDescrp1.cend(),
				[](const std::string& aStr) -> bool {return aStr.empty(); }
			);
			
			using diff_type = std::iterator_traits<std::vector<std::string>::iterator>::difference_type;
			diff_type w_dif = std::distance(w_prodDescrp1.cbegin(), w_findIt);
			//std::string w_lastStr = w_findIt-1;
			//w_prodDescrp1.shrink_to_fit();

			// check last element
			std::string& w_lastOne = w_prodDescrp1[w_dif - 1];
			std::string::size_type w_foundComma = w_lastOne.find_first_of(',');
			if (w_foundComma != std::string::npos)
			{
				// we found something (eraae all characters to end)
				w_lastOne.erase(w_lastOne.cbegin() + w_foundComma, w_lastOne.cend());
			}

			auto w_begp1 = w_prodDescrp1.cbegin() + 1; // first element is the number, skip
			std::string w_rebuildStr;
			do
			{
				w_rebuildStr += *w_begp1++ + std::string(" ");
			} while( !(*w_begp1).empty());

			// there anymore description
			//	std::string w_prodDescr2 = aCmdVec[++vecPos]; in the first version of the algorithm, 
			std::string w_wholeDesr = w_rebuildStr + std::string(" "); //+ w_prodDescr2;
			aBoDucField.m_produit = w_wholeDesr;
		}
	}

	// NOTE: this algorithm is to re-factored. In the next version we will look
	// for "Silo" keyword and then we have all information that we are looking for.
	// We have replaced the iterator by vector value at first line.
	void BoDucSignatureSplit::readQtyAndSiloNo( BoDucFields & aBoDucField)
	{
		// Qty and silo
		const std::string& w_qtySilo = m_vecoflines[m_vecPos]; //*begVec(original);
		auto pos = w_qtySilo.find_first_of("TM");
		if( pos != std::string::npos)
		{
			std::string w_str2Trim(w_qtySilo.cbegin() + pos, w_qtySilo.cend());
			// want to retrieve Qty and Silo
			std::vector<std::string> w_splitStr;
			boost::split(w_splitStr, w_str2Trim, boost::is_any_of(","));
			// remove all empty string, then we have as first element the "TM" string
			// just after we have the qty and silo 
			w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
				[](const std::string& s)
			{ return s.empty(); }));
			//	assert(3==w_splitStr.size());

			auto w_look4TM = std::find(w_splitStr.cbegin(), w_splitStr.cend(), std::string("TM"));
			std::advance(w_look4TM, 1);
			float w_qty = std::stof(*w_look4TM);
			aBoDucField.m_qty = w_qty;
			//	iss.str(*w_bp1);
			std::advance(w_look4TM, 1);
			// check if it is a digit first, if not don't put anything
			int w_silo = std::stoi(*w_look4TM);
			aBoDucField.m_silo = w_silo;
		}
	}

} // End of namespace

