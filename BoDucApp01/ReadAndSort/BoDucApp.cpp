#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <stdlib.h>

// stl includes
#include <map>
#include <vector>
// boost include
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// app includes
#include "BoDucApp.h"
#include "BoDucParsingAlgo.h"
#include "VictoBonLivraison.h"

namespace boduc
{
	void fillBoDucFieldsFromFiles( const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF);
	void fillBoDucFieldsFromFile( const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF);
}

struct checkCarrierCode 
{
	bool operator() (const std::string& aStrCode) const
	{
		return false;
	}
};

using namespace boduc;

BoDucApp::BoDucApp( const std::vector<std::string>& aVecOfilePath) 
: m_fileName(R"(jani 3156 19-06-17.csv)"),
  m_workDir(R"(C:\JeanWorks\BoDuc\Data\190617\)"),
	m_bonLivraison("BonDeLivraison.txt"),
	m_vecfilePath(aVecOfilePath),
	m_readPrm() // BoDuc field
{
   // not much to do
//	const std::string w_filePath = R"(C:\JeanWorks\BoDuc\Data\190617\)";
	//const std::string w_fileName = R"(jani 3156 19-06-17.csv)";
}

BoDucApp::BoDucApp(const std::string& aFile /*= "request-2.csv"*/) 
: m_fileName(aFile),
  m_workDir(R"(C:\JeanWorks\BoDuc\Data\190617\)"),
	m_bonLivraison("BonDeLivraison.txt"),
  //m_vecfilePath(aVecOfilePath),
  m_readPrm() // BoDuc field
{
	std::cout << "Reading the following file: " << m_workDir + m_fileName << "\n";
}

void BoDucApp::countNbCmd()
{
	diff_type w_numOfactures = std::count_if(m_vecfilePath.cbegin(), m_vecfilePath.cend(),
		[](const std::string& aStr2Look) -> bool
	{
		if( boost::algorithm::contains( aStr2Look, "Sold to"))
			return true;
		else
			return false;
	});
}

// ready to read BoDuc format file
// Keyword to search when sorting  
// Shipped to
// Date
// Produit
// Delivery Date
// Notes
// Info Ordonance
//
void BoDucApp::parseCsvFiles( const std::vector<std::string>& aVecOfiles, std::ofstream& aStream)
{
	// alias for vector  
	using vecofstr = std::vector<std::string>;
	using namespace std;
	using namespace boost;
	using namespace boost::algorithm;

	std::string w_fileandpath = m_workDir + m_fileName;
	std::cout << "We are processing the following file: " << w_fileandpath.c_str() << "\n";

	std::ofstream w_bonLivraison(std::string("BonDeLivraison_01.txt"), std::ios::out);
	assert(w_bonLivraison.is_open());
	// write title or header
	w_bonLivraison << "No Command " << "        " << "Shipped To " << "            " << "Deliver Date " << "            " <<
		"Product " << "                   " << "Quantity " << "  " << "Silo" << "\n";

	// reading
	for( const std::string& w_fileName : aVecOfiles)
	{
		if (!w_fileandpath.empty())
		{
			w_fileandpath.clear();
		}
		w_fileandpath = m_workDir + w_fileName;
		//	}

		// declare vector to store our string from cvs document 
		vecofstr w_vecStr;
		w_vecStr.reserve(50); // reserve enough space for storing each line 

		std::ifstream w_readVSV(w_fileandpath.c_str());
		if (w_readVSV) // check if its open, then read line-by-line 
		{
			for (std::string line; std::getline(w_readVSV, line);)
			{
				// std::cout << line << std::endl;
				// add it to the string to be treated
				w_vecStr.push_back(line);
			}//for-loop
		}//if

		//	std::unique_ptr<BoDucFields> w_test = std::make_unique<BoDucFields>();
		BoDucFields w_testRead;
		fillBoDucFieldsFromFiles(w_vecStr, std::string("Date"), std::ref(w_testRead));
		fillBoDucFieldsFromFiles(w_vecStr, std::string("Shipped to"), std::ref(w_testRead));
		fillBoDucFieldsFromFiles(w_vecStr, std::string("Date promise"), std::ref(w_testRead));
		fillBoDucFieldsFromFiles(w_vecStr, std::string("produit et description"), std::ref(w_testRead));
		fillBoDucFieldsFromFiles(w_vecStr, std::string("TM"), std::ref(w_testRead));

	//	m_readPrm = w_testRead;
// 		aStream << m_readPrm.m_noCmd << "      " << m_readPrm.m_deliverTo << m_readPrm.m_datePromise <<
// 			" " << m_readPrm.m_produit << "   " << m_readPrm.m_qty << "      " << m_readPrm.m_silo << "\n";
		
		w_bonLivraison << w_testRead.m_noCmd << "      " << w_testRead.m_deliverTo << w_testRead.m_datePromise <<
			" " << w_testRead.m_produit << "   " << w_testRead.m_qty << "      " << w_testRead.m_silo << "\n";

		if (!w_vecStr.empty())
		{
			w_vecStr.clear();
		}
	}//for-loop

	w_bonLivraison.close();
}

