
// C++ inc;udes 
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
// stl includes
#include <vector>
// boost includes
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> // string algorithm
#include <boost/algorithm/string/split.hpp> // splitting algo
// App includes
#include "StructUtils.h"

// not sure what for?
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// int main()
// {
// 	std::wofstream fs("testout.txt");
// 	const wchar_t* txt = L"some Unicode text ςΰθι€§";
// 	MessageBoxW(0, txt, L"verify", MB_OK);
// 	fs &lt; &lt; txt &lt; &lt; std::flush;
// 	MessageBoxW(0, fs.good() ? L"Good" : L"Bad", L"verify", MB_OK);
// 	return 0;
// }

namespace boduc 
{
// 	// check if all characters in a string are digits
// 	bool is_digits(const std::string &str)
// 	{
// 		return std::all_of( str.begin(), str.end(), ::isdigit); // C++11
// 	}

	// Date (no cmd)
	// Date promise 
	// Shipped to (client)
	// 
	void fillBoDucFieldsFromFiles( const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF)
	{
		using namespace std;
		using namespace boost::algorithm;

		//	using vecstr = std::vector<std::string>;
		short vecPos(0);
		auto begVec = aCmdVec.begin();
		while (begVec != aCmdVec.cend())
		{
			const std::string& str2look = *begVec;
			if (boost::algorithm::contains(str2look, aField2Look.c_str()))
			{
				// retrieve the cmd no which is next line
				std::string w_fieldValue = aCmdVec[++vecPos];
				/*trim_right_if(w_fieldValue, is_any_of(","));*/
				if (::strcmp("Shipped to", aField2Look.c_str())==0)
				{
					w_fieldValue.erase(0,1); // erase first three char
					w_fieldValue.erase(w_fieldValue .size()-1);
          //TODO: this work only for the case of Ferme Jani (debugging purpose)					
					//w_fieldValue.erase(w_fieldValue.begin()+w_pos,w_fieldValue.end()); original
					size_t w_pos = w_fieldValue.find_last_of('M');
					if (w_pos != std::string::npos)
					{
						w_fieldValue.erase(w_fieldValue.begin() + w_pos, w_fieldValue.end());
					}

					aBoducF.m_deliverTo = w_fieldValue; // assume that the last field is the right one 
					break;
				}
				if (::strcmp("Date promise", aField2Look.c_str()) == 0)
				{
					// splitting string to retrieve Cxxx cmd no
					vector<string> strs;
					boost::split(strs, w_fieldValue, boost::is_any_of(","));
					strs.erase(std::remove_if(strs.begin(), strs.end(), // delete empty element
						[](const std::string& s)
					{ return s.empty(); }));
					std::vector<std::string> w_datepromise;
					boost::split(w_datepromise, strs[1], boost::is_any_of(" ")); // need to be split
					aBoducF.m_datePromise = w_datepromise[0];
					break;
				}
				if (::strcmp("produit et description", aField2Look.c_str()) == 0)
				{
					std::string w_prodDescr1 = aCmdVec[++vecPos]; // product description
					std::vector<std::string> w_prodDescrp1;
					boost::split(w_prodDescrp1, w_prodDescr1, boost::is_any_of(" "));
//					if (w_prodDescrp1.size()!=1)
//					{
						w_prodDescrp1.erase(std::remove_if(w_prodDescrp1.begin(), w_prodDescrp1.end(), // delete empty element
							[](const std::string& s)
						{ return s.empty(); }));
						auto w_begp1 = w_prodDescrp1.cbegin() + 1;
						std::string w_rebuildStr;
						do
						{
							w_rebuildStr += *w_begp1++ + std::string(" ");
						} while (!(*w_begp1).empty());

						// there anymore description
						std::string w_prodDescr2 = aCmdVec[++vecPos];
						std::string w_wholeDesr = w_rebuildStr + std::string(" ") + w_prodDescr2;
//					}
					
					// there anymore description
	//				std::string w_prodDescr2 = aCmdVec[++vecPos];
	//				std::string w_wholeDesr = w_rebuildStr + std::string(" ") + w_prodDescr2;
					aBoducF.m_produit = w_wholeDesr;
					break;
				}
				if (::strcmp("TM", aField2Look.c_str()) == 0)
				{
					// Qty and silo
					const std::string& w_qtySilo = *begVec;
					auto pos = w_qtySilo.find_first_of("TM");
					if (pos != std::string::npos)
					{
						std::string w_str2Trim(w_qtySilo.cbegin()+pos, w_qtySilo.cend());
						// want to retrieve Qty and Silo
						std::vector<std::string> w_splitStr;
						boost::split(w_splitStr, w_str2Trim, boost::is_any_of(","));
						auto w_bp1 = w_splitStr.cbegin()+1;
						std::istringstream iss;
						iss.str(w_splitStr[1]);
						iss >> aBoducF.m_qty;
						aBoducF.m_silo = 1;     //TODO: this value is hard coded, need to retrieve silo number
					}
					break;
				}
				else //if(::strcmp("Date", aField2Look.c_str()) == 0)
				{
					trim_right_if(w_fieldValue, is_any_of(","));
					// splitting string to retrieve Cxxx cmd no
				  vector<string> strs;
				  boost::split(strs, w_fieldValue, boost::is_any_of(","));
				  // last element is the cmd no (assume this for now)
				  aBoducF.m_noCmd = strs.back(); // assume that the last field is the right one 
		//		std::cout << "Cmd is : " << aBoducF.m_noCmd.c_str() << "\n";
				break;
				}

			}
			++begVec;
			++vecPos;
		}
	 }

