#pragma once

// App include
#include "BonLivraisonAlgorithm.h"
#include"StructUtils.h"

namespace boduc 
{
	class BoDucSignatureSplit : public BoDucBonLivraisonAlgorithm 
	{
	public:
		// ctor summary
		BoDucSignatureSplit( std::initializer_list<std::string>& aListOfKeywords);
		void fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField) override;
	protected:
		// Design Note: can be part of a separate class   
		virtual void readNoCmd( BoDucFields& aBoDucField);
		virtual void readShippedTo( BoDucFields& aBoDucField);
		virtual void readDatePromise( BoDucFields& aBoDucField);
		virtual void readProductDescr( BoDucFields& aBoDucField);
		virtual void readQtyAndSiloNo( BoDucFields& aBoDucField);
	private:
		short m_vecPos=0; // initialize for all ctor
		std::vector<std::string> m_vecOfKeywords;
		std::vector<std::string> m_vecoflines;
	};
} // End of namespace
