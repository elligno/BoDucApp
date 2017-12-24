#pragma once
// C++ include
#include <string>
// STL include
#include <vector>
// App includes
#include "AddressParser.h"
#include "AddressBuilder.h"

namespace bdGui 
{
	// Abstract class to implement address correctness
	class IBuildShippedToAlgo 
	{
	public:
		using stdvecstr = std::vector<std::string>;
	//	using adrstruct = AddressBuilder::AddressStruct;
		using AddressMalfunc = AddressParser::ePattern;
	public:
		// build the shipped address according to pattern
		virtual void sixLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)=0;
		virtual void fiveLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)=0;
		virtual void fourLineslAgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)=0;
		virtual void threeLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc)=0;
//		virtual adrstruct twoLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) = 0;
	};

	class VictoBuildShippedToAlgo : public IBuildShippedToAlgo
	{
	public:
		//list of malfunctions for each format
	//	VictoBuildShippedToAlgo( AddressParser::mapfmt2malfunctions aList);
		
		// services
		void sixLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) override;
		void fiveLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) override;
		void fourLineslAgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) override;
		void threeLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) override;
	//	adrstruct twoLinesAlgo( const stdvecstr& aVecStrPart, const AddressMalfunc aMalfunc) override;
	protected:
		// can be overloaded by subclass
		// services provided (its not all clear in my mind, i put some thoughts)
		virtual void trimLastLine();
		virtual void checkLines();
		virtual bool cmpAddress( const std::string& aAddress2Cmp);
		virtual void splitAddress(/*boost token*/);
	private:
		std::vector<std::string> m_vecOfAddressPart;
	//	AddressParser::mapfmt2malfunctions m_mapfmt2bugs;
	};
} // End of namespace
