#include "AddressParser.h"
#include "BuildShippedToAlgo.h"

namespace bdGui 
{
	void VictoBuildShippedToAlgo::sixLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)
	{
		//	AddressParser w_address2Parse(m_vecOfAddressPart);

		// shall check for symmetry part in the address
		// 	if( w_address2Parse.isSymetric())
		// 	{
// 		std::string w_str2Build;
// 		aVecStrPart += m_vecOfAddressPart[3] + std::string(" ");
// 		aVecStrPart += m_vecOfAddressPart[4] + std::string(" ");
// 		aVecStrPart += m_vecOfAddressPart[5];
		//	}
	}

	void VictoBuildShippedToAlgo::fiveLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)
	{
		// we need to take the last 2 in most cases
// 		using criter = std::reverse_iterator<std::vector<std::string>::const_iterator>;
// 		criter w_lastPart = m_vecOfAddressPart.crbegin() + 1; // start one before last
// 		auto w_start = m_vecOfAddressPart.cbegin();
// 		aVecStrPart += *w_start; // should be the client name, but not sure
// 		aVecStrPart += *w_lastPart + std::string(" ");
// 		w_lastPart = m_vecOfAddressPart.crbegin();
// 		aVecStrPart += *w_lastPart;
	}

	void VictoBuildShippedToAlgo::fourLineslAgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)
	{
		AddressParser w_address2Parse(m_vecOfAddressPart);
// 		if (w_address2Parse.isSymetric())
// 		{
// 			// then we have a symmetry about the address, 
// 			// take last 2 lines
// 			aVecStrPart += m_vecOfAddressPart[2] + std::string(" ");
// 			aVecStrPart += m_vecOfAddressPart[3];
// 		}
	}

	void VictoBuildShippedToAlgo::threeLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)
	{
		// IMPORTANT: 
		// rebuild the whole address humm!! not sure
		//			w_rebuildStr += aAddressPart[0] + std::string(" ");
// 		aVecStrPart += m_vecOfAddressPart[1] + std::string(" ");
// 		aVecStrPart += m_vecOfAddressPart[2];
	}
#if 0

	IBuildShippedToAlgo::adrstruct 
		VictoBuildShippedToAlgo::twoLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)
	{
		auto w_twoLinesIter = m_mapfmt2bugs.find(AddressParser::eAddressFmt::TwoLines);
		
		// find in the map our list of malfunctions, and treat each one separately
		if( w_twoLinesIter != m_mapfmt2bugs.cend())
		{
			// now check for the malfunctions
			if( AddressParser::eMalfunctionsPattern::duplicateAddress == aMalfunc)
			{
				// return last line
				return AddressBuilder::AddressStruct(aVecStrPart[1]);
			}
		}

		// check malfunctions list for this case
		return AddressBuilder::AddressStruct();
	}

#endif
} // End of namespace
