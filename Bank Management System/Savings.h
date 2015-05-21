#ifndef _Savings_H_
#define _Savings_H_

#include "Account.h"

class Savings : public Account
{
private:
	// Private Data Members
	double	 mInterestRate;
	unsigned mWithdrawalAmounts;

public:
	// Constructor
	Savings(unsigned number, const std::string& name = "Unknown",
			unsigned balance = 0.0);
	// Public Methods
	void updateBalance();
	void transaction(const TransactionType& type);
	void inquiry() const;
};
#endif