// C++ includes
#include <sstream>
#include <fstream>
#include <iostream>
// STL includes
#include <map>
#include <vector>
// Boost includes
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
// test includes
#include "../AddressAlgorithm.h"
#include "../AddressBuilder.h"
#include "../AddressParser.h"

namespace jb11 
{
	// this function is used when we have a line in the 
	// address that we want to check if it ends with postal
	// code. For example in symmetry case, want to know if
	// address is duplicate or different. Need to check  
	bool hasPostalCode( const std::string& aAddress)
	{
		// split with token = ,
		// take last element of split vector and count 
		// if alpha and if digit by using the count count_if
		// stl algorithm. If both count == 3 we have a postal code

		std::locale loc;
		auto w_nbAlpha = std::count_if(aAddress.begin(), aAddress.end(),
				[loc](unsigned char c) { return std::isalpha(c,loc); } // correct
			);

		auto w_nbDigit = std::count_if(aAddress.begin(), aAddress.end(),
			[loc](unsigned char c) { return std::isdigit(c, loc); } // correct
		);

		if( w_nbAlpha == w_nbDigit)
		{
			// found the postal code
		}

		return false;
	}

	void readCmd( const std::initializer_list<std::string>& aList,
		std::map<const std::string, std::vector<std::string>>& aMapCmd) 
	{
		// readcmd
	}

	void readCmd( const std::string aFileName, std::vector<std::string>& aVecLins)
	{
		using namespace std;
		using namespace boost;

		// reading file to process
		ifstream w_fileName2Test(aFileName);
		if( w_fileName2Test)
		{
			string w_line;
			// file open succeed
			while( getline(w_fileName2Test, w_line))
			{
				if( contains(w_line, "Signature")) break;
				aVecLins.push_back(w_line);
			}
		}
// 		else
// 		{
// 			return boost::optional<int> {};
// 		}
		if( aVecLins.capacity() > aVecLins.size())
		{
			aVecLins.shrink_to_fit();
		}
	}

	std::string testParserAlgorithm( const std::vector<std::string> &w_vecOfAddressPart)
	{
		using namespace bdGui;

		std::string w_complAddrs; //whole address
    // starting to parse file
		AddressParser w_testParser(w_vecOfAddressPart); // analyze the address for malfunction
		if( w_testParser.getPattern() == AddressParser::ePattern::NoMalfunction)
		{
			// no point to go further, just return the vec address part as it is
			AddressBuilder w_adrs2Build(w_vecOfAddressPart);
			w_complAddrs = w_adrs2Build.buildAddess();
			return w_complAddrs;
		}
		if (w_testParser.getPattern() == AddressParser::ePattern::Livraison)
		{
			// call the fix algorithm for this pattern, no point to go further
			// once LIVRAISON is removed for the whole address, usually we have 
			// to extract the lines below and take the first line as client name
			// Need to check but we already do that 
			std::vector<std::string> w_adrsFixed =
				AddressAlgorithm::fixLivraisonInserted(w_testParser);

			// build the whole address
			AddressBuilder w_addrsBuilder(w_adrsFixed);
			w_complAddrs = w_addrsBuilder.buildAddess();
			std::cout << "The complete address is: " << w_complAddrs.c_str() << "\n";
			return w_complAddrs;
		}
		if (w_testParser.hasSymmetry())
		{
			std::vector<std::string> w_fixedAddress;
			if (w_testParser.hasMalfunctions())
			{
				AddressParser::ePattern w_checkPattern = w_testParser.getPattern();
				if (w_checkPattern == AddressParser::ePattern::DuplAddressSymmetric)
				{
					// call address malfunction algorithm
					w_fixedAddress = AddressAlgorithm::fixSymetryDuplAddress(w_testParser);

					// build the whole address
					AddressBuilder w_addrsBuilder( std::vector<std::string>(w_fixedAddress.cbegin(), w_fixedAddress.cend()));
					w_complAddrs = w_addrsBuilder.buildAddess();
				}
				else if (w_checkPattern == AddressParser::ePattern::DuplAddressNonSymmetric)
				{ // call address malfunction algorithm
					w_fixedAddress =
						AddressAlgorithm::fixNonSymetryDuplAddress(w_testParser.getNbLines(),
							w_testParser.getVecPart());

					// build the whole address
					AddressBuilder w_addrsBuilder(w_fixedAddress);
					w_complAddrs = w_addrsBuilder.buildAddess();
				}
				else if (w_checkPattern == AddressParser::ePattern::LastLineSrewUp)
				{
					std::vector<std::string> w_splitLine = AddressAlgorithm::splitAboutComma(w_vecOfAddressPart.back());
					if( std::any_of(w_splitLine.cbegin(), w_splitLine.cend(), [](const std::string& aStr) {return aStr.empty(); }))
					{
						w_splitLine.erase(std::unique(w_splitLine.begin(), w_splitLine.end()), w_splitLine.cend()); // white space
						w_splitLine.pop_back(); // erase last character which is  
					}

					//boost::trim_right_if(w_splitLine);
					assert(5==w_splitLine.size());
					if (!w_splitLine.empty())
					{
						// we need to split about comma and then pass to algorithm
						w_fixedAddress = AddressAlgorithm::fixLastLinesCityDupl(w_splitLine);
						const std::string& w_val2insert = *w_testParser.getVecPart().cbegin();
						w_fixedAddress.insert(w_fixedAddress.cbegin(), w_val2insert); // insert at the beginning (client name)
						AddressBuilder w_adrs2Build(w_fixedAddress);
						w_complAddrs = w_adrs2Build.buildAddess();
					}
					else if (w_checkPattern == AddressParser::ePattern::FirstLineWrong)
					{
						 // this case happen when we have 2-lines 
						if (w_testParser.getVecPart().size()==2)
						{
							w_complAddrs = w_testParser.getVecPart().back();
						}
					}
				}
			}//end malfunction
			std::cout << "Complete address is: " << w_complAddrs.c_str() << "\n";
		}//end symmetry
		else
		{
			std::vector<std::string> w_fixedAddress;
			if( w_testParser.hasMalfunctions() && w_testParser.getNbLines() == 3)
			{
// 				if( w_testParser.getNbLines()==3)
// 				{
				   AddressParser::ePattern w_checkPattern = w_testParser.getPattern();
					 if( w_checkPattern==AddressParser::ePattern::Middle3Lines)
					 {
						 w_fixedAddress =
							 AddressAlgorithm::fixThreeLinesAddress(w_testParser);
						 // move semantic
						 AddressBuilder w_addrsBuilder( std::vector<std::string>(w_fixedAddress.cbegin(), w_fixedAddress.cend()));
						 w_complAddrs = w_addrsBuilder.buildAddess();
					 }
					 else if (w_checkPattern == AddressParser::ePattern::LastLineSrewUp)
					 {
						 w_fixedAddress =
							 AddressAlgorithm::fixThreeLinesAddress(w_testParser);
						 const std::string& w_beforeLastOne = w_testParser.getVecPart()[1];
						 w_fixedAddress.insert(w_fixedAddress.cbegin(), w_beforeLastOne); // insert at the beginning (street)
						 const std::string& w_firstOne = *w_testParser.getVecPart().cbegin();
						 w_fixedAddress.insert(w_fixedAddress.cbegin(), w_firstOne); // insert at the beginning (client name)
						 // move semantic
						 AddressBuilder w_addrsBuilder(std::vector<std::string>(w_fixedAddress.cbegin(), w_fixedAddress.cend()));
						 w_complAddrs = w_addrsBuilder.buildAddess();
					 }
					 else if( w_checkPattern == AddressParser::ePattern::FirstLineWrong)
					 {
						 // this case happen when we have 3-lines, take last 2 lines 
						 if( w_testParser.getVecPart().size() == 3)
						 {
							 w_fixedAddress.push_back(w_testParser.getVecPart()[1]);
							 w_fixedAddress.push_back(w_testParser.getVecPart()[2]);
							 // move semantic
							 AddressBuilder w_addrsBuilder(std::vector<std::string>(w_fixedAddress.cbegin(), w_fixedAddress.cend()));
							 w_complAddrs = w_addrsBuilder.buildAddess();
						 }
					 }
			}//if
			std::cout << "The complete address is: " << w_complAddrs.c_str() << "\n";
		} //else

		return w_complAddrs;
	}

