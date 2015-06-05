#ifndef _Bank_H_
#define _Bank_H_

#include "Account.h"

#include <vector>
#include <memory>

class Bank
{
private:
	// Private Data Members
	std::vector<std::unique_ptr<Account>> mAccounts;

	std::string mFilename;

public:
	// Constructor
	Bank();
	// Public Methods
	Account* findAccount() const;
	std::unique_ptr<Account> inputAccountDetails() const;
	void accountInquiries() const;
	void openAccount();
	void modifyAccount(Account* pAcc);
	void closeAccount(Account* pAcc);
	// Nested Exception Class
	class unfoundAccExc :public std::logic_error
	{
	public:
		explicit unfoundAccExc(const std::string& s = "\nAccount Not Found")
			: std::logic_error(s) {}
	};
};
#endif