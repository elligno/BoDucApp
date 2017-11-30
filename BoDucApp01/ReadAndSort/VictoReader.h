#pragma once

#include "BoDucReader.h"

namespace boduc 
{
	class VictoReader : public BoDucReader 
	{
		// alias
		using vecofstr = std::vector<std::string>;
	public:
		VictoReader();
		~VictoReader();
		virtual void readNoCmd( const vecofstr& aCmdVec, BoDucFields& aBoducF) override;
		virtual void readShippedTo(const vecofstr& aFieldValue, BoDucFields& aBoducF) override;
		virtual void readDeliveryDate( const std::string& aFiedValue, BoDucFields& aBoducF) override;
		virtual void readProdDescr( const std::string& aCmdVec, BoDucFields& aBoducF) override;
		virtual void readQtySilo(const std::string & aFieldValue, BoDucFields& aBoducF) override;
		virtual void readProdCode( const std::string& aFieldValue, BoDucFields& aBoducF) override;
		// shall be in the base class
		size_t nbOfCmd(const std::string& aCmdFile) const;
	protected:
		virtual std::string rebuildShippedTo( const vecofstr& aAddressPart);
	  std::size_t m_nbCmd=0;
	};
} // End of namespace
