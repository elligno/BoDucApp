#pragma once

#include <vector>
#include "StructUtils.h"

namespace boduc 
{
	//
	// Base class for all algorithm that parse BoDucStruct 
	// that has been read from a csv file format. To be completed!!
	//
	class BoDucBonLivraisonAlgorithm
	{
	public:
		virtual void fillBoDucFields( const std::vector<std::string>& aCmdVec, BoDucFields& aBoDucField) = 0;
		// shall i provide a virtual dtor?? see Scott Meyers book, i think there is one item about this
		virtual ~BoDucBonLivraisonAlgorithm() = default;
	};
} // End of namespace