void BoDucApp::parseCsvFiles( const std::string& aIntputFile/*=""*/, const std::string& aOutputFile/*=""*/)
{
	using namespace std;
	using namespace boost;
	using namespace boost::algorithm;

	std::string w_fileandpath;
	if( aIntputFile.empty())
	{
		w_fileandpath = m_workDir + m_fileName;
	}
	else
	{
		w_fileandpath = aIntputFile;
	}
	std::cout << "We are processing the following file: " << w_fileandpath.c_str() << "\n";

	// algorithm
	BoDucParsingAlgo w_testAlgo;
	size_t w_checknbCmd = w_testAlgo.nbOfCmd(w_fileandpath);

	w_testAlgo.readFile(w_fileandpath, std::string("Signature")); //can set the splitting token default "Ordered on"
 	unique_ptr<BoDucBonLivraisonAlgorithm> w_bonLivraison = make_unique<VictoBonLivraison>();
 	w_testAlgo.parseCsv( w_bonLivraison.get());
//	w_testAlgo.parseCsv();
	w_testAlgo.createReport();

	std::cout << "Just finished processing command report\n";
}

void BoDucApp::writeBonLivraison(std::iostream& aStream)
{
	aStream << m_readPrm.m_noCmd << "      " << m_readPrm.m_deliverTo << m_readPrm.m_datePromise <<
		" " << m_readPrm.m_produit << "   " << m_readPrm.m_qty << "      " << m_readPrm.m_silo << "\n";
}

void BoDucApp::readFile( const std::string& aFileAnPath, const std::string& aSplitBill)
{
	using namespace std;
	using namespace boost;
	using namespace boost::algorithm;

	// create alias
	using vecofstr = std::vector<std::string>;

	// declare vector to store our string from cvs document 
	vecofstr w_vecStr;
	w_vecStr.reserve(50); // reserve enough space for storing each line ??

												// Command in one file, reading the command by splitting with the "Ordered on"
	short i(0);
	ifstream w_readVSV(aFileAnPath.c_str());
	if (w_readVSV) // check if its open, then read line-by-line 
	{
		for (string line; std::getline(w_readVSV, line);)
		{
			if( i == 0) // first line is obsolete
			{
				// check if contains "Ordered on" as first line
				// NOTE "request-2.csv" we split the bill with this token, 
				// and format was that first line contains this string
				if (contains(line, "Ordered on"))
				{
					++i;  // next line
					continue; // i don't see why we should increment it
				}
			}//if(i==0)
			w_vecStr.push_back(line);

			// NOTE we assume that we are at the last line of the command
			// then we check if the carrier string name is part of the whole 
			// command (if so, add it to the map otherwise skip it)
			// IMPORTANT this algorithm assume that we are at the end or the 
			// last line (split into separate command is based on this assumption)
			// if not the case then it won't work!!
			if (contains(line, aSplitBill))
			{
				// check for carrier name some are different and don't need to be treated
				// Francois mentioned that field can be blank, need to be careful
				// Also, we must check also for the "TM" tonne metrique, if not present
				// the cmd is discarded
				if (any_of(w_vecStr.cbegin(), w_vecStr.cend(),
					[](const std::string& aStr2Look) -> bool     // lambda (anonymous) function
				{
					return (contains(aStr2Look, "NIR R-117971-3 TRSP CPB INC")
						|| contains(aStr2Look, "NIR R-117971-3 TRANSPORT CPB")); // sometime we have empty (blank field) string
				}
				)//any_of
					)//if
				{
					m_mapintVec.insert(make_pair(i++, w_vecStr));
					w_vecStr.clear();
				}
				else
				{
					/*w_mapofCmd.insert(std::make_pair(i++,w_vecStr));*/
					w_vecStr.clear();
				}
			}
		}//for-loop
	}//if
}