	boost::optional<int> TestTextParser()
	{
		using namespace std;
		using namespace boost;
		using namespace boost::algorithm;

		const vector<std::string> w_listFile2Test = 
		{ "2017-11-13c_projects_millenia_9.csv",    // bug: symmetry duplicate address 6-lines
			"2017-11-13936c_projects_millenia_9.csv", // bug: symmetry non-duplicate address 4-lines
			"2017-11-15ENVIROPORC.csv",               // bug: symmetry non-duplicate address 4-lines
			"2017-12-13902KALYAN.csv",                // bug: 2-lines last line duplicate city
			"FRANCOISETHIER11002.csv",                // bug: 2-lines last line duplicate city
		  "2017-11-131030desilets.csv",             // bug: 2-lines last line duplicate city
		  "2017-11-301555porcibel.csv",             // bug: 3-lines livraison keyword
		  "2017-11-131453dujardin.csv",             // bug: 3-lines middle line screw up (insert address)
		  "2017-11-13maquille38967.csv",            // bug: 5-lines livraison keyword
		  "2017-11-AnthonyBeauregardco.csv",        // bug: 3-lines last line duplicate city
		  "2017-11-301544taratuta.csv"};            // bug: 4-lines symmetry duplicate address

		// found the address
		vector<string> w_vecOfAddressPart;
		w_vecOfAddressPart.reserve(6);
	//	const std::string w_fileName = w_listFile2Test[5];
		vector<string> w_vecOfLines;
		w_vecOfLines.reserve(50);
		for( const std::string& w_fileName : w_listFile2Test)
		{
			std::cout << "Reading the following files: " << w_fileName.c_str() << "\n";
			readCmd(w_fileName, w_vecOfLines);
		//	readCmd(w_listFile2Test[9], w_vecOfLines);
			// ready to process each line
			auto w_begRow = w_vecOfLines.cbegin();
			auto w_endRow = w_vecOfLines.cend();
			while (w_begRow != w_endRow)
			{
				if( contains(*w_begRow, "Shipped to")) // looking for the address
				{
					auto w_addressRng = std::next(w_begRow); // copy of the iterator
					do
					{
						w_vecOfAddressPart.push_back(*w_addressRng);
					} while (!contains(*w_addressRng++, string("Contact")));
					w_vecOfAddressPart.pop_back(); // Contact keyword last element which we don't want
					break; // all we need
				}
				++w_begRow;
			}//while-loop
			// ready to parse the cmd
			testParserAlgorithm(w_vecOfAddressPart);
			w_vecOfAddressPart.clear();
			w_vecOfLines.clear();
		}//for-loop

		return EXIT_SUCCESS;
	}
} // End of namespace