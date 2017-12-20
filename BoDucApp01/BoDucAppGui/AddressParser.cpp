// Boost algorithm includes
#include <boost/algorithm//string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/iter_find.hpp>
#include <boost/algorithm/string/predicate.hpp>

// App include
#include "AddressParser.h"

namespace bdGui 
{
	AddressParser::AddressParser( const stdvecStr& aVecOfAddress)
	{
		/*to be implemented*/
// 		if( aVecOfAddress.size()==2)
// 		{
// 			m_patternType = eAddressFmt::TwoLines;
// 		}
	}

	AddressParser::AddressParser( const AddressParser& aOther) 
	: m_addressVec(aOther.m_addressVec),
		m_rebuilded(aOther.m_rebuilded),
		m_lineValue(aOther.m_lineValue)
//		m_addressFields(m_addressFields)
	{
		/*to be implemented*/
	}

	AddressParser::AddressParser(AddressParser&& aOther) 
	: m_addressVec(std::move(aOther.m_addressVec)),
		m_rebuilded(std::move(aOther.m_rebuilded)),
		m_lineValue(std::move(aOther.m_lineValue)),
//		m_addressFields(m_addressFields),
		m_nbOfLines(aOther.m_nbOfLines),
		m_isRebuild(aOther.m_isRebuild),
		m_patternType(aOther.m_patternType)
	{
		std::cout << "We are in the move ctor of AddressParser\n";
	}

	AddressParser& AddressParser::operator=(const AddressParser& aOther)
	{
		// check for self-assignment
		if (this != &aOther)
		{
			m_addressVec = aOther.m_addressVec;
			m_rebuilded = aOther.m_rebuilded;
			m_lineValue = aOther.m_lineValue;
	//		m_addressFields = m_addressFields;
		}
		return *this;
	}

	bdGui::AddressParser& AddressParser::operator=( AddressParser&& aOther)
	{
		std::cout << "We are in the assignment ctor of AddressParser\n";
		// check for temporary or something like that
		if (&aOther != this)
		{
			// do i have to clear content of the vector (size==0 and capacity)
			if (!m_addressVec.empty())
			{
				m_addressVec.clear(); // size shall be zero
			}
			if (!m_rebuilded.empty())
			{
				m_rebuilded.clear(); // size shall be zero
			}
			if (!m_lineValue.empty())
			{
				m_lineValue.clear(); // size shall be zero
			}
			// ready to to set new values 
			m_addressVec = std::move(aOther.m_addressVec);
			m_rebuilded = std::move(aOther.m_rebuilded);
			m_lineValue = std::move(aOther.m_lineValue);
//			m_addressFields = m_addressFields;
			m_nbOfLines = aOther.m_nbOfLines;
			m_isRebuild = aOther.m_isRebuild;
			m_patternType = aOther.m_patternType;
		}

		return *this;
	}

// 	void AddressParser::trimLastLine()
// 	{
// 		/*check VictoReader rebuild..() function */
// 	}
// 
// 	void AddressParser::checkLines()
// 	{
// 		/*what i want to do??*/
// 	}
// 
// 	bool AddressParser::cmpAddress( const std::string& aAddress2Cmp)
// 	{
// 		/*compare to equality, they are the same*/
// 		return false;
// 	}
// 
// 	AddressParser::stdvecStr AddressParser::splitAddress(/*boost token*/)
// 	{
// 		/*split the line into broken fields*/
// 		return stdvecStr();
// 	}

	// Checking for the malfunctions in the address
	bool AddressParser::isValid()
	{
		bool w_returnValid = false;
		// check format (one, two, ... six lines)
		m_addressFmt = checkFormat(m_addressVec.size());

		switch( m_addressFmt)
		{
		case eAddressFmt::OneLine:
			w_returnValid = true;
			break;
		case eAddressFmt::TwoLines:
			if( checkAddressHasMalfunctions(m_addressVec))
			{
				if (isSymetric())
				{
					m_patternType = eMalfunctionsPattern::duplicateAddress;
				}
				w_returnValid = false;
			}
			break;
		case eAddressFmt::ThreeLines:
			if (checkAddressHasMalfunctions(m_addressVec))
			{
				w_returnValid = false;
			}
			break;
		case eAddressFmt::FourLines:
			if (checkAddressHasMalfunctions(m_addressVec))
			{
				w_returnValid = false;
			}
			break;
		case eAddressFmt::FiveLines:
			if (checkAddressHasMalfunctions(m_addressVec))
			{
				w_returnValid = false;
			}
			break;
		case eAddressFmt::SixLines:
			break;
		}

		return w_returnValid;
	}

