// C++ inc;udes 
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
// stl includes
#include <vector>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// App includes
#include "VictoReader.h"
#include "BoDucFields.h"
#include "BoDucUtility.h"
#include "VictoDeliveryOrderImpl.h"

namespace bdGui 
{
	VictoDeliveryOrderImpl::VictoDeliveryOrderImpl( const std::initializer_list<std::string>& aListOfKeyword 
		/*= { string("No Command"),string("Shipped to"),
		string("Date promise"),string("produit et description") }*/) 
	: m_listKeyword(aListOfKeyword) 
	{
		// not much to do for now
	}
	
	void VictoDeliveryOrderImpl::fillBoDucFields( const std::vector<std::string>& aCmdVec, 
		BoDucFields & aBoDucField)
	{
		// contains the original algorithm steps algorithm
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

		// Reader support some kind of algorithm to extract data
		// In many cases data is in mess state and some work must 
		// be done to remove that extra information.
		// Reader support algorithm to do that job

		// create a reader to retrieve field value 
		unique_ptr<BoDucReader> w_boducReader = make_unique<VictoReader>();
		assert( nullptr != w_boducReader);

		auto begVec = aCmdVec.cbegin();
		auto begKeywords = m_listKeyword.begin();

		// someClass.setKeywordsOrder({....});
		// if(someClass.KeyWordContained()) {....}
		// go through all element cmd lines (line-by-line)
		while( begVec != aCmdVec.cend())
		{
			// check if current lines contains one the specific keywords
			if( contains( *begVec, (*begKeywords).c_str())) // check for keyword
			{
				// since it contains one of them, which one it is
				if( ::strcmp((*begKeywords).c_str(), "Date") == 0) // correspond "No Command"
				{
 					std::vector<std::string> w_dateTrimed =
 						BoDucUtility::TrimRightCommaAndErase(*begVec);

					// check if first element is Date
					if( boost::equals(w_dateTrimed[0], "Date"))
					{
						// we found the Date keyword
						auto w_fieldValue = std::next(begVec); // field values one line below
						w_boducReader->readNoCmd(*w_fieldValue, aBoDucField);
						++begKeywords; // next keywords in the list
						std::advance(begVec, 2); // since value one line below, step 2 lines
						continue; // ready to continue to parse lines to the end of order
					}
					else
						continue; // until we find our Date keyword
				}
				if( ::strcmp((*begKeywords).c_str(), "Shipped to") == 0)
				{
					std::vector<std::string> w_addressPart;// fillAddressPartVec(begVec);
					// reader is a ...
					w_boducReader->readShippedTo(w_addressPart, aBoDucField);
					++begKeywords; // next keyword
					std::advance(begVec, 2); // ready to continue parse our order
					continue;
				}
				// delivery date 
				if (::strcmp((*begKeywords).c_str(), "Date promise") == 0)
				{
					// return a copy increment by one
					auto w_fieldValue = std::next(begVec);
					w_boducReader->readDeliveryDate(*w_fieldValue, aBoDucField);
					++begKeywords; // next keyword
					std::advance(begVec, 2); // ready to continue parse our order
					continue;
				}
				// product description
				if( ::strcmp("produit et description", (*begKeywords).c_str()) == 0)
				{
					//	short w_tmp = vecPos;
					auto w_fieldValue = std::next(begVec); // return a copy of the iterator incr by 1
					// for some command we don't get
					std::string w_prodDescr1 = *w_fieldValue; //aCmdVec[vecPos++];  product description

					// check if the first number is a digit 
					// line format is "1,432434,          ,TRUIE GESXXXXXX"
					std::locale loc;
					if (!std::isdigit(static_cast<unsigned char>(w_prodDescr1[0]), loc))
					{
						// need to do something, check for next line
						std::advance(w_fieldValue,1);
						w_prodDescr1 = *w_fieldValue;  //aCmdVec[++w_tmp];  next line

						// we are reading the value field for product code 
						w_boducReader->readProdCode(w_prodDescr1, aBoDucField);

						// we are reading the value field for product description 
						w_boducReader->readProdDescr(w_prodDescr1, aBoDucField);

						// we are reading the value field for quantity and silo number
					//	auto w_lineVec = aCmdVec.cbegin() + vecPos;
						std::advance(begVec, 2);
						auto w_lineVec = begVec;
						while( w_lineVec != aCmdVec.cend())
						{
							w_boducReader->readQtySilo(*w_lineVec, aBoDucField);
							if( aBoDucField.m_qty != 0.f && !aBoDucField.m_silo.empty())
							{
								break; // found it, no point to go further
							}
							++w_lineVec;
						}
					}
					break; // no point to go further, this is the last keywords
				}
			}//if-contains
				++begVec;//next line to process
			}//while-loop
	}

	std::vector<std::string> 
		VictoDeliveryOrderImpl::fillAddressPartVec( const std::vector<std::string>::iterator aBegIter)
	{
		using namespace std;
		using namespace boost;

		std::vector<std::string>::iterator w_cpyIter = aBegIter;
		// return a copy of the iterator incremented by one
		auto w_fieldValue = std::next(w_cpyIter);

		std::vector<string> w_addressPart;
		//TODO: remove the hard coded value, can create bugs that are though to fix
		w_addressPart.reserve(6); // max number of lines in address field
															// reserve memory to store all lines
		do
		{
			trim_right_if(*w_fieldValue, is_any_of(","));
			w_addressPart.push_back(*w_fieldValue);
			std::advance(w_fieldValue, 1); // increment the iterator 
		} while (!contains(*w_fieldValue, "Contact")); // stop criteria

																									 // check capacity and size of the vector
		if( w_addressPart.capacity() > w_addressPart.size())
		{
			// set capacity == size for efficiency
			w_addressPart.shrink_to_fit();
		}

		return std::vector<std::string>(w_addressPart.cbegin(),w_addressPart.cend());
	}
}
