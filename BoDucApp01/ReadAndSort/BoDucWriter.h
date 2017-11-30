#pragma once

namespace boduc 
{
	// Class to write BonLivraison file. This is at the beginning of the day 
	// and stay alive for the whole day. Pile up all other cmd processed.
  // Format for the name of the file is the following
	// BonLivraison_date.txt
	// As we process more command during the day we just append at the end og the file
	// Next day create another file with a different date 
	class BoDucWriter 
	{
	public:
		void setName(const std::string& aBonLivraisonName);
		// ... coming soon
		virtual void createReport(const std::string& aBonDeLivraison = "BonDeLivraison.txt"); //
	private:
	};
} //End of namespace