	bool AddressParser::isSymetric()
	{
		// this case the address is duplicated
		if( m_addressVec.size()==2    || 
			  m_addressVec.size() == 4  || 
			  m_addressVec.size() == 6)
		{
			/*check for duplicated lines*/ 
// 			if( checkDuplicateLines())
// 			{
 				return true;
// 			}
// 			else
// 			{
// 				return false;
// 			}
		}
		return false;
	}

	bool AddressParser::checkDuplicateLines( const eAddressFmt aPattern)
	{
		switch (aPattern)
		{
		case eAddressFmt::FourLines:
			break;
		case eAddressFmt::SixLines :
			break;
		default:
			break;
		}
		// NOTE no need to use a reverse iterator, most cases
		// we have a symetry between the first 2 lines with the last 2
		// this will be enough in most cases, but we could add a test
		// about symetry check 1 with 3
		// and check 2 with 4 both should be equal
		// 				if( boost::iequals(aAddressPart[0], aAddressPart[2]) &&
		// 					  boost::iequals(aAddressPart[1], aAddressPart[3]))
		// 				{

		return false;
	}

	bool AddressParser::checkRepeatingChar( const std::string str/*=" "*/)
	{
		// Returns the maximum repeating character in a
		// given string
		// Design Note
		//  Need to modify to our purpose
		int n = str.length();
		int count = 0;
		char res = str[0];
		int cur_count = 1;

		// Traverse string except last character
		for( int i = 0; i < n; i++)
		{
			// If current character matches with next
			if( i < n - 1 && str[i] == str[i + 1])
				cur_count++;

			// If doesn't match, update result
			// (if required) and reset count
			else
			{
				if (cur_count > count)
				{
					count = cur_count;
					res = str[i];
				}
				cur_count = 1;
			}
		}

		return res;
	}

	// not completed, just testing 
	void AddressParser::createMapFmt2Malfunc()
	{
		using fwdlistmalfunc = std::forward_list<eMalfunctionsPattern>;
		
		// fill the map associative container between format and malfunctions
		fwdlistmalfunc w_list1OfMalFunc{ eMalfunctionsPattern::duplicateAddress };
		fwdlistmalfunc w_list2OfMalFunc{ eMalfunctionsPattern::duplicateAddress, eMalfunctionsPattern::middleLines };

		// ready to build our map
		if( !m_mapFmtWithBugs.insert( std::make_pair(eAddressFmt::OneLine, w_list1OfMalFunc)).second)
		{
			std::cerr << "Could not insert malfunctions in the map container\n";
		}
		if( m_mapFmtWithBugs.insert(std::make_pair(eAddressFmt::TwoLines, w_list2OfMalFunc)).second)
		{
		}
		//m_mapFmtWithBugs.insert( std::make_pair( eAddressFmt::OneLine, w_list1OfMalFunc));
		//m_mapFmtWithBugs.insert(std::make_pair(eAddressFmt::TwoLines, w_list2OfMalFunc));

	}
	AddressParser::eAddressFmt AddressParser::checkFormat( const stdvecStr::size_type aSize)
 {
		if( aSize==1)
		{
			return eAddressFmt::OneLine;
		}
		else if (aSize == 2)
		{
			return eAddressFmt::TwoLines;
		}
		else if(aSize == 3)
		{
			return eAddressFmt::ThreeLines;
		}
		else if (aSize == 4)
		{
			return eAddressFmt::FourLines;
		}
		else if (aSize == 5)
		{
			return eAddressFmt::FiveLines;
		}
		else if (aSize == 6)
		{
			return eAddressFmt::SixLines;
		}

		return eAddressFmt();
	}

	bool AddressParser::checkAddressHasMalfunctions(const stdvecStr& aVec)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

} // End of namespace

