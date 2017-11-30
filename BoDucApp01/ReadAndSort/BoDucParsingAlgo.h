#pragma once

// stl includes
#include <map>
#include <vector>
// app include
#include "StructUtils.h"

namespace boduc 
{
	// forward declaration
  class BoDucBonLivraisonAlgorithm;
}

namespace boduc 
{
	// abstract base class
	class BoDucParsingAlgo 
	{
	public:
		// create alias
		using vecofstr = std::vector<std::string>;
		using mapIntVecstr = std::map<int, vecofstr>;
	public:
		BoDucParsingAlgo() = default;		
		// copy and assign ctor not allowed
		BoDucParsingAlgo( const BoDucParsingAlgo& aOther) = delete;
		BoDucParsingAlgo& operator= ( const BoDucParsingAlgo& aOther) = delete;

		// main algorithm to parse the csv file format of BoDuc
		virtual void parseCsv();         // apply filter to grab fields value
		virtual void parseCsv( boduc::BoDucBonLivraisonAlgorithm* aReader);
		
		// deprecated
		virtual size_t nbOfCmd( const std::string& aCmdFile) const; // return the number of command to process
		// deprecated
		virtual void readFile( const std::string& aFileAnPath, 
			const std::string& aSplitBill= "Ordered on"); // parse and fill data structure 
		// deprecated
		virtual void createReport( const std::string& aBonDeLivraison="BonDeLivraison.txt"); //
		
		// will need to set some keyword of ... in the parsing process
		bool containsMoreThanOne() const { return true; }
		
		// deprecated
		inline const mapIntVecstr readedMap() 
		{ return std::map<int, std::vector<std::string>>(m_mapintVec.cbegin(), m_mapintVec.cend()); }

	protected:
		// deprecated
		virtual void NoCmdAlgo( const std::vector<std::string>& aVecOfLine)
		{
			// check if it contains ordered on
			// if so no need to process anymore, just read the cmd number

			short linenb(0);
			// need to check for "Date" first of
			for( const std::string& w_checkStr : aVecOfLine)
			{
				// looking for "Date" then break
				if( boost::algorithm::contains(w_checkStr, "Date"))
				{
					linenb++; // line below which contains the command number
					break;
				}
				linenb++;
			}
		}
	//	virtual void ShippedToAlgo() {/*not implemented yet*/}
		bool useTM(const std::vector<std::string>& aVecOfCmdLines);
	private:
		mapIntVecstr m_mapintVec;
		std::vector<BoDucFields> m_vecOfStruct;
	};
} // End of namespace
