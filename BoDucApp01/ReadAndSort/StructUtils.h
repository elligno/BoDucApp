
#pragma once

namespace boduc 
{
	struct BoDucFields
	{
		// default ctor
		BoDucFields() 
	 : m_noCmd(""), 
		 m_datePromise(""), 
		 m_deliverTo(""), 
		 m_produit(""),
			m_silo(""),
			m_prodCode(0), // same line as TM (1,/123456789,  qty, silo) page, product code, quantity and silo number/string
		 //m_silo(0),       "TM" tonne metrique must be present otherwise the cmd is discarded
		 m_qty(0.f)
		{}

		std::string m_noCmd;
		std::string m_datePromise;
		std::string m_deliverTo;
		std::string m_produit;
		signed long m_prodCode;  
		//int m_silo;  not always an integer (can be 2&3, 4-A and a string such as "Un seul silo")
		std::string m_silo;
		float m_qty; // better to set as a string
	};	//typedef BoDucFields BoDFields;

	class BoDucTrimming 
	{
	public:
// 		BoDucTrimming ();
// 		~BoDucTrimming ();
		static void TrimBothEndsAndSplit( const std::string& aStr2Trim, std::vector<std::string>& aSplittedStr)
		{
			using namespace std;
			using namespace boost;
			using namespace boost::algorithm;

			std::string w_tmpStr( aStr2Trim.cbegin(), aStr2Trim.cend());
			trim_right_if(w_tmpStr, is_any_of(","));
			trim_left_if(w_tmpStr, is_any_of(","));

			// splitting string to retrieve Cxxx cmd no
	//		vector<string> strs;
			boost::split( aSplittedStr, w_tmpStr, boost::is_any_of(","));
	}
	private:
		// not sure yet
	};

	// check if all characters in a string are digits
// 	bool is_digits( const std::string& str)
// 	{
// 		return std::all_of( str.begin(), str.end(), ::isdigit); // C++11
// 	}
} // End of namespace
