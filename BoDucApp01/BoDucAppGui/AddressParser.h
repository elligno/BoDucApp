#pragma once

#include <iostream>
#include <forward_list>
#include <vector>
#include <map>
#include <list>

namespace bdGui 
{
	// Parse ("Shipped To") address and fix malfunction in the address. 
	// Not sure yet, but it some kind of algorithm or solver that process
	// address and check/fix part or whole 
	// Some kind of a smart algorithm that treat addresses and rebuild them.
	// Patterns repeated (most of them have been identified)
	// Maybe define a pattern type
	//
	// #include <boost/algorithm/string.hpp>
	// Or, for fewer header dependencies:
	//#include <boost/algorithm/string/predicate.hpp>
	//
	// 	std::string str1 = "hello, world!";
	// 	std::string str2 = "HELLO, WORLD!";
	// 
	// 	if (boost::iequals(str1, str2))
	// 	{
	// 		// Strings are identical
	// 	}

	// Responsibility:
	//   
	class AddressParser
	{
	public:
		// all address format supported 
		enum class eAddressFmt  : unsigned { OneLine=1, TwoLines=2, ThreeLines=3, FourLines=4, FiveLines=5, SixLines=6 };
		// Some of the malfunction pattern that we have ....
		enum class eMalfunctionsPattern : char { duplicateCity, duplicateAddress, middleLines, asymetry3L, asymetry5L, symetry}; // more coming

	public:
		using stdvecStr = std::vector<std::string>;
		using maplineValue = std::map<unsigned/*line number*/, std::string/*value*/>;
		using mapfmt2malfunctions = std::map<const eAddressFmt, std::forward_list<eMalfunctionsPattern>>;

	public:
		// default ctor
		AddressParser() = default;
		AddressParser( const stdvecStr& aVecOfAddress);
		AddressParser( const std::initializer_list<std::string>& aListOfAddress);
		
		// copy and assignment ctor
		AddressParser( const AddressParser& aOther);
		AddressParser& operator= (const AddressParser& aOther);

		// move semantic ctor
		AddressParser( AddressParser&& aOther);
		AddressParser& operator= (AddressParser&& aOther);

		// services provided (its not all clear in my mind, i put some thoughts)
// 		virtual void trimLastLine();
// 		virtual void checkLines();
// 		virtual bool cmpAddress( const std::string& aAddress2Cmp);
// 		virtual stdvecStr splitAddress(/*boost token*/);
		virtual bool isValid(); // check for malfunctions
		//bool isRebuilded();
		bool isSymetric(); // lines duplicated in the Shipped address
		
		// setter/getter
		eMalfunctionsPattern getMalfunctionsPattern() { return m_patternType; }
		eAddressFmt getAddressFmt() const { return m_addressFmt; };
		virtual std::string getWholeAddress() { return std::string(""); }
		mapfmt2malfunctions getMapFmtToMalfunctions() const { return m_mapFmtWithBugs; }

		//void asVector();
		short nbOfLines() const { return m_nbOfLines; }
		bool checkDuplicateString( const std::string& aLine2Cmp);
		bool checkDuplicateLines(const eAddressFmt aPattern);
		bool checkRepeatingChar( const std::string str=" "); // white space as default
	private:
		stdvecStr m_addressVec; // original vector containing the address
		stdvecStr m_rebuilded;  // 
		maplineValue m_lineValue;
	
		//	AddressStruct m_addressFields;
		short m_nbOfLines=0;
		bool m_isRebuild=false;
		
		eAddressFmt m_addressFmt;
		eMalfunctionsPattern m_patternType;
		
		// not completed
		mapfmt2malfunctions m_mapFmtWithBugs;
		// helpers
		void createMapFmt2Malfunc();
		eAddressFmt checkFormat( const stdvecStr::size_type aSize);
		bool checkAddressHasMalfunctions(const stdvecStr& aVec);
	};
} // End of namespace