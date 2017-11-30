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
#include "StructUtils.h"
#include "VictoBonLivraison.h"

boduc::VictoBonLivraison::VictoBonLivraison()
: m_fieldKey({ "No Command","Shipped to","Date promise","produit et description" })
{
}

boduc::VictoBonLivraison::~VictoBonLivraison()
{
	std::cout << "Nothing to do\n";
}

void boduc::VictoBonLivraison::fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField)
{
	// contains the original algorithm steps algorithm
	using namespace std;
	using namespace boost;
	using namespace boost::algorithm;

	// create a reader to retrieve field value 
	unique_ptr<BoDucReader> w_boducReader = make_unique<VictoReader>();

	// this is a temporary fix because we have some problem 
	// with the list initializer
	vector<string> w_tmpFix = { string("No Command"),string("Shipped to"),
		string("Date promise"),string("produit et description") };

	// go through each key word 
	for( const std::string& w_field2Look : w_tmpFix)
	{
		// work around to take account the CSV formatting when cmd are tied together  
		if( ::strcmp( w_field2Look.c_str(), "No Command") == 0)
		{
			w_boducReader->readNoCmd(aCmdVec, aBoDucField);
			continue;
		}

		int linenb(0);
		short vecPos(0);
		auto begVec = aCmdVec.begin();

		// go through all element cmd line
		while (begVec != aCmdVec.cend())
		{
			const std::string& str2look = *begVec;
			if( contains( str2look, w_field2Look.c_str()))
			{
				// retrieve the cmd no. which is next line
				std::string w_fieldValue = aCmdVec[++vecPos];

				// retrieve the address  
				if( ::strcmp("Shipped to", w_field2Look.c_str()) == 0)
				{
					std::string w_addressValue = w_fieldValue; // initialize
					std::string w_completeAddress;             // store part of the complete address
					std::vector<string> w_addressPart;

					do // address may take 2 cells, if more it's because of 
					{  // conversion mismatch and generally we use the last 2 lines
						trim_right_if(w_addressValue, is_any_of(","));
						w_addressPart.push_back(w_addressValue);
						//w_completeAddress += w_addressValue + std::string(" ");
						w_addressValue = aCmdVec[++vecPos];
					} while (!contains(w_addressValue, "Contact"));
					
					w_boducReader->readShippedTo(w_addressPart, aBoDucField);
					break;
				}

				if( ::strcmp("Date promise", w_field2Look.c_str()) == 0)
				{
					w_boducReader->readDeliveryDate(w_fieldValue, aBoDucField);
					break;
				}

				if( ::strcmp("produit et description", w_field2Look.c_str()) == 0)
				{
					short w_tmp = vecPos;
					// for some command we don't get
					std::string w_prodDescr1 = aCmdVec[vecPos++]; // product description

					// check if the first number is a digit
					std::locale loc;
					if( !std::isdigit( static_cast<unsigned char>(w_prodDescr1[0]), loc))
					{
						// need to do something, check for next line
						w_prodDescr1 = aCmdVec[++w_tmp]; // next line

						// we are reading the value field for product code 
						w_boducReader->readProdCode(w_prodDescr1, aBoDucField);
						// we are reading the value field for product description 
						w_boducReader->readProdDescr(w_prodDescr1,aBoDucField);
						// we are reading the value field for ...
						auto w_lineVec = aCmdVec.cbegin() + vecPos;
						while( w_lineVec != aCmdVec.cend())
						{
							w_boducReader->readQtySilo(*w_lineVec, aBoDucField);
							if( aBoDucField.m_qty!=0.f && !aBoDucField.m_silo.empty())
							{
								break; // found it, no point to go further
							}
							++w_lineVec;
						}
					}
					break;
				}
			}//if-contains
			++begVec;
			++vecPos;
		}//while-loop
	}//for-loop
}