	 // +++++++++++++++++++++++++++++++++++++++++
	 // considering many command in the same file
	 // +++++++++++++++++++++++++++++++++++++++++

	void fillBoDucFieldsFromFile( const std::vector<std::string>& aCmdVec, const std::string& aField2Look, BoDucFields& aBoducF)
	{
		int linenb(0);
		using namespace std;
		using namespace boost::algorithm;

		// workaround to take account the CSV formatting when cmd are tied together  
		if (::strcmp(aField2Look.c_str(),"No Command")==0) 
		{
			std::string w_fieldValue = aCmdVec[0];        // based on "Ordered on"
			trim_right_if(w_fieldValue, is_any_of(","));
			trim_left_if(w_fieldValue, is_any_of(","));
			
			// splitting string to retrieve Cxxx cmd no
			vector<string> strs;
			boost::split(strs, w_fieldValue, boost::is_any_of(","));
			// last element is the cmd no (assume this for now)

			//aBoducF.m_noCmd = strs.back(); assume that the last field is the right one 
		  // check for the first character 'C' for command number
			if( strs.back()[0]== 'C')
			{
				aBoducF.m_noCmd = strs.back(); // assume that the last field is the right one 
			}
			else
			{
				// need to check for "Date" first of
				for( const std::string& w_checkStr : aCmdVec)
				{
					// looking for "Date" then break
					if( contains(w_checkStr,"Date"))
					{
						linenb++; // line below which contains the command number
						break;
					}
					linenb++;
				}
				w_fieldValue = aCmdVec[linenb];        // based on "Ordered on"
				trim_right_if(w_fieldValue, is_any_of(","));
				trim_left_if(w_fieldValue, is_any_of(","));
				// splitting string to retrieve Cxxx cmd no
				vector<string> strs;
				boost::split( strs, w_fieldValue, boost::is_any_of(","));
				aBoducF.m_noCmd = strs.back();
			}//else

	//		std::cout << "Cmd is : " << aBoducF.m_noCmd.c_str() << "\n";
			return; // shouldn't do that, but no choice
		}

		short vecPos(0);
		auto begVec = aCmdVec.begin();
		while( begVec != aCmdVec.cend()) // go through all element cmd line
		{
			const std::string& str2look = *begVec;
			if( boost::algorithm::contains( str2look, aField2Look.c_str()))
			{
				// retrieve the cmd no. which is next line
				std::string w_fieldValue = aCmdVec[++vecPos];  //next line (below current which contains field to look)

				// to be completed later
				if( ::strcmp("Shipped to", aField2Look.c_str()) == 0)
				{
					std::string w_addressValue = w_fieldValue; // initialize
					std::string w_completeAddress;     // store part of the complete address
					std::vector<string> w_addressPart;

					do // address may take 2 cells, if more it's because of 
					{  // conversion mismatch and generally we use the last 2 lines
						// put in comment for now, may be in use in the future
			//			w_addressValue.erase(0, 1); // erase first three char
			//			w_addressValue.erase(w_addressValue.size() - 1);

						trim_right_if(w_addressValue, is_any_of(","));
						trim_left_if(w_addressValue, is_any_of(","));
						w_addressPart.push_back(w_addressValue);
						//w_completeAddress += w_addressValue + std::string(" ");
						w_addressValue = aCmdVec[++vecPos];
					} while (!contains(w_addressValue,"Contact"));
		
		//			w_fieldValue.erase(0, 1); // erase first three char
		//			w_fieldValue.erase(w_fieldValue.size() - 1);

					//TODO: this work only for the case of Ferme Jani (debugging purpose)					
					//w_fieldValue.erase(w_fieldValue.begin()+w_pos,w_fieldValue.end()); original
// 					size_t w_pos = w_fieldValue.find_last_of('M');
// 					if (w_pos != std::string::npos)
// 					{
// 						w_fieldValue.erase(w_fieldValue.begin() + w_pos, w_fieldValue.end());
// 					}
		//			trim_right_if(w_fieldValue, is_any_of(","));
		//			trim_left_if(w_fieldValue, is_any_of(","));
					if (w_addressPart.size()>2)
					{
						// we need to take the last 2 in most cases
						using riter = std::reverse_iterator<std::vector<std::string>::iterator>;
						riter w_lastPart = w_addressPart.rbegin()+1;
						w_completeAddress += *w_lastPart + std::string(" ");
						w_lastPart = w_addressPart.rbegin();
						w_completeAddress += *w_lastPart;
						aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
					}
					else if(w_addressPart.size()==2)
					{
						// rebuild the whole address
						w_completeAddress += w_addressPart[0] + std::string(" ");
						w_completeAddress += w_addressPart[1];
					  aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one 
					}
					else if (w_addressPart.size() == 1)
					{
						w_completeAddress += w_addressPart[0];
						aBoducF.m_deliverTo = w_completeAddress; // assume that the last field is the right one
					}
					else
					{
						std::cout << "There is a problem with Customer address\n";
					}
					break;
				}
				if (::strcmp("Date promise", aField2Look.c_str()) == 0)
				{
					// splitting string to retrieve Cxxx cmd no
					vector<string> strs;
					boost::split(strs, w_fieldValue, boost::is_any_of(","));
					strs.erase(std::remove_if(strs.begin(), strs.end(), // delete empty element
						[](const std::string& s)
					{ return s.empty(); }));
		//			std::unique(strs.begin(), strs.end());
					strs.erase(std::unique(strs.begin(),strs.end()),strs.end());
					std::vector<std::string> w_datepromise;
					//boost::split(w_datepromise, strs[1], boost::is_any_of(" ")); // need to be split
					boost::split(w_datepromise, strs[strs.size()-3], boost::is_any_of(" ")); // need to be split
					aBoducF.m_datePromise = w_datepromise[0];
					break;
				}
				if (::strcmp("produit et description", aField2Look.c_str()) == 0)
				{
					short w_tmp = vecPos;
					// for some command we don't get
					std::string w_prodDescr1 = aCmdVec[vecPos++]; // product description

					// check if the first number is a digit
					std::locale loc;
					if( !std::isdigit(static_cast<unsigned char>(w_prodDescr1[0]),loc))
					{
						//need to do something, check for next line
						w_prodDescr1 = aCmdVec[++w_tmp]; // next line
#if 0
						// check again for a digit as the first character
						std::locale loc;
						if( std::isdigit(static_cast<unsigned char>(w_prodDescr1[0]), loc))
						{
							// check if it is equal to 1
							int w_descrNo = boost::lexical_cast<int>(w_prodDescr1[0]);
							if( w_descrNo==1)
							{
								// we are at right place in the command
								std::cout << "Product number is: " << w_descrNo << "\n";
							}
						}
#endif
						// +++++++++++++++++++++++++++++++++++++++++++++++++++++
						size_t w_charPos = w_prodDescr1.find_first_of(" ");
						std::string w_subStr(w_prodDescr1.begin(), w_prodDescr1.begin()+w_charPos);
						std::vector<std::string> w_codeSplit;
						boost::split( w_codeSplit, w_subStr, boost::is_any_of(","));
						// search for first digit
						std::string w_prodCode;
						if( std::all_of(w_codeSplit[1].begin(), w_codeSplit[1].end(), ::isdigit)) //!is_digits(w_codeSplit[1]))
						{
							// we have to remove non-digit character from string 
							std::string::iterator w_foundDigit = std::find_if(w_codeSplit[1].begin(), w_codeSplit[1].end(),
								[](char c)
							{
								std::locale loc;
								return std::isdigit(static_cast<unsigned char>(c),loc);
							});
							size_t w_next = std::distance(w_codeSplit[1].begin(),w_foundDigit);
							// product code
							//w_prodCode = std::string(w_codeSplit[1].begin()+w_foundDigit, w_codeSplit[1].end());
							std::string::iterator w_chck = w_codeSplit[1].begin();
							std::advance(w_chck, w_next);
						//	std::string::size_type w_strLength = w_codeSplit[1].size();
							std::string w_codeProd(w_chck, w_codeSplit[1].end());
							//std::string::const_iterator w_test = w_chck + w_foundDigit;
					//		w_prodCode = w_codeSplit[1];
		//				  std::copy(w_chck, w_codeSplit[1].end(), w_prodCode.begin());
							std::cout << "Product code is: " << w_codeProd;
						}
						else
						{
							w_prodCode = w_codeSplit[1];
						}
						// last part is the product description
						std::string w_lastPart(w_prodDescr1.cbegin()+w_charPos, w_prodDescr1.cend());
						boost::trim_left(w_lastPart);
						std::vector<string> w_descrStr;
						boost::split( w_descrStr,w_lastPart, boost::is_any_of(","));
						// check size
					  std::vector<std::string>::size_type w_lastSize = w_descrStr.size();
						if (w_lastSize==1)
						{
							std::cout << "That's product description\n";
						}
						aBoducF.m_produit = w_descrStr[0];

				//		using dist = std::iterator_traits<std::string::iterator>::difference_type;
				//		dist w_startStr = std::distance(aCmdVec.cbegin(), lineIter);
						auto w_lineVec = aCmdVec.cbegin() + vecPos;
						while (w_lineVec != aCmdVec.cend())
						{
							if( boost::algorithm::contains(*w_lineVec, std::string("TM")))
							{
								std::vector<std::string> w_splitStr;
								boost::split(w_splitStr, *w_lineVec, boost::is_any_of(","));

								// remove all empty string, then we have as first element the "TM" string
								// just after we have the qty and silo 
								w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
									[](const std::string& s)
								{ return s.empty(); }), w_splitStr.cend());
								
								auto w_look4TM = std::find( w_splitStr.cbegin(), w_splitStr.cend(), std::string("TM"));
								//size_t distFromBeg = std::distance();
								++w_look4TM;
						//		std::string w_quant = *w_look4TM++;
						//		std::string w_silau = *w_look4TM;
								// sanity check
								//assert(3 == w_splitStr.size());

								aBoducF.m_qty = std::stof(*w_look4TM++);
							//	aBoducF.m_qty = w_qty;

								// NOTE: some of the silo number has the following format "03"
								// want to remove the first character since it is not relevant
								if ((*w_look4TM).find("0") != std::string::npos)
								{
									//(*w_look4TM).erase((*w_look4TM).find("0"), 1);
									aBoducF.m_silo = (*w_look4TM)[1];
								}
								else
								{
									aBoducF.m_silo = *w_look4TM;
								}
								break; // no point to go further
							}
							++w_lineVec;
						}

				//		float w_qty = std::stof(w_splitStr[1]);
				//		aBoducF.m_qty = w_qty;

						// split about "," then take last element as product code
						// position at first empty string and then search for the 
						// non empty string that will be the description string beginning
						// again split about "," and look if there is any result 
						// +++++++++++++++++++++++++++++++++++++++++++++++++++++

					}//if

// 					 // patch to consider the case where description is in the same cell as TM and qty silo
// 					std::string::size_type w_foundComma = w_prodDescr1.find_first_of(',');
// 					if (w_foundComma != std::string::npos)
// 					{
// 						// we found something (eraae all characters to end)
// 						w_prodDescr1.erase(w_prodDescr1.cbegin() + w_foundComma, w_prodDescr1.cend());
// 					}
#if 0
					std::vector<std::string> w_prodDescrp1;
				//	w_prodDescrp1.reserve(10);
					boost::split(w_prodDescrp1, w_prodDescr1, boost::is_any_of(" "));
					if (w_prodDescrp1.size() != 1)
					{
// 						w_prodDescrp1.erase(std::remove_if(w_prodDescrp1.begin(), w_prodDescrp1.end(), // delete empty element
// 							[](const std::string& s)
// 						{ return s.empty(); }));

						// remove all empty string
						w_prodDescrp1.erase(std::remove(w_prodDescrp1.begin(), w_prodDescrp1.end(), std::string("")));
						auto w_findIt = std::find_if(w_prodDescrp1.cbegin(),w_prodDescrp1.cend(),
							[](const std::string& aStr) -> bool {return aStr.empty(); }
						);
						using diff_type = std::iterator_traits<std::vector<std::string>::iterator>::difference_type;
						diff_type w_dif = std::distance(w_prodDescrp1.cbegin(), w_findIt);
					  //std::string w_lastStr = w_findIt-1;
						//w_prodDescrp1.shrink_to_fit();

						// check last element
						std::string& w_lastOne = w_prodDescrp1[w_dif-1];
						std::string::size_type w_foundComma = w_lastOne.find_first_of(',');
						if( w_foundComma != std::string::npos)
						{
							// we found something (eraae all characters to end)
							w_lastOne.erase(w_lastOne.cbegin() + w_foundComma, w_lastOne.cend());
						}

						auto w_begp1 = w_prodDescrp1.cbegin() + 1; // first element is the number, skip
						std::string w_rebuildStr;
						do
						{
							w_rebuildStr += *w_begp1++ + std::string(" ");
						} while (!(*w_begp1).empty());

						// there anymore description
					//	std::string w_prodDescr2 = aCmdVec[++vecPos]; in the first version of the algorithm, 
						std::string w_wholeDesr = w_rebuildStr + std::string(" "); //+ w_prodDescr2;
						aBoducF.m_produit = w_wholeDesr;
					}
#endif
					// there anymore description
					//				std::string w_prodDescr2 = aCmdVec[++vecPos];
					//				std::string w_wholeDesr = w_rebuildStr + std::string(" ") + w_prodDescr2;
					//aBoducF.m_produit = "";w_wholeDesr;
					break;
				}
				if( ::strcmp("TM", aField2Look.c_str()) == 0)
				{
					// Qty and silo
					const std::string& w_qtySilo = *begVec;
					// find position of "TM" in the whole string
			//		auto pos = w_qtySilo.find_last_of("TM");
				
					std::string w_tm("TM");
					const auto pos = std::search(w_qtySilo.cbegin(), w_qtySilo.cend(),
						w_tm.cbegin(), w_tm.cend());//w_qtySilo.find_last_of("TM");

					if( pos != w_qtySilo.cend())
					{
						using dist = std::iterator_traits<std::string::iterator>::difference_type;
						dist w_startStr = std::distance(w_qtySilo.cbegin(), pos);
						//		auto w_begFound = w_qtySilo.cbegin();
						//	  	std::advance(w_qtySilo.cbegin(),pos);
						std::string w_str2Trim(w_qtySilo.cbegin() + w_startStr, w_qtySilo.cend());

// 					if (pos != std::string::npos)
// 					{
					//	std::string w_str2Trim(w_qtySilo.cbegin() + pos, w_qtySilo.cend());
						// want to retrieve Qty and Silo
						std::vector<std::string> w_splitStr;
						boost::split(w_splitStr, w_str2Trim, boost::is_any_of(","));
						// remove all empty string, then we have as first element the "TM" string
						// just after we have the qty and silo 
						w_splitStr.erase(std::remove_if(w_splitStr.begin(), w_splitStr.end(), // delete empty element
							[](const std::string& s)
						{ return s.empty(); }), w_splitStr.cend());
						// sanity check
						assert(3 == w_splitStr.size());
						
						float w_qty = std::stof(w_splitStr[1]);
						aBoducF.m_qty = w_qty;

						// not as a number but a string
					//	int w_silo = std::stoi(w_splitStr[2]);
					//	aBoducF.m_silo = w_silo;

						// NOTE: some of the silo number has the following format "03"
						// want to remove the first character since it is not relevant
						if(w_splitStr[2].find("0")!=std::string::npos)
						{
							w_splitStr[2].erase(w_splitStr[2].find("0"),1);
						}
						aBoducF.m_silo = w_splitStr[2]; 
#if 0
						//		auto w_bp1 = w_splitStr.cbegin() + 1;
						//		float w_qty = std::stof(*w_bp1++);
						auto w_look4TM = std::find(w_splitStr.cbegin(), w_splitStr.cend(), std::string("TM"));
						std::advance(w_look4TM, 1);
						float w_qty = std::stof(*w_look4TM);
						//	std::istringstream iss;
						//	iss.str(*w_bp1++);
						//	iss >> aBoducF.m_qty;
						aBoducF.m_qty = w_qty;
						//	iss.str(*w_bp1);
						std::advance(w_look4TM, 1);
						// check if it is a digit first, if not don't put anything
						int w_silo = std::stoi(*w_look4TM);
						aBoducF.m_silo = w_silo;
						//iss >> aBoducF.m_silo;

						//aBoducF.m_silo = 1;       hard coded
#endif
					}
					break;
				}
				else //no command
				{
					trim_right_if(w_fieldValue, is_any_of(","));
					trim_left_if(w_fieldValue, is_any_of(","));
					// splitting string to retrieve Cxxx cmd no
					vector<string> strs;
					boost::split(strs, w_fieldValue, boost::is_any_of(","));
					// last element is the cmd no (assume this for now)

					aBoducF.m_noCmd = strs.back(); // assume that the last field is the right one 
				  std::cout << "Cmd is : " << aBoducF.m_noCmd.c_str() << "\n";
					break;
				}
			}
			++begVec;
			++vecPos;
		}
	}
} // end of namespace