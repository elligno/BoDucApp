// boost includes
#include <boost/algorithm/string.hpp>
// app includes
#include "AddressParser.h"
#include "AddressBuilder.h"
#include "BuildShippedToAlgo.h"
#include "TestAddressParserImpl.h"

using namespace bdGui;

// void bdGui::TestShippedToImpl::readShippedTo(const vecofstr & aFieldValue, BoDucFields & aField)
// {
// 	using namespace std;
// 	using namespace boost;
// 	using namespace boost::algorithm;
// 
// 	// fill complete address
// 	aBoducF.m_deliverTo = rebuildShippedTo(aFieldValue);
// }

// This method is a big mess. Too many if it means 
// bad design. Need more concept and isolate the 
// functionalities of each class.
// Want to get rid of all these "if case" and support 
// to address new malfunctions in the future an easy manner. 
std::string TestShippedToImpl::rebuildShippedTo( const vecofstr& aAddressPart)
{
	// Hilite (steps)
	// --- parser take the original address
	// --- check for malfunctions in the address
	// --- take some action according to last step 
	// --- no malfunction, just build the address and return it
  // --- malfunctions, then we need to know more 
	// --- form last step, call algorithm (BuildShippedToAlgo)
	// --- Build correct address from return of the algo 

	// Create some alias
	using buildAlgoPtr = std::unique_ptr<IBuildShippedToAlgo>;
	using adrsStruct = AddressBuilder::AddressStruct;
	
	// Create an address parser with default construct
	AddressParser w_adress2Parse;

	// first we want to know if the address is valid
	// then take some actions to fix it (calling the 
	// right build algorithm and return the correct address).
	if( w_adress2Parse.isValid())
	{
		return std::string(); // temp fix for debugging purpose
	}

	// need to investigate further
	AddressParser::eAddressFmt w_fmt2Look = w_adress2Parse.getAddressFmt();
	AddressParser::eMalfunctionsPattern w_pattern2Look = w_adress2Parse.getMalfunctionsPattern();

	// according to address format and malfunctions, call the algorithm
	buildAlgoPtr w_buildAlgo = 
		std::make_unique<VictoBuildShippedToAlgo>(w_adress2Parse.getMapFmtToMalfunctions());
	
	// check for success
	if( nullptr == w_buildAlgo)
	{
		return std::string();
	}

	// Now we can call the right algorithm to fix our address
	if( w_fmt2Look == AddressParser::eAddressFmt::TwoLines &&
		  w_pattern2Look == AddressParser::eMalfunctionsPattern::duplicateAddress)
	{
		adrsStruct w_addressRet = w_buildAlgo->twoLinesAlgo(aAddressPart,w_pattern2Look);

		//start to build the address to be returned
		AddressBuilder w_buildedAddress(w_addressRet);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	std::string w_rebuildStr;

	// NOTE case ==4 and >4, this situation arise for the first 2 element
	// of the billing file (probably because of the conversion from Excell to csv).
	// The former, we fix it by taking the last 2 lines of the complete address.
	// We consider that since we have a kind of pairing 2-2, its jsut a duplicate
	// Then we take the last 2 since its probably the "Shipped to" address. 
	// The latter need more attention, mangling addresses between "Sold To" and "Shipped To" 
	if (aAddressPart.size() > 4) // case 5 (to be completed)
	{
		if (aAddressPart.size() == 6)
		{
			// shall check for symmetry part in the address
			w_rebuildStr += aAddressPart[3] + std::string(" ");
			w_rebuildStr += aAddressPart[4] + std::string(" ");
			w_rebuildStr += aAddressPart[5];
			return w_rebuildStr;
		}

		// we need to take the last 2 in most cases
		using criter = std::reverse_iterator<std::vector<std::string>::const_iterator>;
		criter w_lastPart = aAddressPart.crbegin() + 1; // start one before last
		auto w_start = aAddressPart.cbegin();
		w_rebuildStr += *w_start; // should be the client name, but not sure
		w_rebuildStr += *w_lastPart + std::string(" ");
		w_lastPart = aAddressPart.crbegin();
		w_rebuildStr += *w_lastPart;
	}
	else if (aAddressPart.size() == 4)
	{
		// we need to take the last 2 in most cases
		// symetry, 
		using criter = std::reverse_iterator<std::vector<std::string>::const_iterator>;
		criter w_lastPart = aAddressPart.crbegin() + 1;
		w_rebuildStr += *w_lastPart + std::string(" ");
		w_lastPart = aAddressPart.crbegin();
		w_rebuildStr += *w_lastPart;
		return w_rebuildStr;
	}
	else if (aAddressPart.size() == 3) // need to add a test
	{
		// rebuild the whole address humm!! not sure
		//			w_rebuildStr += aAddressPart[0] + std::string(" ");
		w_rebuildStr += aAddressPart[1] + std::string(" ");
		w_rebuildStr += aAddressPart[2];
	}
	else if (aAddressPart.size() == 2) // 2-lines address
	{
		// rebuild the whole address
		// NOTE we have seen some cases (most of the times first address mixed up)
		// some stuff from "Sold To" inserted in "Shipped To" when converting pdf to csv
		// I think we must do some pre-treatment for the last line and check fields
		// city, province and postal code no more than that
		// split the last line with token set to comma, we should get these fields
		// if we have size > 3, some action must be taken
		std::vector<std::string> w_addressFields;
		boost::split(w_addressFields, aAddressPart[1], boost::is_any_of(","));
		if (w_addressFields.size() > 3)
		{
			// do something at leat check if size==6 
			if (w_addressFields.size() == 5) // number of elements in the last lines, duplicate city, prov and postal code
			{                              // some coming from "Sold To" that have been inserted here
				std::vector<std::string> w_shippedCity;
				boost::split(w_shippedCity, w_addressFields[2], boost::is_any_of(" "));
				w_shippedCity.erase(std::unique(w_shippedCity.begin(), w_shippedCity.end()), w_shippedCity.end());
				auto w_remEmpty = [](const std::string& aChar2Check) -> bool {if (aChar2Check == "") { return true; } else { return false; }};
				w_shippedCity.erase(std::remove_if(w_shippedCity.begin(), w_shippedCity.end(), w_remEmpty), w_shippedCity.end());
				// check if the first 2 element is the postal code
				auto checkDigit = [](const char aChar2Check) -> bool {if (std::isdigit(aChar2Check, std::locale())) { return true; } else { return false; }};
				if (std::find_if(w_shippedCity[1].cbegin(), w_shippedCity[1].cend(), checkDigit) != w_shippedCity[1].cend())
				{
					std::cout << "Found a digit it must be the postal code\n";
					w_rebuildStr += aAddressPart[0] + std::string(" ");
					w_rebuildStr += w_shippedCity[2] + std::string(" ");
				}
				// last element shall be the shipped city
				//w_rebuildStr += w_addressFields[2] + std::string(" "); debugging purpose put in comment
				w_rebuildStr += w_addressFields[3] + std::string(" ");
				w_rebuildStr += w_addressFields[4];
			}
			else
			{
				w_rebuildStr += aAddressPart[0] + std::string(" ");
				w_rebuildStr += aAddressPart[1];
				// just don't know what to do
				std::cerr << "Shipped address case don't know what to do\n";
			}
		}
		else
		{
			w_rebuildStr += aAddressPart[0] + std::string(" ");
			w_rebuildStr += aAddressPart[1];
		}
	}
	else if (aAddressPart.size() == 1)
	{
		// no special treatment, take it as it is
		w_rebuildStr += aAddressPart[0];
	}
	return w_rebuildStr;
}
