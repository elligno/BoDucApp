#pragma once

#include "VictoReader.h"

namespace bdGui 
{
	// Implementing the rebuildShippedTo() method 
	// by using new data type and new concepts such as
	// address format and malfunctions in the shipped to 
	// field (wrong format of the address). 
	class TestShippedToImpl : public VictoReader 
	{
	public:
	protected:
		std::string rebuildShippedTo( const vecofstr& aAddressPart) override final;
	};
} // End of namespace
