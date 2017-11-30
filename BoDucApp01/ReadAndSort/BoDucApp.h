#pragma once

// C++ includes
#include <string>
#include <vector>
#include <map>
// App includes
#include "StructUtils.h"
#include "BonLivraisonAlgorithm.h"

namespace boduc 
{
	class BoDucApp 
	{
		// alias for vector  
		using vecofstr = std::vector<std::string>;
		using mapIntVecstr = std::map<int, vecofstr>;
		using diff_type = std::iterator_traits<vecofstr::iterator>::difference_type;

	public:
		BoDucApp( const std::vector<std::string>& aVecOfilePath);
		BoDucApp( const std::string& aFile = "request-2.csv"); // ???
		BoDucApp( const BoDucApp&) = delete;
		BoDucApp& operator= ( const BoDucApp&) = delete;

		virtual void readFile(const std::string& aFileAnPath,
			const std::string& aSplitBill = "Ordered on"); // parse and fill data structure
	
		void setWorkingDir( const std::string& aWorkPath = R"(C:\JeanWorks\BoDuc\Data\190617\)")
		{
			m_workDir = aWorkPath;
		}
		const std::string& getWorkingDir() const { return m_workDir; }
		void saveFile( const std::string& aFile2Save);
	//	void setTokenSplitCmd(const std::string& aplitCmdToken); // should be signature as default??
		inline const std::string getWorkingDir() { return std::string(m_workDir.cbegin(),m_workDir.cend()); }
		void countNbCmd();
		void setPrintOut() {/*not implemented yet*/}
		// ...
		inline const mapIntVecstr readedMap()
		{
			return std::map<int, std::vector<std::string>>(m_mapintVec.cbegin(), m_mapintVec.cend());
		}

		// algorithm to parse 
		void parseCsvFiles( const std::vector<std::string>& aVecOfiles, std::ofstream& aStream);
		void parseCsvFiles( const std::string& aFileAndPath="", const std::string& aOutputFile="");
		void writeBonLivraison( std::iostream& aStream);
		void setBonLivraisonName(std::string& aBonLivraisonName) { m_bonLivraison = aBonLivraisonName; }
		std::string getBonLivraisonName() const { return m_bonLivraison; }

		// Abstract class to provide a kind of Bridge design pattern
		// Usage:
		//  BoDucBonLivraisonAlgorithm* m_BoDucimpl = new derivedClass;
		BoDucBonLivraisonAlgorithm* m_BoDucimpl;
	private:
		std::vector<std::string> m_vecfilePath;
		std::string m_fileName;
		std::string m_workDir;
		std::string m_bonLivraison;
		BoDucFields m_readPrm;
		mapIntVecstr m_mapintVec;
	};
} // End of namespace
