#pragma once

#include "BonLivraisonAlgorithm.h"

namespace boduc 
{
	class VictoBonLivraison : public BoDucBonLivraisonAlgorithm 
	{
		enum class heywordType 
		{
			CommandNo=1,
			ShippedTo=2
		};
	public:
		VictoBonLivraison();
		~VictoBonLivraison();
		void fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField) override;
	private:
		std::initializer_list<std::string> m_fieldKey;
	};
} // End of namespace
