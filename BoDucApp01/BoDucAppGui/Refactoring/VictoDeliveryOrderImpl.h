#pragma once

// App include
#include "VictoBonLivraison.h"

namespace bdGui 
{
	class VictoDeliveryOrderImpl : public VictoBonLivraison 
	{
	public:
		// default list of keyword 
		VictoDeliveryOrderImpl(const std::initializer_list<std::string>& aListOfKeyword = 
		{ std::string("No Command"),std::string("Shipped to"),
			std::string("Date promise"), std::string("produit et description") });

		// main algorithm
		void fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField) override final;
	private:
		std::initializer_list<std::string> m_listKeyword;
		std::vector<std::string> fillAddressPartVec( const std::vector<std::string>::iterator aBegIter);
		// some helper that could be wrapped into a class (BoDucFieldWriter for example)
		void setBoDucFieldCmdNo( std::vector<std::string>::const_iterator aIter);
		void setBoDucFieldShippedTo( std::vector<std::string>::const_iterator aIter);
		void setBoDucFieldDeliveryDate( std::vector<std::string>::const_iterator aIter);
		void setBoDucFieldProduct( std::vector<std::string>::const_iterator aIter);
	};
} // End of namespace
